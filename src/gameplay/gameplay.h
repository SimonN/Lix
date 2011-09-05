/*
 * gameplay/gameplay.h
 *
 * Gameplay ist die Klasse, die das eigentliche Spiel beschreibt. Es werden
 * einige grosse Codestuecke in separate CPP-Dateien ausgelagert.
 *
 */

#pragma once

#include "effect.h"
#include "goal.h"
#include "hatch.h"
#include "replay.h"
#include "state_ma.h"
#include "gui/chat.h"
#include "gui/panel.h"
#include "gui/win_game.h"

#include "../api/button/b_bitmap.h"
#include "../api/button/b_skill.h"

#include "../graphic/graph_bg.h"
#include "../graphic/map.h"
#include "../lix/ac.h"
#include "../level/level.h"
#include "../network/network.h"
#include "../other/console.h"

class Gameplay {

public:

    static const int block_s; // Blocker-Abstand zur Seite
    static const int block_m; // zur Seite, wenn der andere ein Miner ist
    static const int block_u; // nach oben
    static const int block_d; // nach unten

    static const int distance_safe_fall;
    static const int distance_float;

private:

    const unsigned turbo_times_faster_than_fast;
    const unsigned timer_ticks_for_update_fast;
    const unsigned timer_ticks_for_update_normal;
    const unsigned timer_ticks_for_update_slow;

    const int mouse_cursor_offset;
    const int mouse_max_lix_distance_x;
    const int mouse_max_lix_distance_u;
    const int mouse_max_lix_distance_d;

    bool           exit;
    const Filename filename;
    const Level    level;

    Tribe*         trlo; // Abkuerzung fuer cs.tribes[tribe_local]
    Tribe::Master* malo; // The master of cs.tribes[tribe_local] that
                          // has player_local as its player number

    unsigned long local_ticks;
    unsigned long update_last_exiter; // fuer Einspieler-Rekord
    unsigned long timer_tick_last_update;
    unsigned long timer_tick_last_F1;
    unsigned long timer_tick_last_F2;
    unsigned long timer_tick_last_F12;
    int           timer_ticks_for_update_client;

    int           bg_color;
    Map           map;

    GameState     cs; // current state
    StateManager  state_manager;
    EffectManager effect;

    GraphicBG     mouse_cursor;
    GraphicBG     replay_sign;

    GameplayChat  chat;

    GameplayPanel pan;

    bool              spectating;
    bool              multiplayer;

    // Alles fuers Replay
    bool              replaying;
    Replay            replay;
    bool              replay_recalc_need;
    unsigned long     replay_recalc_from;
    Api::SkillButton* old_selected_skill;

    Api::WindowGameplay* window_gameplay;

    typedef std::vector <Lixxie> ::iterator            LixIt;

    std::vector <std::vector <EdGraphic> >             special;
    std::vector         <Goal>                         goal;
    std::vector         <GameHatch>                    hatches;
    typedef std::vector <EdGraphic> ::iterator         IacIt;
    typedef std::vector <EdGraphic> ::reverse_iterator IacRIt;
    typedef std::vector <GameHatch> ::iterator         HatchIt;

    ////////////////
    // Funktionen //
    ////////////////

    Replay::Data new_replay_data(); // Replay::Data() mit guten Standardinfos

    const Filename& determine_filename(Replay* = 0);

    void prepare_players(Replay*);
    void prepare_level  ();
    void prepare_panel  ();
    void save_result    ();

    // Calculate-Subroutinen
    void calc_window(); // Fuers Unterfenster, ansonsten:
    void calc_self  (); // Wird sonst in jedem Tick ausgeführt
    void calc_active(); // "Aktiv" == kein Replay abzuspielen
    void load_state (const GameState&);
    void write_outcome_to_console();

    // Update-Subroutinen
    void update();           // Normalerweise 15x pro Sek., ruft auf:
    void update_cs_once      ();
    void update_cs_one_data  (Tribe&, Tribe::Master*, Replay::It);
    void update_lix          (Lixxie&, const UpdateArgs&);
    void update_lix_blocker  (Lixxie&);
    void update_lix_goals    (Lixxie&, const UpdateArgs&);
    void make_knockback_explosion(const unsigned long, const Tribe&,
                                  const unsigned,      const int, const int);

public:

    // Der Konstruktor klaut sich seine paar Infos zusammen
    // aus Network und gloB - Spielerzahl, -daten und den Level.
    // Einzig ein Replay kann optional uebergeben werden.
    Gameplay(Replay* = 0);
    ~Gameplay();

    inline bool            get_exit    () { return exit;             }
    inline const Filename& get_filename() { return filename;         }
    inline Replay&         get_replay  () { return replay;           }
    inline int             get_tribes  () { return cs.tribes.size(); }

    void undraw();
    void calc();
    void draw();

};
