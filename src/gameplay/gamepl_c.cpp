/*
 * gameplay/gamepl_c.cpp
 *
 * The gameplay's calc loops.
 *
 */

#include <algorithm> // std::sort for the console message after spectating
#include <sstream>   // console message creation

#include "gameplay.h"

#include "../api/manager.h"
#include "../other/user.h"

void Gameplay::calc()
{
    // Wenn nicht gestartet, macht dies nix
    Network::calc();

    // do this always, even when the window is on top
    chat.calc();

    if (window_gameplay) {
        calc_window();
        // This is a bit kludgy, but it prevents opening the window
        // immediately again during a network game on an ESC press
        if (window_gameplay && Network::get_started()
         && !window_gameplay->get_game_end()) calc_self();
    }
    else calc_self();
}



void Gameplay::calc_window()
{
    mouse_cursor.set_x_frame(0);
    mouse_cursor.set_y_frame(0);

    Api::WindowGameplay::ExitWith exit_with = window_gameplay->get_exit_with();
    if (exit_with != Api::WindowGameplay::NOTHING) {
        switch (exit_with)
        {
        case Api::WindowGameplay::RESUME:
            break;

        case Api::WindowGameplay::MENU:
            save_result();
            // Auto-save the replay. I can't tell whether it was just a
            // watched singleplayer replay, so we'll save all successful
            // plays/views in singleplayer.
            if (multiplayer && ! replaying) {
                write_outcome_to_console();
                replay.save_as_auto_replay(&level);
            }
            if (! multiplayer && trlo->lix_saved >= trlo->required) {
                replay.save_as_auto_replay(&level);
            }
            exit = true;
            break;

        case Api::WindowGameplay::RESTART:
            save_result();
            replay.erase_early_singleplayer_nukes();
            load_state(state_manager.get_zero());
            pan.pause      .set_off();
            pan.speed_slow .set_off();
            pan.speed_fast .set_off();
            pan.speed_turbo.set_off();
            break;

        default:
            break;
        }
        delete window_gameplay;
        window_gameplay = 0;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



void Gameplay::calc_self()
{
    // Berechnungen, die in jedem Tick, nicht nur in jedem Update
    // ausgeführt werden müssen
    ++local_ticks;

    // Aendert die Mauskoordinaten ggf., also frueh aufrufen!
    map.calc_scrolling();

    mouse_cursor.set_x_frame(0);
    mouse_cursor.set_y_frame(0);

    if (malo && malo->aiming == 2) {
        mouse_cursor.set_x_frame(1);
        mouse_cursor.set_y_frame(2);
    }
    else if (map.get_scrolling_now()) mouse_cursor.set_x_frame(3);



    // ec kurz fuer exit_count
    unsigned ec = 0;
    for (Tribe::CIt t = cs.tribes.begin(); t != cs.tribes.end(); ++t)
        ec += t->lix_hatch + t->get_lix_out();
    for (TrigIt i = cs.trap.begin(); i != cs.trap.end(); ++i)
        ec += i->get_x_frame();
    if (pan.speed_turbo.get_on() || pan.speed_fast.get_on())
         ec += effect.get_effects_without_debris();
    else ec += effect.get_effects();
    // Check also for !window_gameplay, as in a networked game,
    // the action keeps running in the background
    if (ec == 0 && !window_gameplay) {
        chat.set_type_off();
        if (multiplayer && spectating && ! replaying) {
            write_outcome_to_console();
            replay.save_as_auto_replay(&level);
            exit = true;
        }
        else if (multiplayer) {
            window_gameplay = new Api::WindowGameplay(&replay,
                cs.tribes, trlo, (malo == 0), &level);
            Api::Manager::add_focus(window_gameplay);
        }
        else {
            // Singleplayer
            window_gameplay = new Api::WindowGameplay(&replay, trlo,
                // Eye candy when not saving anything, won't be saved anyway
                (trlo->lix_saved > 0 ? update_last_exiter : cs.update),
                level.required, level.initial, level.get_name());
            Api::Manager::add_focus(window_gameplay);
        }
        return;
    }
    // Beachte das return im vorhergehenden if.




    // Abort a singleplayer action replay?
    if (replaying && ! multiplayer) {
        const int& csu = cs.update;
        const int  max = replay.get_max_updates();
        const int& fff = turbo_times_faster_than_fast;

        bool abort_replay = false;

        if (csu >= max || max == 0) abort_replay = true;
        if (hardware.get_ml() &&
         ! (pan.state_save .is_mouse_here()
         || pan.state_load .is_mouse_here()
         || pan.pause      .is_mouse_here()
         || pan.zoom       .is_mouse_here()
         || pan.speed_slow .is_mouse_here()
         || pan.speed_fast .is_mouse_here()
         || pan.speed_turbo.is_mouse_here()
         || pan.restart    .is_mouse_here())) abort_replay = true;
        for (size_t i = 0; i < pan.skill.size(); ++i)
         if (pan.skill[i].get_clicked()) abort_replay = true;
        if (pan.rate_minus.get_clicked()
         || pan.rate_plus .get_clicked()
         || hardware.key_hold(useR->key_rate_minus)
         || hardware.key_hold(useR->key_rate_plus)) abort_replay = true;
        // see comment in gamepl_a.cpp for why the rate doesn't have hotkeys.
        if (abort_replay) {
            replaying = false;
            replay.erase_data_after_update(cs.update);
        }
        // abort fast-forward earlier than this
        if (useR->replay_cancel) {
            const int rcat = useR->replay_cancel_at;
            if (pan.speed_fast .get_on() && csu == max - rcat)
                pan.speed_fast .set_off();
            if (pan.speed_turbo.get_on() && csu >= max - rcat
                                         && csu <  max - rcat + fff)
                pan.speed_turbo.set_off();
        }
    }

    // Konsole deaktiviert, normale Buttons ansehen und,
    // wenn kein Replay stattfindet, auch den aktiven Main-Loop abarbeiten
    // console_tt_on_last_frame is a bit kludgy, but it works nicely.
    // Gameplay is not an elder, so this will get calced every frame here,
    // and without said variable, the console would be opened again upon
    // sending text with enter or the menu would open on cancelling typing.
    if (!chat.get_type_on()) {
        // Speichern
        if (pan.state_save.get_clicked()) {
            state_manager.save_user(cs, replay);
            Sound::play_loud(Sound::DISKSAVE);
        }

        // Laden
        if (pan.state_load.get_clicked()) {
            const GameState& sta = state_manager.get_user();
            if (sta) {
                // If saved replay extends current one,  pick saved one.
                // If saved replay doesn't compare,      pick saved one.
                // If equal, for performance,            pick ours.
                // If saved replay is shorter than ours, pick ours.
                const Replay& userrep = state_manager.get_user_replay();
                if (! replay.equal_before(userrep, sta.update)) {
                    replay = userrep;
                }
                save_result();
                pan.speed_fast .set_off();
                pan.speed_turbo.set_off();
                load_state(sta);
            }
        }

        // Pause
        if (pan.pause.get_clicked()) {
            pan.pause.set_on(!pan.pause.get_on());
            pan.speed_slow.set_off();
            pan.speed_fast.set_off();
            pan.speed_turbo.set_off();
        }
        // Zoom
        if (pan.zoom.get_clicked()) {
            pan.zoom.set_on(!pan.zoom.get_on());
            map.set_zoom(pan.zoom.get_on());
        }
        // Geschwindigkeit
        {
            GameplayPanel::BiB* b
             = pan.speed_slow .get_clicked() ? &pan.speed_slow
             : pan.speed_fast .get_clicked() ? &pan.speed_fast
             : pan.speed_turbo.get_clicked() ? &pan.speed_turbo : 0;
            if (b) {
                bool was_on = b->get_on();
                pan.pause      .set_off();
                pan.speed_slow .set_off();
                pan.speed_fast .set_off();
                pan.speed_turbo.set_off();
                b->set_on(!was_on);
            }
        }
        // Neustart
        if (pan.restart.get_clicked()) {
            save_result();
            replay.erase_early_singleplayer_nukes();
            pan.speed_fast .set_off();
            pan.speed_turbo.set_off();
            load_state(state_manager.get_zero());
        }

        // Switch the spectator's panel to a different tribe's skillset
        if (pan.spec_tribe.get_clicked()) {
            if (! cs.tribes.empty()) {
                for (Tribe::It itr = cs.tribes.begin();
                    itr != cs.tribes.end(); ++itr) {
                    if (itr == --cs.tribes.end()) {
                        trlo = &*cs.tribes.begin();
                    }
                    else if (trlo == &*itr) {
                        trlo = &*(++itr); break;
                    }
                }
                pan.set_like_tribe(trlo, malo);
                effect.set_trlo   (trlo);
            }
        }

        // Konsoleneingabe aktivieren
        if (!chat.get_type_on_last_frame()
         && ((multiplayer && ! replaying)
          || (! multiplayer && spectating))
         && hardware.key_once(useR->key_chat)) {
            chat.set_type_on();
        }
        if (chat.get_type_on_last_frame()) {
            // if we just come out of typing something into the chat,
            // have the full panel redraw. Since we don't use elders in
            // Gameplay, this ensures that the mouse doesn't drag garbage
            // on some of the buttons. It's maybe a bit kludgy this way.
            pan.set_draw_required();
        }

        // Beenden bzw. Menue?
        if (hardware.key_once(useR->key_ga_exit)
         && !chat.get_type_on_last_frame()
         && !window_gameplay) {
            window_gameplay = new Api::WindowGameplay(&replay,
                               Network::get_started() ? &level : 0);
            Api::Manager::add_focus(window_gameplay);
            return;
        }
    }
    // Ende von deaktivierter Konsole






    // Grafikeffekte muessen noch vor dem Update berechnet werden,
    // aber geschwindigkeitsmaessig unabhaengig von den cs.update
    // Trotzdem "cs.update" uebergeben, damit zu alte Effektspeicherungen
    // wieder geloescht werden koennen.
    effect.calc(cs.update);





    // Player may issue skills and do related things
    if (!replaying && !chat.get_type_on()) calc_active();

    // Jetzt die Schleife für ein Update, also eine Gameplay-Zeiteinheit
    // Dies prueft nicht die lokalen Ticks, sondern richtet sich nur nach
    // den vom Timer gezaehlten Ticks!
    if (!pan.pause.get_on()) {
        if (pan.speed_turbo.get_on()) {
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_fast)
             for (unsigned i = 0; i < turbo_times_faster_than_fast; ++i) {
                update();
            }
        }
        else if (pan.speed_fast.get_on()) {
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_fast) {
                update();
            }
        }
        else if (pan.speed_slow.get_on()) {
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_slow) {
                update();
            }
        }
        else if (Help::timer_ticks >= timer_tick_last_update
                                    + timer_ticks_for_update_normal) {
            update();
        }
    }
    // Im Netzwerk die Zeit anhand der Server-Informationen synchronisieren
    if (pan.get_gapamode() == GM_PLAY_MULTI) {
        unsigned u = Network::get_updates();
        if (u != 0) {
            u += Network::get_ping_millis()
               * Help::timer_ticks_per_second / 1000 / 2 // /2: ping=Roundtrip
               / timer_ticks_for_update_normal;          // u = Ziel-Update
            unsigned t = timer_ticks_for_update_normal;
            if      (u > cs.update + 10) t -= 2;
            else if (u > cs.update     ) t -= 1;
            else if (u < cs.update - 10) t += 1;
            else if (u < cs.update     ) t += 2;
            timer_ticks_for_update_client = t;
        }
        // In jedem Fall das Update normal rechnen, wie oben ohne Netz
        if (Help::timer_ticks >= timer_tick_last_update
                               + timer_ticks_for_update_client) update();
    }

}
// Ende von main_loop_calc()



// ############################################################################
// ############################################################################
// ############################################################################



void Gameplay::load_state(const GameState& state)
{
    if (state) {
        cs = state;
        // Replay nicht laden, sondern das derzeitige ab dort abspielen
        replaying = cs.update < replay.get_max_updates();
        // Panel aktualisieren
        if (trlo) pan.set_like_tribe(trlo, malo);
        // Sauberputzen
        effect.delete_after(cs.update);
        for (HatchIt i = hatches.begin(); i != hatches.end(); ++i)
         i->animate(effect, cs.update);
    }
}



void Gameplay::write_outcome_to_console()
{
    typedef Api::WindowGameplay::SortableTribe SoTr;

    // Sort the results
    std::vector <SoTr> sortvec;
    for (Tribe::CIt i = cs.tribes.begin(); i != cs.tribes.end(); ++i)
     sortvec.push_back(SoTr(&*i == trlo && ! spectating, &*i, i->lix_saved));
    std::sort(sortvec.begin(), sortvec.end());

    // Format the results
    std::ostringstream str;
    str << Language::net_game_end_result;
    std::vector <SoTr> ::const_iterator itr = sortvec.begin();
    while (itr != sortvec.end()) {
        str << " " << itr->tr->get_name() << ": " << itr->score;
        ++itr;
        str << (itr == sortvec.end() ? "." : " --");
    }
    Console::push_back(str.str());
}
