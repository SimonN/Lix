/*
 * gameplay/gameplay.cpp
 *
 */

#include "gameplay.h"

#include "../api/manager.h"
#include "../graphic/gra_lib.h"
#include "../other/globals.h"
#include "../other/hardware.h"
#include "../other/help.h"
#include "../other/user.h"

// Statische Konstanten initialisieren
const int Gameplay::block_s = 14; // Blocker-Abstand zur Seite
const int Gameplay::block_m = 20; // zur Seite, wenn der andere ein Miner ist
const int Gameplay::block_u = 16; // nach oben
const int Gameplay::block_d =  8; // nach unten

// Standardkonstruktor
Gameplay::Gameplay(Replay* rep)
:
    // Constants
    turbo_times_faster_than_fast (8),
    timer_ticks_for_update_fast  (Help::timer_ticks_per_second/60),
    timer_ticks_for_update_normal(Help::timer_ticks_per_second
                                  / gloB->updates_per_second),
    timer_ticks_for_update_slow  (Help::timer_ticks_per_second/4*3),

    mouse_cursor_offset     (15),
    mouse_max_lix_distance_x(16),
    mouse_max_lix_distance_u(26),
    mouse_max_lix_distance_d(12),

    // Variables
    exit                   (false),
    filename               (determine_filename(rep)),
    level                  (Network::get_started()
                            ? Network::get_level() : filename),
    trlo                   (0),
    malo                   (0),
    local_ticks            (0),
    update_last_exiter     (0),
    timer_tick_last_update (Help::timer_ticks),
    timer_tick_last_F1     (0),
    timer_tick_last_F2     (0),
    timer_tick_last_F12    (0),
    timer_ticks_for_update_client(timer_ticks_for_update_normal),

    bg_color               (makecol(level.bg_red,
                                    level.bg_green,
                                    level.bg_blue)),
    map                    (level.size_x, level.size_y, LEMSCR_X,
                            LEMSCR_Y - gloB->panel_gameplay_yl),
    effect                 (map),

    mouse_cursor(GraLib::get(gloB->file_bitmap_mouse),
                                Api::Manager::get_torbit()),
    replay_sign (GraLib::get(gloB->file_bitmap_game_replay),
                                Api::Manager::get_torbit()),
    spectating        (false),
    multiplayer       (false),

    replaying         (rep? true : false),
    replay_recalc_need(false),
    replay_recalc_from(0),

    window_gameplay   (0),
    special           (Object::MAX)
{
    if (rep) replay = *rep;
    else     replay.set_level_filename(filename);

    prepare_players(rep);
    prepare_level();
    prepare_panel();
}



Gameplay::~Gameplay()
{
    cs.trap  .clear();
    cs.tribes.clear();

    effect.clear_all_lists();
}



const Filename& Gameplay::determine_filename(Replay* rep)
{
    if (rep) {
        return rep->get_level_filename();
    }
    else if (Network::get_started()) {
        return gloB->empty_filename;
    }
    // Normaler Einzelspieler
    else {
        return useR->single_last_level;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



void Gameplay::prepare_players(Replay* rep)
{
    if (!rep) {
        if (!Network::get_started()) {
            replay.add_player(0, LixEn::GARDEN, gloB->user_name);
            replay.set_player_local(0);
        }
        else {
            // The permutation is ignored here, it will be taken into account
            // when hatches and goals are distributed among the players.
            for (Network::PlDatCIt pitr = Network::get_player_data().begin();
             pitr != Network::get_player_data().end(); ++pitr) {
                if (!pitr->spec) {
                    replay.add_player(pitr->number,
                     static_cast <LixEn::Style> (pitr->style), pitr->name);
                }
            }
            const int count = replay.get_players().size();
            if (Network::get_spec() && count > 0)
                 replay.set_player_local(-1);
            else replay.set_player_local(Network::get_number());

            replay.set_permu(Network::get_permu());
        }
    }

    // Done setting up the replay. Now set up the State cs.
    // Team formation is by LixEn::Style. Determine how many teams we have.
    std::vector <bool> style_active(LixEn::STYLE_MAX, false);
    for (Replay::PlayerCIt itr = replay.get_players().begin();
     itr != replay.get_players().end(); ++itr)
     if (style_active[itr->style] == false) {
        style_active[itr->style] = true;
        cs.tribes.push_back(Tribe());
        cs.tribes.back().style = itr->style;
    }
    replay.shorten_permu_to(cs.tribes.size());

    // Assign players to teams.
    for (Replay::PlayerCIt itr = replay.get_players().begin();
     itr != replay.get_players().end(); ++itr) {
        Tribe::It titr = cs.tribes.begin();
        while (titr != cs.tribes.end() && titr->style != itr->style)
         ++titr;
        if (titr != cs.tribes.end()) {
            titr->masters.push_back(Tribe::Master());
            titr->masters.back().number = itr->number;
            titr->masters.back().name   = itr->name;
            if (itr->number == replay.get_player_local()) {
                trlo = &*titr;
            }
        }
    }

    // Find local master. We cannot do this in the last for/if, because
    // there are still masters being added and STL containers may reallocate.
    if (trlo) {
        for (std::list <Tribe::Master> ::iterator
            mitr = trlo->masters.begin(); mitr != trlo->masters.end(); ++mitr)
            if (mitr->number == replay.get_player_local()) {
                malo = &*mitr;
                break;
            }
    }
    else if (cs.tribes.size() > 0) {
        trlo = & cs.tribes[ ::rand() % cs.tribes.size() ];
    }

    multiplayer = (cs.tribes.size() > 1);
    if (multiplayer && rep) malo = 0;
    spectating = ! malo;

    effect.set_trlo(trlo);
}



void Gameplay::prepare_level()
{
    // Die Karte erschaffen
    map    .set_torus_xy(level.torus_x, level.torus_y);
    cs.land.set_torus_xy(level.torus_x, level.torus_y);
    cs.land.resize      (level.size_x,  level.size_y);
    level.draw_to       (cs.land, &cs.lookup);

    Lixxie::set_static_maps(cs.land, cs.lookup, map);
    Lixxie::set_effect_manager(effect);
    map.clear_to_color(color[COL_BLACK]);
    // save_bmp("test_land.bmp",  cs.land   .get_al_bitmap(), 0);
    // save_bmp("test_steel.bmp", steel_mask.get_al_bitmap(), 0);

    cs.clock         = level.seconds * gloB->updates_per_second;
    cs.clock_running = (cs.tribes.size() == 1);
    // set_show_clock() wird stets aktuell in Gameplay::draw() aufgerufen

    // Interaktive Objekte aufstellen
    // In umgekehrter Reihenfolge erzeugen
    const Level& lv = level;

    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (Level::PosIt i = lv.pos[type].begin(); i != lv.pos[type].end();++i)
     switch (type) {
     case Object::HATCH:
         hatches.push_back(GameHatch(map, i->ob, i->x, i->y));
         hatches.back().set_rotation(i->rot);
         hatches.back().draw_lookup(cs.lookup);
         break;
     case Object::GOAL:
        goal.push_back(Goal(map, i->ob, i->x, i->y));
        goal.back().draw_lookup(cs.lookup);
        break;
     case Object::TRAP:
        cs.trap.push_back(EdGraphic(map, i->ob, i->x, i->y));
        cs.trap.back().draw_lookup(cs.lookup);
        break;
     case Object::FLING:
        cs.fling.push_back(EdGraphic(map, i->ob, i->x, i->y));
        cs.fling.back().draw_lookup(cs.lookup);
        break;
     case Object::DECO:
     case Object::WATER:
     case Object::ONEWAY:
     case Object::TRAMPOLINE:
        special[type].push_back(EdGraphic(map, i->ob, i->x, i->y));
        special[type].back().draw_lookup(cs.lookup);
        break;
     default:
        break;
    }

    // Nummer an Klappen ggf. nach unten korrigieren, nie nach oben
    // Irgendwas % 1 ist immer 0, also hoeren wir spaetestens bei einer
    // Klappe oder einem Ausgang auf
    while (hatches.size() % cs.tribes.size() != 0
     && cs.tribes.size()  % hatches.size()   != 0) hatches.pop_back();
    while (goal.size()    % cs.tribes.size() != 0
     && cs.tribes.size()  % goal.size()      != 0) goal.pop_back();

    // Spieler initialisieren
    // p.skill kommt unten nach, wenn wir das richtige Panel gewaehlt haben
    for (size_t i = 0; i < cs.tribes.size(); ++i) {
        size_t i_permuted = replay.get_permu()[i];
        Tribe& t = cs.tribes[i];
        // Style und Name sind schon gesetzt beim Spielererschaffen.
        // Couldn't have resized the vector earlier because the map must exist.
        t.lixvec          . resize(lv.initial);
        t.hatch_next      = i_permuted % hatches.size();
        t.initial         = lv.initial;
        t.lix_hatch       = lv.initial;
        t.required        = lv.required;
        t.rate_min        = lv.rate;
        t.rate            = lv.rate;
        for (int a = 0; a < gloB->skill_max; ++a) {
            t.skill[a].ac = lv.skill[a].ac;
            t.skill[a].nr = lv.skill[a].nr;
        }

        // Ausga(e)ng(e) zuweisen - dies ist etwas geklauter Code vom
        // Klappen zuweisen, der direkt hierunter steht
        unsigned g = i_permuted % goal.size();
        do {
            goal[g].add_tribe(&t);
            g += cs.tribes.size();
            g %= goal.size();
        } while (!goal[g].has_tribe(&t));

        // Auf dem Panel unten eintragen
        pan.stats.add_tribe(t);
    }
    if (trlo) pan.stats.set_tribe_local(trlo);

    // Bildschirm-Start-Koordinaten festlegen fuer den lokalen Spieler, wenn
    // wir nicht alleine sind: In die Mitte aller eigenen Klappen
    // Debugging: Eventuell Antipode auf dem Torus nehmen, wenn die naeher
    // an allen liegt als das Ergebnis hier.
    // Also, these hatches will be marked to blink with the player's icon.
    if (multiplayer) {
        unsigned sum_h = 0;
        int sum_x = 0;
        int sum_y = 0;
        int h = trlo->hatch_next;
        do {
            hatches[h].set_style(trlo->style);
            sum_x+=hatches[h].get_x()+hatches[h].get_object()->get_trigger_x();
            sum_y+=hatches[h].get_y()+hatches[h].get_object()->get_trigger_y();
            ++sum_h;
            // Die nächste Klappe kommt an die Reihe
            h += cs.tribes.size();
            h %= hatches.size();
        } while (h != trlo->hatch_next);

        map.set_screen_x(sum_x / sum_h -  LEMSCR_X / 2);
        map.set_screen_y(sum_y / sum_h - (LEMSCR_Y-gloB->panel_gameplay_yl)/3);
    }
    else {
        // In singleplayer, start at the given coordinates.
        map.set_screen_x(lv.start_x);
        map.set_screen_y(lv.start_y);
    }

    // Faehigkeit auswaehlen: Das muss in einer separaten Schleife geschehen,
    // weil durch Neuallokation die Adressen in der ersten sich ggf. aendern.
    for (int i = 0; i < gloB->skill_max; ++i)
     if (lv.skill[i].nr != 0) {
        for  (Tribe::It  t = cs.tribes.begin();  t != cs.tribes.end();  ++t)
         for (Tribe::MIt m = t->masters.begin(); m != t->masters.end(); ++m) {
            m->skill_sel = i;
        }
        break;
    }

    state_manager.save_zero(cs);
}



void Gameplay::prepare_panel()
{
    pan.set_like_tribe(trlo, malo);

    if (multiplayer) {
        if      (replaying)  pan.set_gapamode(GM_REPLAY_MULTI);
        else if (spectating) pan.set_gapamode(GM_SPEC_MULTI);
        else                 pan.set_gapamode(GM_PLAY_MULTI);
    }
    else {
        if (spectating) pan.set_gapamode(GM_REPLAY_MULTI);
        else            pan.set_gapamode(GM_PLAY_SINGLE);
    }
}



void Gameplay::save_result()
{
    if (!trlo || !malo) return;

    // Nur Einzelspielermodus speichern, und keine abgespielten Replays
    // anderer Leute als Erfolg vermerken. Man kann natuerlich schummeln
    // und kurz vor Replay-Ende eingreifen - aber schummeln kann man ohnehin
    // schon durch Bearbeiten der User-Datei.
    // Wandernde soll man nicht aufhalten. :-)
    if (! multiplayer
     && trlo->lix_saved >= level.required
     && malo->name      == gloB->user_name)
    {
        Result res(level.built, trlo->lix_saved,
         trlo->skills_used, update_last_exiter);
        useR->set_level_result(filename, res);
        useR->save();
    }
}
