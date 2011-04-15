/*
 * other/user.h
 *
 * Benutzerdaten, hauptsaechlich gespeicherte Levels
 *
 * struct LevelData enthaelt keinen Eintrag, der auf den Level selbst ver-
 * weist. Stattdessen wird pro Benutzer eine Hashmap benutzt, die einem
 * Dateinamen jeweils ein LevelResult-Objekt zuordnet.
 *
 * bool Result::operator < (const Result& r)
 *
 *   Liefert wahr, wenn *this ein schlechteres Ergebnis ist als r.
 *   Groessere Ergebnisse sind bessere Leistungen, oder aber built stimmt.
 *
 * void User::set_level_result(...)
 *
 *   Veraendert die Benutzerdaten des Levels, falls das LevelResult-Objekt
 *   ein besseres Ergebnis ist (Result::operator >). Besser ist das Ergebnis
 *   immer, wenn Level::built neuer ist als Result::built, bei Versions-
 *   gleichheit entscheidet die Zahl der geretteten Lemminge, bei erneuter
 *   Gleichheit die kuerzere Zeit.
 *
 */

#pragma once

#include <map>
#include <vector>

#include "date.h"

struct Result {
    Date built;
    int  lix_saved;
    int  skills_used;
    int  updates_used;

    Result ();
    Result (const Date&, const int, const int, const int);
    ~Result();

    bool    operator == (const Result&) const;
    bool    operator != (const Result&) const;
    bool    operator <  (const Result&) const;
    bool    operator >  (const Result&) const;
};



// ############################################################################
// ############################################################################
// ############################################################################



class User {

private:

    std::map <std::string, Result> result;

public:

    ////////////
    // Optionen: Sind frei setzbar, wie damals in gloB
    ////////////

    int  language;
    int  option_group;

    bool scroll_edge;
    bool scroll_right;
    bool scroll_middle;
    int  mouse_speed;
    int  scroll_speed_edge;
    int  scroll_speed_click;
    int  scroll_torus_x;
    int  scroll_torus_y;
    bool multiple_builders;

    int  screen_scaling;
    bool screen_border_colored;
    bool screen_windowed;
    bool arrows_replay;
    bool arrows_network;
    int  debris_amount;
    int  debris_type;

    int  sound_volume;

    std::vector <int> key_skill;

    int  key_force_left;
    int  key_force_right;
    int  key_rate_minus;
    int  key_rate_plus;
    int  key_pause;
    int  key_speed_slow;
    int  key_speed_fast;
    int  key_speed_turbo;
    int  key_restart;
    int  key_state_load;
    int  key_state_save;
    int  key_zoom;
    int  key_nuke;
    int  key_chat;

    int  key_ed_left;
    int  key_ed_right;
    int  key_ed_up;
    int  key_ed_down;
    int  key_ed_copy;
    int  key_ed_delete;
    int  key_ed_grid;
    int  key_ed_sel_all;
    int  key_ed_sel_frame;
    int  key_ed_sel_add;
    int  key_ed_background;
    int  key_ed_foreground;
    int  key_ed_mirror;
    int  key_ed_rotate;
    int  key_ed_dark;
    int  key_ed_noow;
    int  key_ed_zoom;
    int  key_ed_help;
    int  key_ed_menu_size;
    int  key_ed_menu_vars;
    int  key_ed_menu_skills;
    int  key_ed_add_terrain;
    int  key_ed_add_steel;
    int  key_ed_add_hatch;
    int  key_ed_add_goal;
    int  key_ed_add_deco;
    int  key_ed_add_hazard;

    std::string single_last_dir;
    std::string single_last_file;
    std::string network_last_dir;
    std::string network_last_file;
    std::string replay_last_dir;
    std::string replay_last_file;

    std::string editor_last_dir_terrain;
    std::string editor_last_dir_steel;
    std::string editor_last_dir_hatch;
    std::string editor_last_dir_goal;
    std::string editor_last_dir_deco;
    std::string editor_last_dir_hazard;

    ////////////////
    // Funktionen //
    ////////////////

    User();
    ~User();

    const Result* get_level_result(const std::string&) const;
    void          set_level_result(const std::string&, const Result&);

    static void initialize();
    static void deinitialize();

    void load();
    void save() const;

};

extern User* useR;
