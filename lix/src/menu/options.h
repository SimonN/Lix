/*
 * menu/options.h
 *
 * Das Optionsmenue, in dem viele Einstellungen der Konfigurationsdatei
 * bequem geaendert werden koennen.
 *
 * void reset_elements()
 *
 *   Ruft nochamls die globalen und die User-Variablen ab (aber ohne sie zuvor
 *   aus den entsprechenden Dateien neu zu laden) und setzt alle API-Elemente
 *   auf diese Werte.
 *
 *   Nach Aenderung des Spielernamens kann man beispielsweise die Spieler-
 *   datei von Hand laden und anschliessend reset_elements() aufrufen.
 *
 */

#pragma once

#include "../api/button/b_enum.h"
#include "../api/button/b_key.h"
#include "../api/button/b_text.h"
#include "../api/label.h"
#include "../api/number.h"
#include "../api/texttype.h"
#include "../api/window.h"

#include "../lix/lix_enum.h"

namespace Api {
class OptionMenu : public Window {

public:

                OptionMenu();
    virtual    ~OptionMenu();

    inline bool get_exit() { return exit; }

protected:

    virtual void calc_self();

private:

    static const unsigned check_x;  // X-Position von Checkoxen einfach 20
    static const unsigned check_nx; // X-Pos. fuer Checkbox-Beschriftungen, 60
    static const unsigned other_x;  // LEMSCR_X - 160, da Buttons 140 breit
    static const unsigned other_nx; // Irgendwas in der Mitte, dass es passt

    static const unsigned button_xl; // 140
    static const unsigned key_xl;
    static const unsigned frame_y;
    static const unsigned frame_yl;

    static const unsigned key_b1;    // 1st row's buttons
    static const unsigned key_t1;    // 1st row's text
    static const unsigned key_b2;
    static const unsigned key_t2;
    static const unsigned key_b3;
    static const unsigned key_t3;

    bool       exit;
    int        hotkey_add_counter;

    enum OptionGroup {
        GROUP_GENERAL,
        GROUP_CONTROLS,
        GROUP_HOTKEYS,
        GROUP_EDITOR,
        GROUP_GRAPHICS,
        GROUP_SOUND,
        GROUP_MAX
    };

    TextButton button_okay;
    TextButton button_cancel;

    std::vector <TextButton> button_group;

    // Nimmt einen Zeiger zu jedem erstellten Button auf. Alle Zeiger zu
    // generellen Einstellungen landen im Vektor pointers[GROUP_GENERAL] etc.
    // Man kann so rasch ueber alle notwendigen Buttons iterieren.
    typedef std::vector <Element*> PtrVec;
    std::vector <PtrVec> pointers;

    Texttype   user_name;
    Checkbox   user_name_ask;
    TextButton language;
    int        language_nr;
    Label      desc_user_name,
               desc_user_name_ask,
               desc_language;

    Number     mouse_speed;
    Number     scroll_speed_edge;
    Number     scroll_speed_click;
    Checkbox   scroll_edge;
    Checkbox   scroll_right;
    Checkbox   scroll_middle;
    EnumButton scroll_torus_x;
    EnumButton scroll_torus_y;
    Checkbox   multiple_builders;
    Label      desc_mouse_speed,
               desc_scroll_speed_edge,
               desc_scroll_speed_click,
               desc_scroll_edge,
               desc_scroll_right,
               desc_scroll_middle,
               desc_scroll_torus_x,
               desc_scroll_torus_y,
               desc_multiple_builders;

    KeyButton  key_force_left;
    KeyButton  key_force_right;
    KeyButton  key_rate_minus;
    KeyButton  key_rate_plus;
    KeyButton  key_pause;
    KeyButton  key_speed_slow;
    KeyButton  key_speed_fast;
    KeyButton  key_speed_turbo;
    KeyButton  key_restart;
    KeyButton  key_state_load;
    KeyButton  key_state_save;
    KeyButton  key_zoom;
    KeyButton  key_chat;
    KeyButton  key_nuke;
    std::vector <KeyButton> key_skill;
    std::vector <Label>     desc_key_skill;
    Label      desc_key_force_left,
               desc_key_force_right,
               desc_key_rate_minus,
               desc_key_rate_plus,
               desc_key_pause,
               desc_key_speed_slow,
               desc_key_speed_fast,
               desc_key_speed_turbo,
               desc_key_restart,
               desc_key_state_load,
               desc_key_state_save,
               desc_key_zoom,
               desc_key_chat,
               desc_key_nuke,
               desc_key_info_1,
               desc_key_info_2,
               desc_key_info_3;

    KeyButton  ed_left;
    KeyButton  ed_right;
    KeyButton  ed_up;
    KeyButton  ed_down;
    KeyButton  ed_copy;
    KeyButton  ed_delete;
    KeyButton  ed_grid;
    KeyButton  ed_sel_all;
    KeyButton  ed_sel_frame;
    KeyButton  ed_sel_add;
    KeyButton  ed_background;
    KeyButton  ed_foreground;
    KeyButton  ed_mirror;
    KeyButton  ed_rotate;
    KeyButton  ed_dark;
    KeyButton  ed_noow;
    KeyButton  ed_zoom;
    KeyButton  ed_help;
    KeyButton  ed_menu_size;
    KeyButton  ed_menu_vars;
    KeyButton  ed_menu_skills;
    KeyButton  ed_add_terrain;
    KeyButton  ed_add_steel;
    KeyButton  ed_add_hatch;
    KeyButton  ed_add_goal;
    KeyButton  ed_add_deco;
    KeyButton  ed_add_hazard;
    Label      desc_ed_left,
               desc_ed_right,
               desc_ed_up,
               desc_ed_down,
               desc_ed_copy,
               desc_ed_delete,
               desc_ed_grid,
               desc_ed_sel_all,
               desc_ed_sel_frame,
               desc_ed_sel_add,
               desc_ed_background,
               desc_ed_foreground,
               desc_ed_mirror,
               desc_ed_rotate,
               desc_ed_dark,
               desc_ed_noow,
               desc_ed_zoom,
               desc_ed_help,
               desc_ed_menu_size,
               desc_ed_menu_vars,
               desc_ed_menu_skills,
               desc_ed_add_terrain,
               desc_ed_add_steel,
               desc_ed_add_hatch,
               desc_ed_add_goal,
               desc_ed_add_deco,
               desc_ed_add_hazard;

    Texttype   screen_resolution_x;
    Texttype   screen_resolution_y;
    Texttype   screen_windowed_x;
    Texttype   screen_windowed_y;
    Checkbox   screen_windowed;
    EnumButton screen_scaling;
    Checkbox   screen_border_colored;
    Checkbox   screen_vsync;
    Checkbox   arrows_replay;
    Checkbox   arrows_network;
    EnumButton debris_amount;
    EnumButton debris_type;
    Label      desc_screen_resolution,
               desc_screen_windowed_res,
               desc_screen_windowed,
               desc_screen_scaling,
               desc_screen_border_colored,
               desc_screen_vsync,
               desc_arrows_replay,
               desc_arrows_network,
               desc_debris_amount,
               desc_debris_type,
               desc_option_gfx_zero,
               desc_option_gfx_info;

    Checkbox   sound_load_driver;
    Number     sound_volume;
    Label      desc_sound_load_driver,
               desc_sound_volume,
               desc_sound_info;

    void add_skill_to_hotkey_dialogue(const LixEn::Ac&);
    void reset_elements();
    void show_group (const OptionGroup);

    // Kopierverbot
    OptionMenu      (const OptionMenu&);
    void operator = (const OptionMenu&);

}; // Ende Klasse OptionMenu
}  // Ende Namensraum Api
