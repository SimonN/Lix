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
            restart_level();
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



void Gameplay::check_skill_buttons() {
    // This starts at the next button and breaks immediately if one is
    // pressed. This enables a hotkey to cycle through its skills.
    // This next if makes that an overloaded hotkey will select the skill
    // that's more to the left always, unless a skill with the same
    // hotkey is already selected. If the if wasn't there, sometimes the
    // first hit of a new skillkey would select a different than the
    // leftmost occurence, based on where the skills are.

    // find current button in case it has been clicked exactly this calc
    GameplayPanel::SkBIt cur_but = pan.skill.begin();
    while (cur_but != pan.skill.end() && ! cur_but->get_on())
        ++cur_but;
    int pan_vec_id = cur_but - pan.skill.begin();
    if (cur_but == pan.skill.end() || ! cur_but->get_clicked())
        pan_vec_id = pan.skill.size() - 1;
    // we don't need cur_but from here on anymore, only the panel ID

    // Scan for hotkey presses of empty/nonpresent skills
    // if this is still false later, iterate over nonpresent skills
    // and play the empty-skill sound if a key for them was pressed
    bool some_panel_action = false;

    for (size_t j = 0; j < pan.skill.size(); ++j) {
        size_t i = (pan_vec_id + j + 1) % pan.skill.size();
        if (pan.skill[i].get_number() != 0
         && pan.skill[i].get_clicked()
         && ! pan.skill[i].get_on())
        {
            // This will be done during the next physics update, but
            // we'll do it now, to make the button seem more responsive
            pan.set_skill_on(pan.skill[i].get_skill());
            Sound::play_loud(Sound::PANEL);
            // Don't check any more buttons, see comment before the loop.
            some_panel_action = true;
            break;
        }
    }

    if (! some_panel_action) {
        // check for empty clicked panel icons
        for (size_t i = 0; i < pan.skill.size(); ++i)
         if (pan.skill[i].get_clicked()
         &&  pan.skill[i].get_skill() != LixEn::NOTHING) {
            if (! pan.skill[i].get_on()) {
                if (hardware.get_ml()) {
                    Sound::play_loud(Sound::PANEL_EMPTY);
                }
                // else play no sound -- we're holding the mouse button
                // over a wrong skill, that's not notify-necessary, but
                // leave open the possibility to play a sound later
            }
            // we've clicked on an activated skill, that's all good,
            // never play a sound even if a hotkey was used
            else some_panel_action = true;
        }
        // check for hotkeys of present/nonpresent skills:
        // if the hotkey is of an available skill, we wouldn't have ended
        // up in this if (! some_panel_action)
        if (! some_panel_action)
         for (size_t i = 0; i < LixEn::AC_MAX; ++i) {
            const int key = useR->key_skill[i];
            if (key != 0 && hardware.key_once(key))
             Sound::play_loud(Sound::PANEL_EMPTY);
        }
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

    if (map.get_scrolling_now())
        mouse_cursor.set_x_frame(3);

    // encourage user to scroll with right-click if he's not doing it
    if ((map.get_scrollable_left() || map.get_scrollable_right())
     && (hardware.get_mx() == 0 || hardware.get_mx() == LEMSCR_X - 1))
        pan.suggest_tooltip_scrolling();
    if ((map.get_scrollable_up() || map.get_scrollable_down())
     && (hardware.get_my() == 0 || hardware.get_my() == LEMSCR_Y - 1))
        pan.suggest_tooltip_scrolling();

    // ec kurz fuer exit_count. game ends when this is 0.
    unsigned ec = 0;
    for (Tribe::CIt t = cs.tribes.begin(); t != cs.tribes.end(); ++t)
        ec += t->get_still_playing();
    for (TrigIt i = cs.trap.begin(); i != cs.trap.end(); ++i)
        ec += i->get_x_frame();
    if (pan.get_speed() == GameplayPanel::SPEED_TURBO
     || pan.get_speed() == GameplayPanel::SPEED_FAST)
         ec += effect.get_effects_without_debris();
    else ec += effect.get_effects();
    // Check also for !window_gameplay, as in a networked game,
    // the action keeps running in the background
    if (ec == 0 && !window_gameplay) {
        chat.set_type_off();
        if (verify_mode == VERIFY_MODE) {
            // Verifier object should ask this Gameplay about stats,
            // then delete this Gameplay object
            exit = true;
        }
        else if (multiplayer && spectating && ! replaying) {
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
            window_gameplay = new Api::WindowGameplay(&replay, &level, trlo,
                // Eye candy when not saving anything, won't be saved anyway
                (trlo->lix_saved > 0
                ? trlo->update_saved - state_manager.get_zero().update
                : cs.update          - state_manager.get_zero().update));
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
        if (hardware.get_ml() && abort_replay == false) {
            abort_replay = true;
            if (pan.state_save .is_mouse_here()
             || pan.state_load .is_mouse_here()
             || pan.pause      .is_mouse_here()
             || pan.zoom       .is_mouse_here()
             || pan.speed_back .is_mouse_here()
             || pan.speed_ahead.is_mouse_here()
             || pan.speed_fast .is_mouse_here()
             || pan.restart    .is_mouse_here()
             || pan.nuke_single.is_mouse_here()) abort_replay = false;
        }
        // Cancel replay when you're selecting a skill with mouse or hotkey,
        // even if that has no effect on the replay contents. Pepole are used
        // to cancelling replays with skill switches.
        for (size_t i = 0; i < pan.skill.size(); ++i)
            if (pan.skill[i].get_clicked())
                abort_replay = true;
        // we ignore clicks on the nuke here, as we do with non-gamestate-
        // -altering buttons like pause or fast forward. We'll come back
        // to it right here:
        if    (pan.get_nuke_doubleclicked()
            || pan.spawnint_slow.get_clicked()
            || pan.spawnint_cur .get_clicked()
        ) {
            abort_replay = true;
        }

        if (abort_replay) {
            replaying = false;
            replay.erase_data_after_update(cs.update);
        }
        // abort fast-forward earlier than this
        if (useR->replay_cancel) {
            const int rcat = useR->replay_cancel_at;
            if (pan.get_speed() == GameplayPanel::SPEED_FAST
                && csu == max - rcat
            ) {
                pan.set_speed(GameplayPanel::SPEED_NORMAL);
            }
            if (pan.get_speed() == GameplayPanel::SPEED_TURBO
                && csu >= max - rcat
                && csu <  max - rcat + fff
            ) {
                pan.set_speed(GameplayPanel::SPEED_NORMAL);
            }
        }
    }

    // Konsole deaktiviert, normale Buttons ansehen und,
    // wenn kein Replay stattfindet, auch den aktiven Main-Loop abarbeiten
    // console_tt_on_last_frame is a bit kludgy, but it works nicely.
    // Gameplay is not an elder, so this will get calced every frame here,
    // and without said variable, the console would be opened again upon
    // sending text with enter or the menu would open on cancelling typing.
    if (!chat.get_type_on()) {

        check_skill_buttons();

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
                if (pan.get_speed() != GameplayPanel::SPEED_PAUSE)
                    pan.set_speed(GameplayPanel::SPEED_NORMAL);
                load_state(sta);
            }
        }

        // Pause
        if (pan.pause.get_clicked()) {
            if (pan.get_speed() == GameplayPanel::SPEED_PAUSE)
                 pan.set_speed(GameplayPanel::SPEED_NORMAL);
            else pan.set_speed(GameplayPanel::SPEED_PAUSE);
        }
        // Zoom
        else if (pan.zoom.get_clicked()) {
            pan.zoom.set_on(!pan.zoom.get_on());
            map.set_zoom(pan.zoom.get_on());
        }
        // Speed. On some executions, update
        else if (pan.speed_back.get_execute_left()) {
            go_back_updates(1);
            pan.set_speed(GameplayPanel::SPEED_PAUSE);
        }
        else if (pan.speed_back.get_execute_right()) {
            // go back 1 second
            go_back_updates(Help::timer_ticks_per_second
                          / timer_ticks_for_update_normal);
            pan.set_speed(GameplayPanel::SPEED_PAUSE);
        }
        else if (pan.speed_ahead.get_execute_left()) {
            pan.set_speed(GameplayPanel::SPEED_PAUSE);
            // do a single logic update even though the game is paused
            update();
        }
        else if (pan.speed_ahead.get_execute_right()) {
            for (size_t i = 0; i < Help::timer_ticks_per_second
                                 * seconds_skipped_on_speed_slow_right
                                 / timer_ticks_for_update_normal; ++i)
                update();
        }
        else if (pan.speed_fast.get_execute_left()) {
            if (pan.speed_fast.get_on())
                 pan.set_speed(GameplayPanel::SPEED_NORMAL);
            else pan.set_speed(GameplayPanel::SPEED_FAST);
        }
        else if (pan.speed_fast.get_execute_right()) {
            if (pan.speed_fast.get_on())
                 pan.set_speed(GameplayPanel::SPEED_NORMAL);
            else pan.set_speed(GameplayPanel::SPEED_TURBO);
        }
        // Neustart
        else if (pan.restart.get_clicked()) {
            restart_level();
        }

        // Switch the spectator's panel to a different tribe's skillset
        else if (pan.spec_tribe.get_clicked()) {
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
                pan.set_like_tribe(trlo);
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
            window_gameplay = new Api::WindowGameplay(&replay, &level,
                                                      cs.tribes.size());
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
    if (verify_mode == INTERACTIVE_MODE && !replaying && !chat.get_type_on()) {
        calc_active();
    }

    // Jetzt die Schleife für ein Update, also eine Gameplay-Zeiteinheit
    // Dies prueft nicht die lokalen Ticks, sondern richtet sich nur nach
    // den vom Timer gezaehlten Ticks!
    if (!pan.pause.get_on()) {

        if (verify_mode == VERIFY_MODE) {
            // update as fast as possilbe
            update();
        }
        else switch (pan.get_speed()) {
        case GameplayPanel::SPEED_TURBO:
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_fast)
                for (unsigned i = 0; i < turbo_times_faster_than_fast; ++i)
                    update();
            break;
        case GameplayPanel::SPEED_FAST:
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_fast)
                update();
            break;
        default:
            if (Help::timer_ticks >= timer_tick_last_update
                                   + timer_ticks_for_update_normal)
                update();
            break;
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



void Gameplay::restart_level()
{
    save_result();
    replay.erase_early_singleplayer_nukes();
    load_state(state_manager.get_zero());
    pan.set_speed(GameplayPanel::SPEED_NORMAL);
    pan.set_hint_cur(0); // remove shown hints (displays tutorial if present)
}



void Gameplay::load_state(const GameState& state)
{
    if (state) {
        cs = state;
        // Replay nicht laden, sondern das derzeitige ab dort abspielen
        replaying = cs.update < replay.get_max_updates();
        // Panel aktualisieren
        if (trlo) pan.set_like_tribe(trlo);
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
