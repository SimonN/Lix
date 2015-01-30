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
#include "../other/user.h" // returning game results

class Gameplay {

public:

    static const int block_s; // blocker distance to either side (incl. miners)
    static const int block_u; // blocker distance to above
    static const int block_d; // blocker distance to below

    static const int distance_safe_fall;
    static const int distance_float;

    static const int      updates_before_run_forever;
    static const unsigned updates_to_skip_singleplayer;

    enum VerifyMode {
        INTERACTIVE_MODE, // play or watch a replay from the normal game
        VERIFY_MODE       // auto-abort sometime after replay, update very fast
    };

private:

    const unsigned turbo_times_faster_than_fast;
    const unsigned timer_ticks_for_update_fast;
    const unsigned timer_ticks_for_update_normal;
    const unsigned timer_ticks_for_update_slow;

    const int mouse_cursor_offset;
    const int mouse_max_lix_distance_x;
    const int mouse_max_lix_distance_u;
    const int mouse_max_lix_distance_d;

    bool             exit;
    const VerifyMode verify_mode;
    const Filename   filename;
    const Level      level;

    Tribe*         trlo; // Abkuerzung fuer cs.tribes[tribe_local]
    Tribe::Master* malo; // The master of cs.tribes[tribe_local] that
                          // has player_local as its player number

    unsigned long local_ticks;
    unsigned long timer_tick_last_update;
    unsigned long timer_tick_last_F1;
    unsigned long timer_tick_last_F2;
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

    std::vector <std::vector <EdGraphic> >              special;
    std::vector         <Goal>                          goal;
    std::vector         <GameHatch>                     hatches;
    typedef std::vector <EdGraphic>  ::iterator         IacIt;
    typedef std::vector <EdGraphic>  ::reverse_iterator IacRIt;
    typedef std::vector <GameHatch>  ::iterator         HatchIt;
    typedef std::vector <Triggerable>::iterator         TrigIt;

    ////////////////
    // Funktionen //
    ////////////////

    Replay::Data new_replay_data(); // Replay::Data() mit guten Standardinfos

    const Filename& determine_filename(Replay*);
    const Level     determine_level   ();

    void prepare_players(Replay*);
    void prepare_level  ();
    void prepare_panel  ();
    void save_result    ();

    int  distance_x_to_hatches(int, const std::vector <GameHatch*>&);
    int  distance_y_to_hatches(int, const std::vector <GameHatch*>&);
    void determine_screen_start_from_hatches(const std::vector <GameHatch*>&);

    // Callback for hints changes
    static void on_hint_change_callback(void*, const int);

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
    void score_lix_for_tribe (Lixxie&, const UpdateArgs&, Tribe& tribe);
    void make_knockback_explosion(const unsigned long, const Tribe&,
                                  const unsigned,      const int, const int);
    void finally_fling       (Lixxie&);

public:

    // Der Konstruktor klaut sich seine paar Infos zusammen
    // aus Network und gloB - Spielerzahl, -daten und den Level.
    // Einzig ein Replay kann optional uebergeben werden.
    Gameplay(VerifyMode, Replay* = 0);
    ~Gameplay();

    inline bool            get_exit    () { return exit;             }
    inline const Filename& get_filename() { return filename;         }
    inline Replay&         get_replay  () { return replay;           }
    inline int             get_tribes  () { return cs.tribes.size(); }

    inline const std::string& get_chat_type() { return chat.get_text();     }
    inline       bool         get_chat_on  () { return chat.get_type_on();  }
    inline void  set_chat_type(const std::string& s) { chat.set_text(s);    }
    inline void  set_chat_on  (bool b = true)        { chat.set_type_on(b); }

    Result get_result();
    bool   will_run_forever();

    void undraw();
    void calc();
    void draw();

};
