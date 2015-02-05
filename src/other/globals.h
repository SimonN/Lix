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

#include "./file/filename.h"

// At the end of this file, a pointer to a Globals called gloB
// and a pointer to the User called useR will be declared externally.
// This pointer will get new'd by Globals::initialize().

class Globals {

public:

    static void initialize();
    static void deinitialize();

    void load();
    void save();

    // version is the current version. version_min is the oldest version
    // featuring the same game mechanics as the current one, so it's the
    // oldest version that we can netplay with.
    // If the version is stable, playing on the central server is allowed
    const unsigned long version;
    const unsigned long version_min;
    const bool          version_stable;

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
    std::string screen_modestr_now;

    bool        sound_load_driver;

    int         replay_auto_max;
    bool        replay_auto_single;
    bool        replay_auto_multi;
    int         replay_auto_next_s;
    int         replay_auto_next_m;

    std::string ip_last_used;
    std::string ip_central_server;
    unsigned    server_port;

    const std::string

        // Diverses
        empty_string,
        net_ip_localhost,
        error_wrong_working_dir,

        // Dateien laden
        ext_level,
        ext_level_orig,
        ext_level_lemmini,
        ext_replay,
        ext_user,
        ext_object_definitions,
        mask_anything,
        mask_ext_replay,
        mask_ext_user,

        file_level_dir_order,
        file_level_dir_english,
        file_level_dir_german,

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

        // Config-Schluessel
        config_user_name,
        config_user_name_ask,

        config_screen_resolution_x,
        config_screen_resolution_y,
        config_screen_windowed_x,
        config_screen_windowed_y,
        config_screen_vsync,

        config_sound_load_driver,

        config_replay_auto_max,
        config_replay_auto_single,
        config_replay_auto_multi,
        config_replay_auto_next_s,
        config_replay_auto_next_m,

        config_ip_last_used,
        config_ip_central_server,
        config_server_port,

        // Leveldatei-Schluessel
        level_built,
        level_author,
        level_name_german,
        level_name_english,
        level_tutorial_german,
        level_tutorial_english,
        level_hint_german,
        level_hint_english,
        level_size_x,
        level_size_y,
        level_torus_x,
        level_torus_y,
        level_start_x,
        level_start_y,
        level_bg_red,
        level_bg_green,
        level_bg_blue,
        level_seconds,
        level_initial,
        level_initial_legacy,
        level_required,
        level_spawnint_slow,
        level_spawnint_fast,
        level_rate,
        level_nuke_forbidden,
        level_nuke_skill,
        level_count_neutrals_only,
        level_transfer_skills,

        // Special level objects (hatches, exits, ...) definitions file
        objdef_type,
        objdef_ta_absolute_x,
        objdef_ta_absolute_y,
        objdef_ta_from_center_x,
        objdef_ta_from_center_y,
        objdef_ta_from_bottom_y,
        objdef_ta_xl,
        objdef_ta_yl,
        objdef_hatch_opening_frame,
        objdef_fling_nonpermanent,
        objdef_fling_ignore_orient,
        objdef_fling_x,
        objdef_fling_y,

        objdef_type_terrain,
        objdef_type_steel,
        objdef_type_deco,
        objdef_type_hatch,
        objdef_type_goal,
        objdef_type_trap,
        objdef_type_water,
        objdef_type_fire,
        objdef_type_oneway_left,
        objdef_type_oneway_right,
        objdef_type_fling,
        objdef_type_trampoline,

        // Replaydatei-Schluessel
        replay_version_min,
        replay_built_required,
        replay_level_filename,
        replay_friend,
        replay_player,
        replay_permu,
        replay_update,
        replay_spawnint,
        replay_skill,
        replay_aim,
        replay_nuke,
        replay_assign_any,
        replay_assign_left,
        replay_assign_right,

        // Userdatei-Schluessel
        user_language,
        user_option_group,

        user_mouse_speed,
        user_mouse_acceleration,
        user_scroll_speed_edge,
        user_scroll_speed_click,
        user_scroll_edge,
        user_scroll_right,
        user_scroll_middle,
        user_scroll_torus_x,
        user_scroll_torus_y,
        user_replay_cancel,
        user_replay_cancel_at,
        user_multiple_builders,
        user_batter_priority,
        user_prioinv_middle,
        user_prioinv_right,

        user_screen_scaling,
        user_screen_border_colored,
        user_screen_windowed,
        user_arrows_replay,
        user_arrows_network,
        user_gameplay_help,
        user_debris_amount,
        user_debris_type,
        user_gui_color_red,
        user_gui_color_green,
        user_gui_color_blue,

        user_sound_volume,

        user_editor_hex_level_size,
        user_editor_grid_selected,
        user_editor_grid_custom,

        user_single_last_level,
        user_network_last_level,
        user_replay_last_level,
        user_network_last_style,

        user_editor_last_dir_terrain,
        user_editor_last_dir_steel,
        user_editor_last_dir_goal,
        user_editor_last_dir_hatch,
        user_editor_last_dir_deco,
        user_editor_last_dir_hazard,

        user_key_force_left,
        user_key_force_right,
        user_key_scroll,
        user_key_priority,
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
        user_key_spec_tribe,
        user_key_chat,
        user_key_ga_exit,

        user_key_me_okay,
        user_key_me_edit,
        user_key_me_export,
        user_key_me_delete,
        user_key_me_up_dir,
        user_key_me_up_1,
        user_key_me_up_5,
        user_key_me_down_1,
        user_key_me_down_5,
        user_key_me_exit,
        user_key_me_main_single,
        user_key_me_main_network,
        user_key_me_main_replay,
        user_key_me_main_options,

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
        user_key_ed_add_hazard,
        user_key_ed_exit;

    const Filename

        empty_filename,

        // Wichtige Verzeichnisse
        dir_levels,
        dir_levels_single,
        dir_levels_network,
        dir_replay,
        dir_replay_auto,
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

        file_translations,
        file_translations_dump,

        file_bitmap_api_number,
        file_bitmap_checkbox,
        file_bitmap_debris,
        file_bitmap_edit_flip,
        file_bitmap_edit_hatch,
        file_bitmap_edit_panel,
        file_bitmap_explosion,
        file_bitmap_fuse_flame,
        file_bitmap_game_arrow,
        file_bitmap_game_icon,
        file_bitmap_game_nuke,
        file_bitmap_game_panel,
        file_bitmap_game_panel_2,
        file_bitmap_game_panel_hints,
        file_bitmap_game_spi_fix,
        file_bitmap_game_pause,
        file_bitmap_game_replay,
        file_bitmap_lix,
        file_bitmap_lix_recol,
        file_bitmap_lobby_spec,
        file_bitmap_menu_background,
        file_bitmap_menu_checkmark,
        file_bitmap_mouse,
        file_bitmap_preview_icon,

        file_bitmap_font_big,
        file_bitmap_font_nar,
        file_bitmap_font_med,
        file_bitmap_font_sml,

        file_replay_auto_single,
        file_replay_auto_multi;

private:

    Globals();
    inline ~Globals() {}

    void operator = (const Globals&);

};
// end class gloB

extern Globals* gloB;
