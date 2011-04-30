/*
 * other/globals.cpp
 *
 */

#include "myalleg.h"

#include <fstream>

#include "io.h"

#define CONCAT(a, b, c, d) (a ## b ## c ## d)

Globals* gloB(0);



Globals::Globals()
:
    version              (CONCAT(2011, 03, 05, 00)),
    version_min          (CONCAT(2011, 03, 05, 00)),

    updates_per_second   (15),
    skill_max            (12),
    panel_gameplay_yl    (80),
    panel_editor_yl      (60),
    scroll_torus_max     (2),

    user_name            (""),
    user_name_ask        (false),

    screen_resolution_x  (   0),
    screen_resolution_y  (   0),
    screen_windowed_x    ( 640),
    screen_windowed_y    ( 480),
    screen_vsync         (false),
    screen_fullscreen_now(false),

    sound_load_driver    (true),

    ip_last_used         ("127.0.0.1"),
    ip_central_server    ("flux.camanis.net"),
    server_port          (22934),

    // Constant strings
    // Diverses
    empty_string                 (""),
    net_ip_localhost             ("127.0.0.1"),

    // Dateien laden
    ext_level                    (".txt"),
    ext_level_orig               (".lvl"),
    ext_replay                   (".txt"),
    ext_user                     (".txt"),
    mask_anything                ("*.*"),
    mask_ext_level               ("*.txt"),
    mask_ext_level_orig          ("*.lvl"),
    mask_ext_replay              ("*.txt"),
    mask_ext_user                ("*.txt"),
    mask_ext_bmp                 ("*.bmp"),
    mask_ext_tga                 ("*.tga"),
    mask_ext_pcx                 ("*.pcx"),

    // Unterteilung der Bilddateien nach Prae-Endung
    pre_ext_null                 (":"),
    pre_ext_internal             ("I"),
    pre_ext_steel                ("S"),
    pre_ext_deco                 ("D"),
    pre_ext_hatch                ("H"),
    pre_ext_goal                 ("G"),
    pre_ext_trap                 ("T"),
    pre_ext_fire                 ("F"),
    pre_ext_water                ("W"),
    pre_ext_oneway_left          ("L"),
    pre_ext_oneway_right         ("R"),

    // Wichtige Verzeichnisse
    dir_levels                   ("./levels/"),
    dir_levels_single            ("./levels/"),
    dir_levels_network           ("./levels/network/"),
    dir_replay                   ("./replay/"),
    dir_data                     ("./data/"),
    dir_data_bitmap              ("./data/bitmap/"),
    dir_data_sound               ("./data/sound/"),
    dir_data_user                ("./data/user/"),
    dir_bitmap                   ("./bitmap/"),
    dir_bitmap_orig              ("./bitmap/orig/"),
    dir_bitmap_orig_l1           ("./bitmap/orig/L1/"),
    dir_bitmap_orig_l2           ("./bitmap/orig/L2/"),

    // Wichtige Dateien
    file_config                  ("./data/config.txt"),
    file_log                     ("./data/log.txt"),
    file_level_network           ("./data/netlevel.txt"),

    file_bitmap_api_number       ("./data/bitmap/api_numb.I"),
    file_bitmap_checkbox         ("./data/bitmap/checkbox.I"),
    file_bitmap_debris           ("./data/bitmap/debris.I"),
    file_bitmap_edit_flip        ("./data/bitmap/edit_flp.I"),
    file_bitmap_edit_hatch       ("./data/bitmap/edit_hat.I"),
    file_bitmap_edit_panel       ("./data/bitmap/edit_pan.I"),
    file_bitmap_edit_big         ("./data/bitmap/edit_big.I"),
    file_bitmap_explosion        ("./data/bitmap/explode.I"),
    file_bitmap_fuse_flame       ("./data/bitmap/fuse_fla.I"),
    file_bitmap_game_arrow       ("./data/bitmap/game_arr.I"),
    file_bitmap_game_icon        ("./data/bitmap/game_ico.I"),
    file_bitmap_game_nuke        ("./data/bitmap/game_nuk.I"),
    file_bitmap_game_panel       ("./data/bitmap/game_pan.I"),
    file_bitmap_game_panel_2     ("./data/bitmap/game_pa2.I"),
    file_bitmap_game_pause       ("./data/bitmap/game_pau.I"),
    file_bitmap_game_replay      ("./data/bitmap/game_rep.I"),
    file_bitmap_lix              ("./data/bitmap/lix.I"),
    file_bitmap_lix_recol        ("./data/bitmap/lixrecol.I"),
    file_bitmap_lobby_spec       ("./data/bitmap/lobby_sp.I"),
    file_bitmap_mouse            ("./data/bitmap/mouse.I"),
    file_bitmap_preview_icon     ("./data/bitmap/prev_ico.I"),

    file_bitmap_font_big         ("./data/bitmap/font_big.I.bmp"),
    file_bitmap_font_nar         ("./data/bitmap/font_nar.I.bmp"),
    file_bitmap_font_med         ("./data/bitmap/font_med.I.bmp"),
    file_bitmap_font_sml         ("./data/bitmap/font_sml.I.bmp"),

    // Config-Schluessel
    config_user_name             ("USER_NAME"),
    config_user_name_ask         ("USER_NAME_ASK"),

    config_screen_resolution_x   ("SCREEN_RESOLUTION_X"),
    config_screen_resolution_y   ("SCREEN_RESOLUTION_Y"),
    config_screen_windowed_x     ("SCREEN_WINDOWED_X"),
    config_screen_windowed_y     ("SCREEN_WINDOWED_Y"),
    config_screen_vsync          ("SCREEN_VSYNC"),

    config_sound_load_driver     ("SOUND_LOAD_DRIVER"),

    config_ip_last_used          ("IP_LAST_USED"),
    config_ip_central_server     ("IP_CENTRAL_SERVER"),
    config_server_port           ("SERVER_PORT"),

    // Leveldatei-Schluessel
    level_built                  ("BUILT"),
    level_author                 ("AUTHOR"),
    level_name_german            ("GERMAN"),
    level_name_english           ("ENGLISH"),
    level_size_x                 ("SIZE_X"),
    level_size_y                 ("SIZE_Y"),
    level_start_x                ("START_X"),
    level_start_y                ("START_Y"),
    level_torus_x                ("TORUS_X"),
    level_torus_y                ("TORUS_Y"),
    level_seconds                ("SECONDS"),
    level_initial                ("INITIAL"),
    level_initial_legacy         ("LEMMINGS"), // backwards compatibility
    level_required               ("REQUIRED"),
    level_rate                   ("RATE"),
    level_count_neutrals_only    ("COUNT_NEUTRALS_ONLY"),
    level_transfer_skills        ("TRANSFER_SKILLS"),

    // Replaydatei-Schluessel
    replay_version_min           ("VERSION_REQUIRED"),
    replay_built_required        ("BUILT_REQUIRED"),
    replay_level_filename        ("FILENAME"),
    replay_friend                ("FRIEND"),
    replay_player                ("PLAYER"),
    replay_permu                 ("PERMUTATION"),
    replay_update                ("UPDATE"),
    replay_rate                  ("RATE"),
    replay_skill                 ("SKILL"),
    replay_aim                   ("AIM"),
    replay_nuke                  ("NUKE"),
    replay_assign                ("ASSIGN"),
    replay_assign_legacy         ("LEMMING"), // backwards compatibility

    // Userdatei-Schluessel
    user_language                ("LANGUAGE"),
    user_option_group            ("OPTION_GROUP"),

    user_mouse_speed             ("MOUSE_SPEED"),
    user_scroll_speed_edge       ("SCROLL_SPEED_EDGE"),
    user_scroll_speed_click      ("SCROLL_SPEED_CLICK"),
    user_scroll_edge             ("SCROLL_EDGE"),
    user_scroll_right            ("SCROLL_RIGHT"),
    user_scroll_middle           ("SCROLL_MIDDLE"),
    user_scroll_torus_x          ("SCROLL_TORUS_X"),
    user_scroll_torus_y          ("SCROLL_TORUS_Y"),
    user_multiple_builders       ("MULTIPLE_BUILDERS"),

    user_screen_scaling          ("SCREEN_SCALING"),
    user_screen_border_colored   ("SCREEN_BORDER_COLORED"),
    user_screen_windowed         ("SCREEN_WINDOWED"),
    user_arrows_replay           ("ARROWS_REPLAY"),
    user_arrows_network          ("ARROWS_NETWORK"),
    user_debris_amount           ("DEBRIS_AMOUNT"),
    user_debris_type             ("DEBRIS_TYPE"),

    user_sound_volume            ("SOUND_VOLUME"),

    user_single_last_dir         ("SINGLE_LAST_DIR"),
    user_single_last_file        ("SINGLE_LAST_FILE"),
    user_network_last_dir        ("NETWORK_LAST_DIR"),
    user_network_last_file       ("NETWORK_LAST_FILE"),
    user_replay_last_dir         ("REPLAY_LAST_DIR"),
    user_replay_last_file        ("REPLAY_LAST_FILE"),

    user_editor_last_dir_terrain ("EDITOR_LAST_DIR_TERRAIN"),
    user_editor_last_dir_steel   ("EDITOR_LAST_DIR_STEEL"),
    user_editor_last_dir_goal    ("EDITOR_LAST_DIR_GOAL"),
    user_editor_last_dir_hatch   ("EDITOR_LAST_DIR_HATCH"),
    user_editor_last_dir_deco    ("EDITOR_LAST_DIR_DECO"),
    user_editor_last_dir_hazard  ("EDITOR_LAST_DIR_HAZARD"),

    user_key_force_left          ("KEY_FORCE_LEFT"),
    user_key_force_right         ("KEY_FORCE_RIGHT"),
    user_key_rate_minus          ("KEY_RATE_MINUS"),
    user_key_rate_plus           ("KEY_RATE_PLUS"),
    user_key_pause               ("KEY_PAUSE"),
    user_key_speed_slow          ("KEY_SPEED_SLOW"),
    user_key_speed_fast          ("KEY_SPEED_FAST"),
    user_key_speed_turbo         ("KEY_SPEED_TURBO"),
    user_key_restart             ("KEY_RESTART"),
    user_key_state_load          ("KEY_STATE_LOAD"),
    user_key_state_save          ("KEY_STATE_SAVE"),
    user_key_zoom                ("KEY_ZOOM"),
    user_key_nuke                ("KEY_NUKE"),
    user_key_chat                ("KEY_CHAT"),

    user_key_ed_left             ("KEY_EDITOR_LEFT"),
    user_key_ed_right            ("KEY_EDITOR_RIGHT"),
    user_key_ed_up               ("KEY_EDITOR_UP"),
    user_key_ed_down             ("KEY_EDITOR_DOWN"),
    user_key_ed_copy             ("KEY_EDITOR_COPY"),
    user_key_ed_delete           ("KEY_EDITOR_DELETE"),
    user_key_ed_grid             ("KEY_EDITOR_GRID"),
    user_key_ed_sel_all          ("KEY_EDITOR_SELECT_ALL"),
    user_key_ed_sel_frame        ("KEY_EDITOR_SELECT_FRAME"),
    user_key_ed_sel_add          ("KEY_EDITOR_SELECT_ADD"),
    user_key_ed_foreground       ("KEY_EDITOR_FOREGROUND"),
    user_key_ed_background       ("KEY_EDITOR_BACKGROUND"),
    user_key_ed_mirror           ("KEY_EDITOR_MIRROR"),
    user_key_ed_rotate           ("KEY_EDITOR_ROTATE"),
    user_key_ed_dark             ("KEY_EDITOR_DARK"),
    user_key_ed_noow             ("KEY_EDITOR_NO_OVERWRITE"),
    user_key_ed_zoom             ("KEY_EDITOR_ZOOM"),
    user_key_ed_help             ("KEY_EDITOR_HELP"),
    user_key_ed_menu_size        ("KEY_EDITOR_MENU_SIZE"),
    user_key_ed_menu_vars        ("KEY_EDITOR_MENU_GENERAL"),
    user_key_ed_menu_skills      ("KEY_EDITOR_MENU_SKILLS"),
    user_key_ed_add_terrain      ("KEY_EDITOR_ADD_TERRAIN"),
    user_key_ed_add_steel        ("KEY_EDITOR_ADD_STEEL"),
    user_key_ed_add_hatch        ("KEY_EDITOR_ADD_HATCH"),
    user_key_ed_add_goal         ("KEY_EDITOR_ADD_GOAL"),
    user_key_ed_add_deco         ("KEY_EDITOR_ADD_DECO"),
    user_key_ed_add_hazard       ("KEY_EDITOR_ADD_HAZARD")
{
}



void Globals::initialize()
{
    if (!gloB) gloB = new Globals();
}



void Globals::deinitialize()
{
    if (gloB) {
        delete gloB;
        gloB = 0;
    }
}



// Konfigurationsdatei korrekt verarbeiten
void Globals::load()
{
    // Jetzt aber die Datei laden.
    std::vector <IO::Line> lines;
    IO::fill_vector_from_file(lines, gloB->file_config);

    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) switch(i->type) {
    // String setzen
    case '$':
        if      (i->text1 == config_user_name          ) user_name           = i->text2;
        else if (i->text1 == config_ip_last_used       ) ip_last_used        = i->text2;
        else if (i->text1 == config_ip_central_server  ) ip_central_server   = i->text2;
        break;

    // Ganzzahlwert setzen
    case '#':
        if      (i->text1 == config_user_name_ask      ) user_name_ask       = i->nr1;

        else if (i->text1 == config_screen_resolution_x) screen_resolution_x = i->nr1;
        else if (i->text1 == config_screen_resolution_y) screen_resolution_y = i->nr1;
        else if (i->text1 == config_screen_windowed_x  ) screen_windowed_x   = i->nr1;
        else if (i->text1 == config_screen_windowed_y  ) screen_windowed_y   = i->nr1;
        else if (i->text1 == config_screen_vsync       ) screen_vsync        = i->nr1;

        else if (i->text1 == config_sound_load_driver  ) sound_load_driver   = i->nr1;

        else if (i->text1 == config_server_port        ) server_port         = i->nr1;
        break;
    }
}



void Globals::save()
{
    std::ofstream file(file_config.c_str());

    file
     << IO::LineDollar(config_user_name,               user_name)
     << IO::LineHash  (config_user_name_ask,           user_name_ask)
     << std::endl

     << IO::LineHash  (config_screen_resolution_x,     screen_resolution_x)
     << IO::LineHash  (config_screen_resolution_y,     screen_resolution_y)
     << IO::LineHash  (config_screen_windowed_x,       screen_windowed_x)
     << IO::LineHash  (config_screen_windowed_y,       screen_windowed_y)
     << IO::LineHash  (config_screen_vsync,            screen_vsync)
     << std::endl

     << IO::LineHash  (config_sound_load_driver,       sound_load_driver)
     << std::endl

     << IO::LineDollar(config_ip_last_used,            ip_last_used)
     << IO::LineDollar(config_ip_central_server,       ip_central_server)
     << IO::LineHash  (config_server_port,             server_port);

    file.close();
}
// Ende save_config_file
