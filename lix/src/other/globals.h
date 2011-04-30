/*
 * other/globals.h
 *
 * Konstanten, Optionen und andere globale Variablen, die fuer mehr als ein
 * Programmmodul von Wichtigkeit sind
 *
 * void initialize()
 *
 *   Wird aufgerunfen von Funktionen, die schon sehr fruehzeitig gesetzte
 *   globale Konstanten, etwa die sprachunabhaengigen Strings, benoetigen.
 *
 */

#pragma once

#include <string>

// At the end of this file, a pointer to a Globals called gloB
// and a pointer to the User called useR will be declared externally.
// These pointer will get new'd by Globals::initialize().
// These are the only variables throughout the program
// that are written with a capital at the end.

class Globals {

public:

    static void initialize();
    static void deinitialize();

    void load();
    void save();

    // Konstanten
    // version_min legt die aelteste Version fest, mit der wir kommunizieren
    // duerfen. Der Client prueft die Server-Version und der Server die
    // Client-Version. Wenn einem der jeweils andere zu alt ist, wird
    // die Verbindung abgebrochen.
    const unsigned long version;
    const unsigned long version_min;

    const int   updates_per_second;
    const int   skill_max;

    const int   panel_gameplay_yl;
    const int   panel_editor_yl;

    const int   scroll_torus_max;

    std::string user_name;
    bool        user_name_ask;

    int         screen_resolution_x;
    int         screen_resolution_y;
    int         screen_windowed_x;
    int         screen_windowed_y;
    bool        screen_vsync;
    bool        screen_fullscreen_now;

    bool        sound_load_driver;

    std::string ip_last_used;
    std::string ip_central_server;
    unsigned    server_port;

    const std::string

        // Diverses
        empty_string,
        net_ip_localhost,

        // Dateien laden
        ext_level,
        ext_level_orig,
        ext_replay,
        ext_user,
        mask_anything,
        mask_ext_level,
        mask_ext_level_orig,
        mask_ext_replay,
        mask_ext_user,
        mask_ext_bmp,
        mask_ext_tga,
        mask_ext_pcx,

        // Unterteilung der Bilddateien nach Prae-Endung
        pre_ext_null,
        pre_ext_internal,
        pre_ext_steel,
        pre_ext_deco,
        pre_ext_hatch,
        pre_ext_goal,
        pre_ext_trap,
        pre_ext_fire,
        pre_ext_water,
        pre_ext_oneway_left,
        pre_ext_oneway_right,

        // Wichtige Verzeichnisse
        dir_levels,
        dir_levels_single,
        dir_levels_network,
        dir_replay,
        dir_data,
        dir_data_bitmap,
        dir_data_sound,
        dir_data_user,
        dir_bitmap,
        dir_bitmap_orig,
        dir_bitmap_orig_l1,
        dir_bitmap_orig_l2,

        // Wichtige Dateien
        file_config,
        file_log,
        file_level_network,

        file_bitmap_api_number,
        file_bitmap_checkbox,
        file_bitmap_debris,
        file_bitmap_edit_flip,
        file_bitmap_edit_hatch,
        file_bitmap_edit_panel,
        file_bitmap_edit_big,
        file_bitmap_explosion,
        file_bitmap_fuse_flame,
        file_bitmap_game_arrow,
        file_bitmap_game_icon,
        file_bitmap_game_nuke,
        file_bitmap_game_panel,
        file_bitmap_game_panel_2,
        file_bitmap_game_pause,
        file_bitmap_game_replay,
        file_bitmap_lix,
        file_bitmap_lix_recol,
        file_bitmap_lobby_spec,
        file_bitmap_mouse,
        file_bitmap_preview_icon,

        file_bitmap_font_big,
        file_bitmap_font_nar,
        file_bitmap_font_med,
        file_bitmap_font_sml,

        // Config-Schluessel
        config_user_name,
        config_user_name_ask,

        config_screen_resolution_x,
        config_screen_resolution_y,
        config_screen_windowed_x,
        config_screen_windowed_y,
        config_screen_vsync,

        config_sound_load_driver,

        config_ip_last_used,
        config_ip_central_server,
        config_server_port,

        // Leveldatei-Schluessel
        level_built,
        level_author,
        level_name_german,
        level_name_english,
        level_size_x,
        level_size_y,
        level_start_x,
        level_start_y,
        level_torus_x,
        level_torus_y,
        level_seconds,
        level_initial,
        level_initial_legacy,
        level_required,
        level_rate,
        level_count_neutrals_only,
        level_transfer_skills,

        // Replaydatei-Schluessel
        replay_version_min,
        replay_built_required,
        replay_level_filename,
        replay_friend,
        replay_player,
        replay_permu,
        replay_update,
        replay_rate,
        replay_skill,
        replay_aim,
        replay_nuke,
        replay_assign,
        replay_assign_legacy,

        // Userdatei-Schluessel
        user_language,
        user_option_group,

        user_mouse_speed,
        user_scroll_speed_edge,
        user_scroll_speed_click,
        user_scroll_edge,
        user_scroll_right,
        user_scroll_middle,
        user_scroll_torus_x,
        user_scroll_torus_y,
        user_multiple_builders,

        user_screen_scaling,
        user_screen_border_colored,
        user_screen_windowed,
        user_arrows_replay,
        user_arrows_network,
        user_debris_amount,
        user_debris_type,

        user_sound_volume,

        user_single_last_dir,
        user_single_last_file,
        user_network_last_dir,
        user_network_last_file,
        user_replay_last_dir,
        user_replay_last_file,

        user_editor_last_dir_terrain,
        user_editor_last_dir_steel,
        user_editor_last_dir_goal,
        user_editor_last_dir_hatch,
        user_editor_last_dir_deco,
        user_editor_last_dir_hazard,

        user_key_force_left,
        user_key_force_right,
        user_key_rate_minus,
        user_key_rate_plus,
        user_key_pause,
        user_key_speed_slow,
        user_key_speed_fast,
        user_key_speed_turbo,
        user_key_restart,
        user_key_state_load,
        user_key_state_save,
        user_key_zoom,
        user_key_nuke,
        user_key_chat,

        user_key_ed_left,
        user_key_ed_right,
        user_key_ed_up,
        user_key_ed_down,
        user_key_ed_copy,
        user_key_ed_delete,
        user_key_ed_grid,
        user_key_ed_sel_all,
        user_key_ed_sel_frame,
        user_key_ed_sel_add,
        user_key_ed_foreground,
        user_key_ed_background,
        user_key_ed_mirror,
        user_key_ed_rotate,
        user_key_ed_dark,
        user_key_ed_noow,
        user_key_ed_zoom,
        user_key_ed_help,
        user_key_ed_menu_size,
        user_key_ed_menu_vars,
        user_key_ed_menu_skills,
        user_key_ed_add_terrain,
        user_key_ed_add_steel,
        user_key_ed_add_hatch,
        user_key_ed_add_goal,
        user_key_ed_add_deco,
        user_key_ed_add_hazard;

private:

    Globals();
    inline ~Globals() {}

    void operator = (const Globals&);

};
// end class gloB

extern Globals* gloB;
