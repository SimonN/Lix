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

// Statische Konstanten initialisieren
const int Gameplay::block_s = 14; // Blocker-Abstand zur Seite
const int Gameplay::block_u = 16; // nach oben
const int Gameplay::block_d =  8; // nach unten

// continue for 10 minutes in verifier mode before the gameplay aborts
const int      Gameplay::updates_before_run_forever   = 15*60*10;
const unsigned Gameplay::updates_to_skip_singleplayer = 30;
const unsigned Gameplay::seconds_skipped_on_speed_slow_right = 10;



// Standardkonstruktor
Gameplay::Gameplay(
    Gameplay::VerifyMode vrfm,
    Replay* rep
) :
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
    verify_mode            (vrfm),
    filename               (determine_filename(rep)),
    level                  (determine_level()),
    trlo                   (0),
    malo                   (0),
    local_ticks            (0),
    timer_tick_last_update (Help::timer_ticks),
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

    if (verify_mode == INTERACTIVE_MODE) {
        prepare_panel();
        Api::Manager::add_elder(&pan);
        Api::Manager::add_elder(&chat);
        // don't add the panel as an elder in noninteractive mode,
        // because we'd get a crash on scancode_to_string with uninitialized
        // keyboard during calcing the panel.
    }
}



Gameplay::~Gameplay()
{
    cs.trap  .clear();
    cs.fling .clear();
    cs.trampoline.clear();
    cs.tribes.clear();

    effect.clear_all_lists();
}



const Filename& Gameplay::determine_filename(Replay* rep)
{
    if (rep && verify_mode == VERIFY_MODE) {
        // always use the contained level
        return rep->get_level_filename();
    }
    else if (rep && verify_mode == INTERACTIVE_MODE) {
        // try the contained level, only then try the pointed-to level
        Level l(useR->replay_last_level);
        if (l.get_good()) return useR->replay_last_level;
        else return rep->get_level_filename();
        // we assume that at least one of these two possibilities has a
        // good level. If neither does, the replay browser should have stopped
        // us from entering Gameplay in the first place.
    }
    else if (Network::get_started()) {
        return gloB->empty_filename;
    }
    // Normaler Einzelspieler
    else {
        return useR->single_last_level;
    }
}



const Level Gameplay::determine_level() {
    if (Network::get_started()) {
        return Network::get_level();
    }
    // in determine_filename, we've set this to useR->replay_last_level
    // or to the level filename contained in there, based on which level was OK
    else return Level(filename);
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
        cs.trap.push_back(Triggerable(map, i->ob, i->x, i->y));
        cs.trap.back().draw_lookup(cs.lookup);
        break;
     case Object::FLING:
        cs.fling.push_back(Triggerable(map, i->ob, i->x, i->y));
        cs.fling.back().draw_lookup(cs.lookup);
        break;
     case Object::TRAMPOLINE:
        cs.trampoline.push_back(Triggerable(map, i->ob, i->x, i->y));
        cs.trampoline.back().draw_lookup(cs.lookup);
        break;
     case Object::DECO:
     case Object::WATER:
     case Object::ONEWAY:
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
        // t.update_hatch = - lv.spawnint;
        t.initial         = lv.initial;
        t.lix_hatch       = lv.initial;
        t.required        = lv.required;
        t.spawnint_slow   = lv.spawnint_slow;
        t.spawnint_fast   = lv.spawnint_fast;
        t.spawnint        = lv.spawnint_slow;

        t.skills          = lv.skills; // copies the entire map

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
    // wir nicht alleine sind: In die Mitte aller eigenen Klappen, oder
    // auf die Antipode des Torus oder sowas, falls das mehr Sinn macht.
    // Also, these hatches will be marked to blink with the player's icon.
    if (multiplayer || ! lv.start_manual) {
        int h = trlo->hatch_next;
        std::vector <GameHatch*> ownhatches;
        do {
            // in multiplayer, have the hatches blink
            if (multiplayer) hatches[h].set_style(trlo->style);

            ownhatches.push_back(&hatches[h]);
            // the next hatch shall be analyzed
            h += cs.tribes.size();
            h %= hatches.size();
        } while (h != trlo->hatch_next);
        determine_screen_start_from_hatches(ownhatches);
    }
    else {
        map.set_screen_x(lv.start_x);
        map.set_screen_y(lv.start_y);
    }

    // a hack from 2015-07
    // This doesn't write to the file.
    replay.fix_legacy_replays_according_to_current_state(cs, lv.legacy_ac_vec);

    // faster start in singleplayer, with or without tutorials
    if (! multiplayer || replaying) {
        cs.update = updates_to_skip_singleplayer;
        if (replaying) replay.increase_early_data_to_update(cs.update + 1);
    }

    // now the initial state of the game is set. Save this, so the reload
    // button can fetch it back later. Also, to determine the used time,
    // we will compare with this state's updates, i.e., either 0 or 30.
    state_manager.save_zero(cs);
}



void Gameplay::prepare_panel()
{
    // This function is not used when verifying from the command line.
    // There was a crash from the uninitialized keyboard key names.

    pan.set_like_tribe(trlo);

    pan.on_hint_change       = on_hint_change_callback;
    pan.on_hint_change_where = this;

    GapaMode gm = GM_NONE;
    if (multiplayer) {
        if      (replaying)  gm = GM_REPLAY_MULTI;
        else if (spectating) gm = GM_SPEC_MULTI;
        else                 gm = GM_PLAY_MULTI;
    }
    else {
        if (spectating) gm = GM_REPLAY_MULTI;
        else            gm = GM_PLAY_SINGLE;
    }

    // activate the leftmost nonzero panel
    if (gm == GM_PLAY_SINGLE || gm == GM_PLAY_MULTI || gm == GM_REPLAY_SINGLE)
        for (size_t i = 0; i < pan.skill.size(); ++i)
            if (pan.skill[i].get_number() != 0) {
                pan.set_skill_on(pan.skill[i].get_skill());
                break;
            }

    pan.set_gapamode_and_hints(gm, level.get_hints().size());
}



// ############################################################################
// ############################################################################
// ############################################################################



int Gameplay::distance_x_to_hatches(
    int x, const std::vector <GameHatch*>& vec
) {
    typedef std::vector <GameHatch*> ::const_iterator Chit;
    int ret = 0;
    for (Chit itr = vec.begin(); itr != vec.end(); ++itr) {
        ret += std::abs(map.distance_x(x,
            (**itr).get_x() + (**itr).get_object()->get_trigger_x()) );
    }
    return ret;
}



int Gameplay::distance_y_to_hatches(
    int y, const std::vector <GameHatch*>& vec
) {
    typedef std::vector <GameHatch*> ::const_iterator Chit;
    int ret = 0;
    for (Chit itr = vec.begin(); itr != vec.end(); ++itr) {
        ret += std::abs(map.distance_y(y,
            (**itr).get_y() + (**itr).get_object()->get_trigger_y()) );
    }
    return ret;
}



void Gameplay::determine_screen_start_from_hatches(
    const std::vector <GameHatch*>& vec
) {
    if (vec.empty()) return;
    // first, calculate the average without torus
    typedef std::vector <GameHatch*> ::const_iterator Chit;
    int sum_x = 0;
    int sum_y = 0;
    for (Chit itr = vec.begin(); itr != vec.end(); ++itr) {
        sum_x += (**itr).get_x() + (**itr).get_object()->get_trigger_x();
        sum_y += (**itr).get_y() + (**itr).get_object()->get_trigger_y();
    }
    sum_x /= vec.size();
    sum_y /= vec.size();

    int best_x = sum_x;
    int best_y = sum_y;

    // If we have n hatches with n > 1, and there are torus directions,
    // repeatedly add 1/n of the screen size and see whether the new point is
    // better than all before. For checking goodness of a point, we test
    // independently for x and y distance using the 1-norm. This should
    // produce nice results, since we start with the average of all coordinates
    // and merely shift that around because of the torus.
    if (vec.size() > 1 && map.get_torus_x()) {
        int dist = 0;
        for (int i = 0; i < (int) vec.size(); ++i) {
            int plus_x  = map.get_xl() * i / vec.size();
            int newdist = distance_x_to_hatches(sum_x + plus_x, vec);
            if (newdist < dist || i == 0) {
                dist   = newdist;
                best_x = sum_x + i;
            }
        }
    }
    if (vec.size() > 1 && map.get_torus_y()) {
        // here, we do the same stuff as above, but using the already computed
        // best_x instead of the original value.
        int dist = 0;
        for (int i = 0; i < (int) vec.size(); ++i) {
            int plus_y  = map.get_yl() * i / vec.size();
            int newdist = distance_y_to_hatches(sum_y + plus_y, vec);
            if (newdist < dist || i == 0) {
                dist   = newdist;
                best_y = sum_y + i;
            }
        }
    }

    // Now we have a point (best_x, best_y) on which we want to center the
    // screen initially.
    map.set_screen_center(best_x, best_y);
}



void Gameplay::on_hint_change_callback(void* v, const int hint_cur)
{
    Gameplay& g = *static_cast <Gameplay*> (v);
    const std::vector <std::string>& hint_vec = g.level.get_hints();
    if (hint_vec.empty()) g.chat.set_hint("");
    else                  g.chat.set_hint(hint_vec[hint_cur]);
}



Result Gameplay::get_result()
{
    // a Result will save the spent updates, not the current updates.
    // The main menu will convert the spent updates into seconds again.
    return Result(level.built, trlo->lix_saved, trlo->skills_used,
        trlo->update_saved > 0
        ? trlo->update_saved - state_manager.get_zero().update
        : cs.update - state_manager.get_zero().update
    );
}



void Gameplay::save_result()
{
    if (!trlo || !malo) return;

    // Nur Einzelspielermodus speichern, und keine abgespielten Replays
    // anderer Leute als Erfolg vermerken. Man kann natuerlich schummeln
    // und kurz vor Replay-Ende eingreifen - aber schummeln kann man ohnehin
    // schon durch Bearbeiten der User-Datei.
    // Wandernde soll man nicht aufhalten. :-)
    if (! multiplayer && malo->name == gloB->user_name
                      && trlo->lix_saved >= level.required
    ) {
        useR->set_level_result_carefully(filename,get_result(),level.required);
        useR->save();
    }
}



bool Gameplay::will_run_forever()
{
    return cs.update > replay.get_max_updates() + updates_before_run_forever;
}
