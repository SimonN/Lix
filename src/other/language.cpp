/*
 * language.cpp
 *
 */

#include "language.h"
#include "user.h"
#include "../editor/editor.h"
#include "./file/io.h"
#include <map>
#include <algorithm>

// Alles nochmal deklarieren, weil es im Header als extern markiert ist
namespace Language {

// see comment on set_window_text() in main() in other/main.cpp
// for why we need this string constant
char const* const main_name_of_game_English = "Lix";

// Geheime Variable

// +1 because unlike other vectors, we'll have an element
// here for Language::MAX too
std::vector <std::string> language_name(MAX + 1);

namespace Option {
    enum OptionGroupNames {
        GENERAL,
        CONTROLS,
        HOTKEYS,
        EDITOR,
        MENU,
        GRAPHICS,
        SOUND,

        MAX
    };
};
std::vector <std::string> option_group(Option::MAX);

std::vector <std::string> editor_button(Editor::MAX);
std::vector <std::string>& eb = editor_button; // nur intern in language.cpp

typedef std::map<std::string, std::string*> translate_map_type;
static translate_map_type map_vars_to_translate;

// a dummy "" is returned to enable following usage:
//    std::string myVar = add_to_translation_map("myVar", myVar);
static char const* add_to_translation_map(char const *const varname, std::string &ref_var) {
    map_vars_to_translate[varname] = &ref_var;
    return "";
}

#define DECL_VARIABLE_FOR_TRANSLATION(varname) \
    std::string varname = add_to_translation_map( #varname , varname)

// For new std::string global variables in namespace Language for a new
// string that can be translated, just copy the DECL_VARIABLE_... line
// and change the macro argument to name of the new variable.
//
// For the strings stored in eb (editor_button) vector or similar,
// refer to add_all_vector_elements_to_translation_map() further below.

// Grundlegende Dinge
DECL_VARIABLE_FOR_TRANSLATION(main_name_of_the_game);
DECL_VARIABLE_FOR_TRANSLATION(main_loading_1);
DECL_VARIABLE_FOR_TRANSLATION(main_loading_2);
DECL_VARIABLE_FOR_TRANSLATION(main_version);
DECL_VARIABLE_FOR_TRANSLATION(main_website);

// Logging - mostly for developer use, so
// probably should not be exposed for translation
std::string

log_error,
log_info,
log_program_start_1,
log_program_start_2,
log_program_exit,

log_found,
log_loaded,
log_file_not_found,
log_bitmap_bad,

log_level_unknown_bitmap,
log_level_file_saved;

// Verschiedene Dialoge
DECL_VARIABLE_FOR_TRANSLATION(common_ok);
DECL_VARIABLE_FOR_TRANSLATION(common_cancel);
DECL_VARIABLE_FOR_TRANSLATION(common_yes);
DECL_VARIABLE_FOR_TRANSLATION(common_no);
DECL_VARIABLE_FOR_TRANSLATION(common_back);
DECL_VARIABLE_FOR_TRANSLATION(common_exit);
DECL_VARIABLE_FOR_TRANSLATION(common_dir_parent);
DECL_VARIABLE_FOR_TRANSLATION(common_dir_flip_page);

// Browser
DECL_VARIABLE_FOR_TRANSLATION(browser_single_title);
DECL_VARIABLE_FOR_TRANSLATION(browser_network_title);
DECL_VARIABLE_FOR_TRANSLATION(browser_replay_title);
DECL_VARIABLE_FOR_TRANSLATION(browser_play);
DECL_VARIABLE_FOR_TRANSLATION(browser_edit);
DECL_VARIABLE_FOR_TRANSLATION(browser_replay);
DECL_VARIABLE_FOR_TRANSLATION(browser_delete);
DECL_VARIABLE_FOR_TRANSLATION(browser_extract);
DECL_VARIABLE_FOR_TRANSLATION(browser_export_image);
DECL_VARIABLE_FOR_TRANSLATION(browser_export_image_done);

DECL_VARIABLE_FOR_TRANSLATION(browser_info_author);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_initgoal);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_initial);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_hatches);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_goals);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_skills);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_clock_2);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_author_none);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_clock_none);

DECL_VARIABLE_FOR_TRANSLATION(browser_info_result_saved);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_result_skills);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_result_time);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_result_old_1);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_result_old_2);

DECL_VARIABLE_FOR_TRANSLATION(browser_info_player);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_version);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_built);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_new);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_same);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_old);
DECL_VARIABLE_FOR_TRANSLATION(browser_info_holds_level);

DECL_VARIABLE_FOR_TRANSLATION(browser_box_delete_tit_rep);
DECL_VARIABLE_FOR_TRANSLATION(browser_box_delete_que_rep);
DECL_VARIABLE_FOR_TRANSLATION(browser_box_delete_tit_lev);
DECL_VARIABLE_FOR_TRANSLATION(browser_box_delete_que_lev);

// Lobby
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_title);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_title_lobby);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_title_room);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_exit);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_unstable_central);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_start_central);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_start_server);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_start_client);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_chat);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_select_level);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_ready);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_room_number);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_room_player);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_room_players);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_room_create);
DECL_VARIABLE_FOR_TRANSLATION(win_lobby_room_leave);

// Spielfenster
DECL_VARIABLE_FOR_TRANSLATION(win_game_title);
DECL_VARIABLE_FOR_TRANSLATION(win_game_result);
DECL_VARIABLE_FOR_TRANSLATION(win_game_lix_saved);
DECL_VARIABLE_FOR_TRANSLATION(win_game_lix_saved_in_time);
DECL_VARIABLE_FOR_TRANSLATION(win_game_resume);
DECL_VARIABLE_FOR_TRANSLATION(win_game_restart);
DECL_VARIABLE_FOR_TRANSLATION(win_game_save_replay);
DECL_VARIABLE_FOR_TRANSLATION(win_game_menu);
DECL_VARIABLE_FOR_TRANSLATION(win_game_comment_perfect);
DECL_VARIABLE_FOR_TRANSLATION(win_game_comment_more);
DECL_VARIABLE_FOR_TRANSLATION(win_game_comment_exactly);
DECL_VARIABLE_FOR_TRANSLATION(win_game_comment_less);
DECL_VARIABLE_FOR_TRANSLATION(win_game_comment_none);
DECL_VARIABLE_FOR_TRANSLATION(win_game_result_skills);
DECL_VARIABLE_FOR_TRANSLATION(win_game_result_time);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_first);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_second);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_middle);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_last);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_first_tie);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_last_tie);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_all_tie);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_zero);
DECL_VARIABLE_FOR_TRANSLATION(win_game_net_all_zero);
DECL_VARIABLE_FOR_TRANSLATION(win_game_replay_win_one);
DECL_VARIABLE_FOR_TRANSLATION(win_game_replay_win_team);
DECL_VARIABLE_FOR_TRANSLATION(win_game_replay_tie);
DECL_VARIABLE_FOR_TRANSLATION(win_game_overwrite_title);
DECL_VARIABLE_FOR_TRANSLATION(win_game_overwrite_question);

// Help texts inside the game
DECL_VARIABLE_FOR_TRANSLATION(gameplay_rate_minus);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_rate_plus);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_pause);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_zoom);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_left_right_click);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_speed_back);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_speed_ahead);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_speed_fast);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_state_save);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_state_load);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_restart);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_nuke);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_hint_first);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_hint_next);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_hint_prev);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_hint_off);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_spec_tribe);

DECL_VARIABLE_FOR_TRANSLATION(gameplay_rmb);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_mmb);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_scroll_right_1);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_scroll_right_2);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_force_dir_1);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_force_dir_2);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_force_dir_3);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_priority_1);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_priority_2);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_queue_builders);
DECL_VARIABLE_FOR_TRANSLATION(gameplay_queue_platformers);

// Editor allgemein
DECL_VARIABLE_FOR_TRANSLATION(editor_unsaved_title);
DECL_VARIABLE_FOR_TRANSLATION(editor_unsaved_title_new);
DECL_VARIABLE_FOR_TRANSLATION(editor_unsaved_question);
DECL_VARIABLE_FOR_TRANSLATION(editor_unsaved_question_new);
DECL_VARIABLE_FOR_TRANSLATION(editor_file_name);
DECL_VARIABLE_FOR_TRANSLATION(editor_level_name);
DECL_VARIABLE_FOR_TRANSLATION(editor_hotkey);
DECL_VARIABLE_FOR_TRANSLATION(editor_hotkeys);
DECL_VARIABLE_FOR_TRANSLATION(editor_hotkey_hold);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_at);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_hover);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_selection);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_movekeys_long);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_movekeys_short);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_movekeys_mid);
DECL_VARIABLE_FOR_TRANSLATION(editor_bar_movekeys_end);

// SaveBrowser
DECL_VARIABLE_FOR_TRANSLATION(save_browser_title);
DECL_VARIABLE_FOR_TRANSLATION(save_filename);
DECL_VARIABLE_FOR_TRANSLATION(save_box_overwrite_title);
DECL_VARIABLE_FOR_TRANSLATION(save_box_overwrite_question);

// BitmapBrowser
DECL_VARIABLE_FOR_TRANSLATION(add_terrain);
DECL_VARIABLE_FOR_TRANSLATION(add_steel);
DECL_VARIABLE_FOR_TRANSLATION(add_hatch);
DECL_VARIABLE_FOR_TRANSLATION(add_goal);
DECL_VARIABLE_FOR_TRANSLATION(add_deco);
DECL_VARIABLE_FOR_TRANSLATION(add_hazard);

// Kartengroesse
DECL_VARIABLE_FOR_TRANSLATION(win_size_title);
DECL_VARIABLE_FOR_TRANSLATION(win_size_l);
DECL_VARIABLE_FOR_TRANSLATION(win_size_r);
DECL_VARIABLE_FOR_TRANSLATION(win_size_u);
DECL_VARIABLE_FOR_TRANSLATION(win_size_d);
DECL_VARIABLE_FOR_TRANSLATION(win_size_x);
DECL_VARIABLE_FOR_TRANSLATION(win_size_y);
DECL_VARIABLE_FOR_TRANSLATION(win_size_hex);
DECL_VARIABLE_FOR_TRANSLATION(win_size_torus_x);
DECL_VARIABLE_FOR_TRANSLATION(win_size_torus_y);

// Karten-Startausschnitt
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_title);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_manual);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_x);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_y);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_r);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_g);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_b);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_jump);
DECL_VARIABLE_FOR_TRANSLATION(win_scroll_current);

// Variablenfenster
DECL_VARIABLE_FOR_TRANSLATION(win_var_title);
DECL_VARIABLE_FOR_TRANSLATION(win_var_author);
DECL_VARIABLE_FOR_TRANSLATION(win_var_name_german);
DECL_VARIABLE_FOR_TRANSLATION(win_var_name_english);
DECL_VARIABLE_FOR_TRANSLATION(win_var_initial);
DECL_VARIABLE_FOR_TRANSLATION(win_var_required);
DECL_VARIABLE_FOR_TRANSLATION(win_var_spawnint_slow);
DECL_VARIABLE_FOR_TRANSLATION(win_var_spawnint_fast);
DECL_VARIABLE_FOR_TRANSLATION(win_var_clock);

// Faehigkeits-Fenster
DECL_VARIABLE_FOR_TRANSLATION(win_skill_title);
DECL_VARIABLE_FOR_TRANSLATION(win_skill_clear);
DECL_VARIABLE_FOR_TRANSLATION(win_skill_classic_8_to);
DECL_VARIABLE_FOR_TRANSLATION(win_skill_all_to);
DECL_VARIABLE_FOR_TRANSLATION(win_skill_use_fling);

// Exporting a level into a file
DECL_VARIABLE_FOR_TRANSLATION(export_single_lix);
DECL_VARIABLE_FOR_TRANSLATION(export_single_spawnint);
DECL_VARIABLE_FOR_TRANSLATION(export_single_clock);
DECL_VARIABLE_FOR_TRANSLATION(export_multi_lix);
DECL_VARIABLE_FOR_TRANSLATION(export_multi_spawnint);
DECL_VARIABLE_FOR_TRANSLATION(export_multi_clock);

// Netzwerk-Chatnachrichten
DECL_VARIABLE_FOR_TRANSLATION(net_chat_welcome_1);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_welcome_2);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_unstable_1);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_unstable_2);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_start_server);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_start_client);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_start_cancel);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_disconnection);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_too_old);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_too_new);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_someone_old);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_someone_new);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_named_guy_old);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_version_yours);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_version_server);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_server_update);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_please_download);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_connected);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_in_room);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_in_room_2);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_we_in_lobby);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_in_room);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_in_room_2);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_in_lobby);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_out_room);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_out_room_2);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_player_out_lobby);
DECL_VARIABLE_FOR_TRANSLATION(net_chat_level_change);
DECL_VARIABLE_FOR_TRANSLATION(net_game_start);
DECL_VARIABLE_FOR_TRANSLATION(net_game_how_to_chat_1);
DECL_VARIABLE_FOR_TRANSLATION(net_game_how_to_chat_2);
DECL_VARIABLE_FOR_TRANSLATION(net_game_end);
DECL_VARIABLE_FOR_TRANSLATION(net_game_end_result);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_1);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_2);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_2_one);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_3);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_4);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_4_one);
DECL_VARIABLE_FOR_TRANSLATION(net_game_overtime_4_sec);

// Optionsfenster
DECL_VARIABLE_FOR_TRANSLATION(option_title);
DECL_VARIABLE_FOR_TRANSLATION(option_user_name);
DECL_VARIABLE_FOR_TRANSLATION(option_user_name_ask);
DECL_VARIABLE_FOR_TRANSLATION(option_language);
DECL_VARIABLE_FOR_TRANSLATION(option_replay_auto_max);
DECL_VARIABLE_FOR_TRANSLATION(option_replay_auto_single);
DECL_VARIABLE_FOR_TRANSLATION(option_replay_auto_multi);
DECL_VARIABLE_FOR_TRANSLATION(option_replay_cancel);
DECL_VARIABLE_FOR_TRANSLATION(option_replay_cancel_at);
DECL_VARIABLE_FOR_TRANSLATION(option_mouse_speed);
DECL_VARIABLE_FOR_TRANSLATION(option_scroll_speed_edge);
DECL_VARIABLE_FOR_TRANSLATION(option_scroll_speed_click);
DECL_VARIABLE_FOR_TRANSLATION(option_scroll_edge);
DECL_VARIABLE_FOR_TRANSLATION(option_scroll_right);
DECL_VARIABLE_FOR_TRANSLATION(option_scroll_middle);
DECL_VARIABLE_FOR_TRANSLATION(option_allow_builder_queuing);
DECL_VARIABLE_FOR_TRANSLATION(option_avoid_builder_queuing);
DECL_VARIABLE_FOR_TRANSLATION(option_avoid_batter_to_exploder);
DECL_VARIABLE_FOR_TRANSLATION(option_prioinv_middle);
DECL_VARIABLE_FOR_TRANSLATION(option_prioinv_right);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_resolution);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_windowed_res);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_windowed);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_scaling);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_scaling_stretch);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_scaling_aspect);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_scaling_integer);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_border_colored);
DECL_VARIABLE_FOR_TRANSLATION(option_screen_vsync);
DECL_VARIABLE_FOR_TRANSLATION(option_arrows_replay);
DECL_VARIABLE_FOR_TRANSLATION(option_arrows_network);
DECL_VARIABLE_FOR_TRANSLATION(option_game_show_tooltips);
DECL_VARIABLE_FOR_TRANSLATION(option_game_show_skill_keys);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_amount);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_amount_none);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_amount_own);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_amount_all);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_type);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_type_stars);
DECL_VARIABLE_FOR_TRANSLATION(option_debris_type_pixels);
DECL_VARIABLE_FOR_TRANSLATION(option_gui_color_red);
DECL_VARIABLE_FOR_TRANSLATION(option_gui_color_green);
DECL_VARIABLE_FOR_TRANSLATION(option_gui_color_blue);
DECL_VARIABLE_FOR_TRANSLATION(option_info);
DECL_VARIABLE_FOR_TRANSLATION(option_gfx_zero);
DECL_VARIABLE_FOR_TRANSLATION(option_sound_load_driver);
DECL_VARIABLE_FOR_TRANSLATION(option_sound_volume);
DECL_VARIABLE_FOR_TRANSLATION(option_info_sound);

DECL_VARIABLE_FOR_TRANSLATION(option_key_unassigned);
DECL_VARIABLE_FOR_TRANSLATION(option_key_force_left);
DECL_VARIABLE_FOR_TRANSLATION(option_key_force_right);
DECL_VARIABLE_FOR_TRANSLATION(option_key_scroll);
DECL_VARIABLE_FOR_TRANSLATION(option_key_priority);
DECL_VARIABLE_FOR_TRANSLATION(option_key_rate_minus);
DECL_VARIABLE_FOR_TRANSLATION(option_key_rate_plus);
DECL_VARIABLE_FOR_TRANSLATION(option_key_pause);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_back_one);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_back_many);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_ahead_one);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_ahead_many);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_fast);
DECL_VARIABLE_FOR_TRANSLATION(option_key_speed_turbo);
DECL_VARIABLE_FOR_TRANSLATION(option_key_restart);
DECL_VARIABLE_FOR_TRANSLATION(option_key_state_load);
DECL_VARIABLE_FOR_TRANSLATION(option_key_state_save);
DECL_VARIABLE_FOR_TRANSLATION(option_key_zoom);
DECL_VARIABLE_FOR_TRANSLATION(option_key_chat);
DECL_VARIABLE_FOR_TRANSLATION(option_key_spec_tribe);
DECL_VARIABLE_FOR_TRANSLATION(option_key_nuke);

DECL_VARIABLE_FOR_TRANSLATION(option_sorted_skills);

DECL_VARIABLE_FOR_TRANSLATION(option_key_me_okay);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_edit);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_export);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_delete);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_up_dir);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_up_1);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_up_5);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_down_1);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_down_5);
DECL_VARIABLE_FOR_TRANSLATION(option_key_me_exit);

DECL_VARIABLE_FOR_TRANSLATION(option_ed_left);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_right);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_up);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_down);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_copy);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_delete);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_grid);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_sel_all);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_sel_frame);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_sel_add);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_foreground);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_background);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_mirror);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_rotate);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_dark);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_noow);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_zoom);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_help);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_menu_size);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_menu_vars);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_menu_skills);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_terrain);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_steel);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_hatch);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_goal);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_deco);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_add_hazard);
DECL_VARIABLE_FOR_TRANSLATION(option_ed_grid_custom);

// Neuen Spieler begruessen (Mini-Dialog)
DECL_VARIABLE_FOR_TRANSLATION(option_new_player_title);
DECL_VARIABLE_FOR_TRANSLATION(option_new_player_first);
DECL_VARIABLE_FOR_TRANSLATION(option_new_player_second);

std::string last_string;

static char const* const special_varname_for_language = "main_name_of_language";

#define INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(vector, enumValue ) \
    (static_cast<void>(add_to_translation_map( #enumValue , vector[enumValue])))

// a dummy return value is provided to allow the function be
// called during initialization of static variables.  But in
// doing so please note the following:
//
// http://www.parashift.com/c++-faq-lite/static-init-order.html
//
// currently all references to global/static variables inside
// below function are initialized in this same source file and
// before the initialization expression calling below function,
// so linked-to issue above does not apply.
static int add_all_vector_elements_to_translation_map() {
    static_cast<void>(add_to_translation_map(special_varname_for_language,
                                             language_name[CUSTOM]));

    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::GENERAL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::CONTROLS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::HOTKEYS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::EDITOR);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::MENU);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::GRAPHICS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(option_group, Option::SOUND);

    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::FILE_NEW);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::FILE_EXIT);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::FILE_SAVE);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::FILE_SAVE_AS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::GRID_2);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::GRID_CUSTOM);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::GRID_16);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_ALL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_FRAME);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_ADD);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_COPY);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_DELETE);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_MINUS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_PLUS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_BACK);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_FRONT);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_FLIP);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_ROTATE);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_DARK);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::SELECT_NOOW);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::VIEW_ZOOM);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::HELP);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::MENU_SIZE);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::MENU_SCROLL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::MENU_VARS);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::MENU_SKILL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_TERRAIN);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_STEEL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_HATCH);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_GOAL);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_DECO);
    INCLUDE_VECTOR_ELEMENT_FOR_TRANSLATION(eb, Editor::ADD_HAZARD);

    return 0;
}

static int dummy_variable_finish_setting_up_translation_map =
    add_all_vector_elements_to_translation_map();

}  // namespace Language

// Rueckgabewert   Funktion
Language::Language Language::get()
{
    return static_cast <Language> (useR->language);
}



void Language::set(const unsigned i)
{
    if (i < MAX) set(static_cast <Language> (i));
    else         set(ENGLISH);
}



// ############################################################################
// ############################################################################
// ############################################################################



void Language::set(const Language lang)
{
    language_name[NONE]    = "None";
    language_name[GERMAN]  = "Deutsch";
    language_name[ENGLISH] = "English";
    if (language_name[CUSTOM].empty()) {
        language_name[CUSTOM]  = "(custom)";
    }
    language_name[MAX]     = "Max";

    if (lang < MAX) useR->language = lang;

    switch (useR->language) {

    case NONE:
        // Grundlegende Dinge
        main_name_of_the_game         = main_name_of_game_English;
        main_loading_1                = "---------- Loading Lix ----------";
        main_loading_2                = "---------- Please Wait ----------";
        main_version                  = "Version:";
        main_website                  = "asdfasdf.ethz.ch/~simon";
        break;

    case MAX:
        break; // Sollte nicht vorkommen, es wird bei Spielstart gefragt

    // custom language will always load English first, for proper
    // fallback behavior (eg. missing entries in translation file)
    case CUSTOM:
    case ENGLISH:

        // Grundlegende Dinge
        main_name_of_the_game         = main_name_of_game_English;
        main_loading_1                = "---------- Loading Lix ----------";
        main_loading_2                = "---------- Please Wait ----------";
        main_version                  = "Version:";
        main_website                  = "asdfasdf.ethz.ch/~simon";

        // Logging
        log_error                     = "Error: ";
        log_info                      = "  (i)  ";
        log_program_start_1           = "Lix is starting...";
        log_program_start_2           = "Lix successfully started.";
        log_program_exit              = "Lix successfully exited.";

        log_found                     = "found.";
        log_loaded                    = "read.";
        log_file_not_found            = "File not found:";
        log_bitmap_bad                = "Bad picture:";

        log_level_unknown_bitmap      = "Missing image:";
        log_level_file_saved          = "File written:";

        // Verschiedene Dialoge
        common_ok                     = "Okay";
        common_cancel                 = "Cancel";
        common_yes                    = "Yes";
        common_no                     = "No";
        common_back                   = "Back";
        common_exit                   = "Exit";
        common_dir_parent             = "..",
        common_dir_flip_page          = "(more...)",

        // Browser
        browser_single_title          = "Single Player";
        browser_network_title         = "Select Multiplayer level";
        browser_replay_title          = "Replays";
        browser_play                  = "Play";
        browser_edit                  = "Edit";
        browser_replay                = "Replay";
        browser_delete                = "Delete";
        browser_extract               = "Extract level";
        browser_export_image          = "Export image";
        browser_export_image_done     = "Done.";

        browser_info_author           = "By:";
        browser_info_initgoal         = "Save:";
        browser_info_initial          = "Lix:";
        browser_info_hatches          = "Hatches:";
        browser_info_goals            = "Goals:";
        browser_info_skills           = "Skills:";
        browser_info_clock_2          = "Overtime:";
        browser_info_author_none      = "?";
        browser_info_clock_none       = "--";

        browser_info_result_saved     = "Saved:";
        browser_info_result_skills    = "Skills used:";
        browser_info_result_time      = "Time used:";
        browser_info_result_old_1     = "The level has";
        browser_info_result_old_2     = "been edited.";

        browser_info_player           = "Player:";
        browser_info_version          = "Game:";
        browser_info_built            = "Level:";
        browser_info_new              = "is too new";
        browser_info_same             = "fits!";
        browser_info_old              = "is too old";
        browser_info_holds_level      = "contained";

        browser_box_delete_tit_rep    = "Delete replay?";
        browser_box_delete_que_rep    = "Do you really want to delete "
                                        "this replay?";
        browser_box_delete_tit_lev    = "Delete level?";
        browser_box_delete_que_lev    = "Do you really want to delete "
                                        "this level?";

        // Lobby
        win_lobby_title               = "Network Game";
        win_lobby_title_lobby         = "Lobby";
        win_lobby_title_room          = "Room #";
        win_lobby_exit                = "Leave network";
        win_lobby_unstable_central    = "(Experimental version. Please don't "
                                        "use central server.)";
        win_lobby_start_central       = "Connect to central server";
        win_lobby_start_server        = "Host a game yourself";
        win_lobby_start_client        = "Connect to:";
        win_lobby_chat                = "Chat:";
        win_lobby_select_level        = "Choose level";
        win_lobby_ready               = "Ready for start";
        win_lobby_room_number         = "Room #";
        win_lobby_room_player         = "player";
        win_lobby_room_players        = "players";
        win_lobby_room_create         = "Open a new room";
        win_lobby_room_leave          = "Leave room";

        // Spielfenster
        win_game_title                = "Game Menu";
        win_game_result               = "Result";
        win_game_lix_saved            = "Lix saved:";
        win_game_lix_saved_in_time    = "Lix saved in time:";
        win_game_resume               = "Continue";
        win_game_restart              = "Restart level";
        win_game_save_replay          = "Save replay";
        win_game_menu                 = "Exit";
        win_game_comment_perfect      = "Perfect! All saved!";
        win_game_comment_more         = "Super, more than necessary!";
        win_game_comment_exactly      = "Target reached exactly!";
        win_game_comment_less         = "Sorry, those were not enough.";
        win_game_comment_none         = "That was nothing... Try again?";
        win_game_result_skills        = "Skills used:";
        win_game_result_time          = "Time taken:";
        win_game_net_first            = "Awesome! You're the champion!";
        win_game_net_second           = "Well done, nice second place.";
        win_game_net_middle           = "An okay outcome... Revanche?";
        win_game_net_last             = "Sorry, you lie behind this time.";
        win_game_net_first_tie        = "Great, you're among the champs!";
        win_game_net_last_tie         = "At least you're not alone there.";
        win_game_net_all_tie          = "What's that? No true winner?";
        win_game_net_zero             = "Rock bottom! Did you nuke them?";
        win_game_net_all_zero         = "Do we have a nuclear winter?";
        win_game_replay_win_one       = "has won!";
        win_game_replay_win_team      = "have won!";
        win_game_replay_tie           = "The game is a tie.";
        win_game_overwrite_title      = "Overwrite Replay?";
        win_game_overwrite_question   = "Do you really want to overwrite this replay?";

        // Help texts inside the game
        gameplay_rate_minus           = "Raise spawn interval: Lix enter the level slower.";
        gameplay_rate_plus            = "Lower spawn interval: Lix enter the level faster.";
        gameplay_pause                = "Pause the game.";
        gameplay_zoom                 = "Zoom into the level.";
        gameplay_left_right_click     = "Left/right click:";
        gameplay_speed_back           = "Back by 1 frame/1 second.";
        gameplay_speed_ahead          = "Ahead by 1 frame/1 second.";
        gameplay_speed_fast           = "fast/turbo-fast forward.";
        gameplay_state_save           = "Quicksave the current game position.";
        gameplay_state_load           = "Load the previously quicksaved position.";
        gameplay_restart              = "Restart the level.";
        gameplay_nuke                 = "Nuke. Activate with a double click.";
        gameplay_hint_first           = "View a hint for this level.";
        gameplay_hint_next            = "View the next hint.";
        gameplay_hint_prev            = "View the previous hint.";
        gameplay_hint_off             = "Hide the hint.";
        gameplay_spec_tribe           = "Examine the skills of a different team.";

        gameplay_rmb                  = "right mouse button";
        gameplay_mmb                  = "middle mouse button";
        gameplay_scroll_right_1       = "To scroll, hold ";
        gameplay_scroll_right_2       = ", move mouse around.";
        gameplay_force_dir_1          = "Hold [";
        gameplay_force_dir_2          = "] or [";
        gameplay_force_dir_3          = "] to select a lix looking left or right.";
        gameplay_priority_1           = "Hold ";
        gameplay_priority_2           = " to assign to the walker.";
        gameplay_queue_builders       = "Click the Lix again to queue several builders.";
        gameplay_queue_platformers    = "Click the Lix again to queue several platformers.";


        // Editor allgemein
        editor_unsaved_title          = "Save changes?";
        editor_unsaved_title_new      = "Save level?";
        editor_unsaved_question       = "Save the changes on this level?";
        editor_unsaved_question_new   = "Save this new level?";
        editor_file_name              = "File name:";
        editor_level_name             = "Level title:";
        editor_hotkey                 = "Hotkey:";
        editor_hotkeys                = "Hotkeys:";
        editor_hotkey_hold            = "Hotkey: hold";
        editor_bar_at                 = "at";
        editor_bar_hover              = "objects about to be selected.";
        editor_bar_selection          = "objects selected.";
        editor_bar_movekeys_long      = "Drag objects or move them with [";
        editor_bar_movekeys_short     = "Move with [";
        editor_bar_movekeys_mid       = "], [";
        editor_bar_movekeys_end       = "].";

        // SaveBrowser
        save_browser_title            = "Save As";
        save_filename                 = "File name:";
        save_box_overwrite_title      = "Overwrite Level?";
        save_box_overwrite_question   = "Do you really want to overwrite this level?";

        // BitmapBrowser
        add_terrain                   = "Add terrain";
        add_steel                     = "Add steel";
        add_hatch                     = "Add hatch";
        add_goal                      = "Add goal";
        add_deco                      = "Add decoration";
        add_hazard                    = "Add hazard";

        // Kartengroesse
        win_size_title                = "Level Measures";
        win_size_l                    = "Left:";
        win_size_r                    = "Right:";
        win_size_u                    = "Top:";
        win_size_d                    = "Bottom:";
        win_size_x                    = "Width:";
        win_size_y                    = "Height:";
        win_size_hex                  = "Show size in hexadecimal";
        win_size_torus_x              = "Wrap around horizontally";
        win_size_torus_y              = "Wrap around vertically";

        // Scrollposition
        win_scroll_title              = "Level Visuals";
        win_scroll_manual             = "Manually set initially visible"
                                        " region";
        win_scroll_x                  = "X Coordinate (0 = left)";
        win_scroll_y                  = "Y Coordinate (0 = top)";
        win_scroll_r                  = "Background red";
        win_scroll_g                  = "Background green";
        win_scroll_b                  = "Background blue";
        win_scroll_jump               = "Scroll there";
        win_scroll_current            = "Current Position";

        // Variablenfenster
        win_var_title                 = "General Level Settings";
        win_var_author                = "Author";
        win_var_name_german           = "German title";
        win_var_name_english          = "English title";
        win_var_initial               = "No. of lix";
        win_var_required              = "Lix to save";
        win_var_spawnint_slow         = "Spawn interval";
        win_var_spawnint_fast         = "Spawn int. fast";
        win_var_clock                 = "Time/overtime";

        // Faehigkeits-Fenster
        win_skill_title               = "Skill Settings";
        win_skill_clear               = "Clear all";
        win_skill_classic_8_to        = "Adjust classic 8 to:";
        win_skill_all_to              = "Adjust all:";
        win_skill_use_fling           = "Use knockback exploder";

        // Exporting a level into a file
        export_single_lix             = "Save:";
        export_single_spawnint        = "Spawn int.:";
        export_single_clock           = "Time:";
        export_multi_lix              = "Lix:";
        export_multi_spawnint         = "Spawn int.:";
        export_multi_clock            = "Overtime:";

        // Netzwerk-Chatnachrichten
        net_chat_welcome_1            = "You can join the central server, "
                                        "host a game yourself,";
        net_chat_welcome_2            = "or enter an IP address/a host name "
                                        "and click \"" + win_lobby_start_client
                                         + "\".";
        net_chat_unstable_1           = "This is an experimental version. "
                                        "Host a game yourself";
        net_chat_unstable_2           = net_chat_welcome_2;
        net_chat_start_server         = "You are the server. Other "
                                        "players can connect to you now.";
        net_chat_start_client         = "Searching a server at";
        net_chat_start_cancel         = "Connection attempt cancelled.";
        net_chat_disconnection        = " has quit the network.";
        net_chat_we_too_old           = "You have too old a Lix version "
                                        "to connect to the server.";
        net_chat_we_too_new           = "You have too new a Lix version. "
                                        "The server should upgrade.";
        net_chat_someone_old          = "Someone with too old a Lix version "
                                        "tried to connect.";
        net_chat_someone_new          = "Someone with too new a Lix version "
                                        "tried to connect.";
        net_chat_named_guy_old        = " must update his Lix version to "
                                        "continue.";
        net_chat_version_yours        = "Your version is ";
        net_chat_version_server       = ", the server requires ";
        net_chat_server_update        = "The server should update to the "
                                        "most recent Lix version.";
        net_chat_please_download      = "Download the newest version from: "
                                      + main_website;
        net_chat_we_connected         = "You have joined the network. "
                                        "Enter a room to start playing.";
        net_chat_we_in_room           = "You have entered room #";
        net_chat_we_in_room_2         = ".";
        net_chat_we_in_lobby          = "You went back into the lobby.";
        net_chat_player_in_room       = " has joined room #";
        net_chat_player_in_room_2     = ".";
        net_chat_player_in_lobby      = " has entered the lobby.";
        net_chat_player_out_room      = " has entered room #";
        net_chat_player_out_room_2    = ".";
        net_chat_player_out_lobby     = " has left the room.";
        net_chat_level_change         = "has selected this level:";
        net_game_start                = "The game has started.";
        net_game_how_to_chat_1        = " Press [";
        net_game_how_to_chat_2        = "] to chat.";
        net_game_end                  = "The game is over.";
        net_game_end_result           = "Game outcome:";
        net_game_overtime_1           = "has finished playing and has saved";
        net_game_overtime_2           = "lix.";
        net_game_overtime_2_one       = "one lix.";
        net_game_overtime_3           = "Now, the overtime of";
        net_game_overtime_4           = "minutes will begin!";
        net_game_overtime_4_one       = "minute will begin!";
        net_game_overtime_4_sec       = "seconds will begin!";

        // Optionsfenster
        option_title                  = "Options";
        option_group[0]               = "General";
        option_group[1]               = "Controls";
        option_group[2]               = "Hotkeys";
        option_group[3]               = "Editor";
        option_group[4]               = "Menu";
        option_group[5]               = "Graphics";
        option_group[6]               = "Sound";
        option_user_name              = "Player name";
        option_user_name_ask          = "Ask for name on startup";
        option_language               = "Language";
        option_replay_auto_max        = "Max. auto replays";
        option_replay_auto_single     = "Auto replay in Singleplayer";
        option_replay_auto_multi      = "Auto replay in Multiplayer";
        option_replay_cancel          = "Replay ends: normal speed...";
        option_replay_cancel_at       = "...1/15 sec before end";
        option_scroll_edge            = "Scroll at screen border";
        option_scroll_right           = "Scroll with right click";
        option_scroll_middle          = "Scroll with middle click";
        option_mouse_speed            = "Mouse speed";
        option_scroll_speed_edge      = "Edge scroll speed";
        option_scroll_speed_click     = "Right/mid. scroll spd.";
        option_allow_builder_queuing  = "Builders: allow queuing";
        option_avoid_builder_queuing  = "Builders: avoid queuing";
        option_avoid_batter_to_exploder="Avoid assigning batter to exploder";
        option_prioinv_middle         = "Middle button inverts priority";
        option_prioinv_right          = "Right button inverts priority";

        option_screen_windowed        = "Windowed mode*";
        option_screen_resolution      = "Fullscreen resol.**";
        option_screen_windowed_res    = "Windowed resolution";
        option_screen_scaling         = "Screen scaling";
        option_screen_scaling_stretch = "Stretched";
        option_screen_scaling_aspect  = "Aspect ratio";
        option_screen_scaling_integer = "Integer";
        option_screen_border_colored  = "Screen border colored";
        option_screen_vsync           = "Wait for V-sync";
        option_arrows_replay          = "Arrows during replays";
        option_arrows_network         = "Arrows in network games";
        option_game_show_tooltips     = "Tooltips during game";
        option_game_show_skill_keys   = "Show hotkeys on skills";
        option_debris_amount          = "Debris amount";
        option_debris_amount_none     = "None";
        option_debris_amount_own      = "Own";
        option_debris_amount_all      = "All";
        option_debris_type            = "Debris type";
        option_debris_type_stars      = "Stars, clouds";
        option_debris_type_pixels     = "Pixels";
        option_gui_color_red          = "Menu color red";
        option_gui_color_green        = "Menu color green";
        option_gui_color_blue         = "Menu color blue";
        option_info                   = "*) Hit [Alt] + [Enter] to toggle full"
                                        "screen/windowed mode at any time.";
        option_gfx_zero               = "**) Enter 0 in both fields to use "
                                        "your normal desktop resolution.";
        option_sound_load_driver      = "Load sound driver*";
        option_sound_volume           = "Sound volume";
        option_info_sound             = "*) This option requires a program "
                                        "restart to take effect.";

        option_key_unassigned         = "none";
        option_key_force_left         = "Force left";
        option_key_force_right        = "Force right";
        option_key_scroll             = "Hold to scroll";
        option_key_priority           = "Priority invert";
        option_key_rate_minus         = "Interval +";
        option_key_rate_plus          = "Interval -";
        option_key_pause              = "Pause";
        option_key_speed_back_one     = "-1 frame";
        option_key_speed_back_many    = "-1 second";
        option_key_speed_ahead_one    = "+1 frame";
        option_key_speed_ahead_many   = "+1 second";
        option_key_speed_fast         = "Fast forw.";
        option_key_speed_turbo        = "Turbo speed";
        option_key_restart            = "Restart";
        option_key_state_load         = "Load state";
        option_key_state_save         = "Save state";
        option_key_zoom               = "Zoom";
        option_key_chat               = "Chat";
        option_key_spec_tribe         = "Cycle teams";
        option_key_nuke               = "Nuke";

        option_sorted_skills          = "Click two images to swap their"
                                        " position in the game's panel.";

        option_key_me_okay            = "Okay/yes";
        option_key_me_edit            = "Edit";
        option_key_me_export          = "Export";
        option_key_me_delete          = "Delete/no";
        option_key_me_up_dir          = "Parent dir";
        option_key_me_up_1            = "Up by 1";
        option_key_me_up_5            = "Up by 5";
        option_key_me_down_1          = "Down by 1";
        option_key_me_down_5          = "Down by 5";
        option_key_me_exit            = "Back/cancel";

        option_ed_left                = "Move left";
        option_ed_right               = "Move right";
        option_ed_up                  = "Move up";
        option_ed_down                = "Move down";
        option_ed_copy                = "Copy";
        option_ed_delete              = "Delete";
        option_ed_grid                = "Grid size";
        option_ed_sel_all             = "Select all";
        option_ed_sel_frame           = "Select frame";
        option_ed_sel_add             = "Add to sel.";
        option_ed_foreground          = "Foreground";
        option_ed_background          = "Background";
        option_ed_mirror              = "Mirror";
        option_ed_rotate              = "Rotate";
        option_ed_dark                = "Draw black";
        option_ed_noow                = "No overwrite";
        option_ed_zoom                = "Zoom";
        option_ed_help                = "Show help";
        option_ed_menu_size           = "Size menu";
        option_ed_menu_vars           = "General menu";
        option_ed_menu_skills         = "Skills menu";
        option_ed_add_terrain         = "Add terrain";
        option_ed_add_steel           = "Add steel";
        option_ed_add_hatch           = "Add hatch";
        option_ed_add_goal            = "Add goal";
        option_ed_add_deco            = "Add deco";
        option_ed_add_hazard          = "Add hazard";
        option_ed_grid_custom         = "Custom grid size";

        // Neuen Spieler begruessen (Mini-Dialog)
        option_new_player_title       = "Lix";
        option_new_player_first       = "Hello and Welcome!";
        option_new_player_second      = "Please enter your name:";

eb[Editor::FILE_NEW]
 = "New: Deletes everything on screen and starts a new level.";
eb[Editor::FILE_EXIT]
 = "Quit: Exits the editor and asks about saving changed data.";
eb[Editor::FILE_SAVE]
 = "Save: Saves the current level under its current file name.";
eb[Editor::FILE_SAVE_AS]
 = "Save As: Saves the current level under a new file name.";
eb[Editor::GRID_2]
 = "Grid 2: Rounds object coordinates to multiples of 2.";
eb[Editor::GRID_CUSTOM]
 = "Custom grid: This grid size is settable in the options.";
eb[Editor::GRID_16]
 = "Grid 16: Rounds object coordinates to multiples of 16.";
eb[Editor::SELECT_ALL]
 = "Everything: Selects all objects in the level.";
eb[Editor::SELECT_FRAME]
 = "Frame: Selects multiple objects by dragging a frame.";
eb[Editor::SELECT_ADD]
 = "Add: Selects new objects, keeping old ones in the selection.";
eb[Editor::SELECT_COPY]
 = "Copy: Clones the selected objects for placing them elsewhere.";
eb[Editor::SELECT_DELETE]
 = "Delete: Removes the selected objects.";
eb[Editor::SELECT_MINUS]
 = "Back: Puts the selected objects behind some others.";
eb[Editor::SELECT_PLUS]
 = "Forth: Puts the selected objects in front of some others.";
eb[Editor::SELECT_BACK]
 = "Background: Puts the selected objects into the background.";
eb[Editor::SELECT_FRONT]
 = "Foreground: Puts the selected objects into the foreground.";
eb[Editor::SELECT_FLIP]
 = "Flip: Mirrors the selected terrain horizontally.";
eb[Editor::SELECT_ROTATE]
 = "Rotate: Performs a quarter turn on the selected terrain.";
eb[Editor::SELECT_DARK]
= "Dark: Paints the selected terrain in black inside the game.";
eb[Editor::SELECT_NOOW]
= "No overwrite: Selected terrain overwrites only dark objects.";
eb[Editor::VIEW_ZOOM]
= "Zoom: Activate or deactivate the map zoom.";
eb[Editor::HELP]
 = "(this doesn't do anything right now)";
eb[Editor::MENU_SIZE]
 = "Measures: Set the size and the topology for the level.";
eb[Editor::MENU_SCROLL]
 = "Visuals: Set the scrolling position and background color.";
eb[Editor::MENU_VARS]
 = "Variables: Set the fundamental level variables.";
eb[Editor::MENU_SKILL]
 = "Skills: Set the usable lix skills.";
eb[Editor::ADD_TERRAIN]
 = "Terrain: Add a terrain object that can be dug through.";
eb[Editor::ADD_STEEL]
 = "Steel: Add a steel object that cannot be dug through.";
eb[Editor::ADD_HATCH]
 = "Hatch: Add a lix entrance hatch.";
eb[Editor::ADD_GOAL]
 = "Goal: Add a goal, i.e. an exit for the lix.";
eb[Editor::ADD_DECO]
 = "Decoration: Add a non-interactive decoration object.";
eb[Editor::ADD_HAZARD]
 = "Hazard: Add a trap, water or fire.";

        break;
        // Ende Englisch



// ############################################################################
// ############################################################################
// ############################################################################



    case GERMAN:

        // Grundlegende Dinge
        main_name_of_the_game         = main_name_of_game_English; // "Lix"
        main_loading_1                = "------- Lix wird geladen -------";
        main_loading_2                = "--------- Bitte warten ---------";
        main_version                  = "Version:";
        main_website                  = "asdfasdf.ethz.ch/~simon";

        // Logging
        log_error                     = "Fehler:";
        log_info                      = "  (i)  ";
        log_program_start_1           = "Lix wird gestartet...";
        log_program_start_2           = "Lix erfolgreich gestartet.";
        log_program_exit              = "Lix erfolgreich beendet.";

        log_found                     = "gefunden.";
        log_loaded                    = "gelesen.";
        log_file_not_found            = "Datei nicht gefunden:";
        log_bitmap_bad                = "Fehlerhaftes Bild:";

        log_level_unknown_bitmap      = "Fehlendes Bild:";
        log_level_file_saved          = "Datei geschrieben:";

        // Verschiedene Dialoge
        common_ok                     = "Okay";
        common_cancel                 = "Abbrechen";
        common_yes                    = "Ja";
        common_no                     = "Nein";
        common_back                   = "Zurück";
        common_exit                   = "Beenden";
        common_dir_parent             = "..",
        common_dir_flip_page          = "(weitere...)",

        // Browser
        browser_single_title          = "Einzelspieler";
        browser_network_title         = "Netzwerk-Level aussuchen";
        browser_replay_title          = "Replays";
        browser_play                  = "Spielen";
        browser_edit                  = "Editieren";
        browser_replay                = "Abspielen";
        browser_delete                = "Löschen";
        browser_extract               = "Level extrah.";
        browser_export_image          = "Bild export.";
        browser_export_image_done     = "Fertig.";

        browser_info_author           = "Von:";
        browser_info_initgoal         = "Ziel:";
        browser_info_initial          = "Anzahl Lix:";
        browser_info_hatches          = "Klappen:";
        browser_info_goals            = "Ausgänge:";
        browser_info_skills           = "Fähigkeiten:";
        browser_info_clock_2          = "Nachzeit:";
        browser_info_author_none      = "?";
        browser_info_clock_none       = "--";

        browser_info_result_saved     = "Gerettet:";
        browser_info_result_skills    = "Fähigkeiten:";
        browser_info_result_time      = "Dauer:";
        browser_info_result_old_1     = "Der Level wurde";
        browser_info_result_old_2     = "seitdem editiert.";

        browser_info_player           = "Spieler:";
        browser_info_version          = "Spiel:";
        browser_info_built            = "Level:";
        browser_info_new              = "ist zu neu";
        browser_info_same             = "passt!";
        browser_info_old              = "ist zu alt";
        browser_info_holds_level      = "enthalten";

        browser_box_delete_tit_rep    = "Replay löschen?";
        browser_box_delete_que_rep    = "Soll dieses Replay wirklich "
                                        "gelöscht werden?";
        browser_box_delete_tit_lev    = "Level löschen?";
        browser_box_delete_que_lev    = "Soll dieser Level wirklich "
                                        "gelöscht werden?";

        // Lobby
        win_lobby_title               = "Netzwerkspiel";
        win_lobby_title_lobby         = "Lobby";
        win_lobby_title_room          = "Raum Nr. ";
        win_lobby_exit                = "Netz verlassen";
        win_lobby_unstable_central    = "(Experimentelle Version, bitte nicht "
                                        "zentralen Server nutzen.)";
        win_lobby_start_central       = "Verbinden zu zentralem Server";
        win_lobby_start_server        = "Selbst ein Spiel hosten";
        win_lobby_start_client        = "Verbinden zu:";
        win_lobby_chat                = "Chat:";
        win_lobby_select_level        = "Level aussuchen";
        win_lobby_ready               = "Bereit zum Start";
        win_lobby_room_number         = "Raum Nr. ";
        win_lobby_room_player         = "Spieler";
        win_lobby_room_players        = "Spieler";
        win_lobby_room_create         = "Neuen Raum eröffnen";
        win_lobby_room_leave          = "Raum verlassen";

        // Spielfenster
        win_game_title                = "Spielmenü";
        win_game_result               = "Ergebnis";
        win_game_lix_saved            = "Lix gerettet:";
        win_game_lix_saved_in_time    = "Lix rechtzeitig gerettet:";
        win_game_resume               = "Weiterspielen";
        win_game_restart              = "Level neustarten";
        win_game_save_replay          = "Replay speichern";
        win_game_menu                 = "Beenden";
        win_game_comment_perfect      = "Perfekt! Alle gerettet!";
        win_game_comment_more         = "Super, mehr als notwendig!";
        win_game_comment_exactly      = "Ziel haargenau erreicht!";
        win_game_comment_less         = "Das sind leider zu wenige.";
        win_game_comment_none         = "Das war wohl nix. Nochmal?";
        win_game_result_skills        = "Fähigkeiten:";
        win_game_result_time          = "Gebrauchte Zeit:";
        win_game_net_first            = "Ein hervorragender Sieg!";
        win_game_net_second           = "Fein, guter zweiter Platz.";
        win_game_net_middle           = "Naja, ganz okay. Revanche?";
        win_game_net_last             = "Schade, du liegst hinten.";
        win_game_net_first_tie        = "Super, einer der Ersten!";
        win_game_net_last_tie         = "Immerhin nicht alleine unten.";
        win_game_net_all_tie          = "Wie das? Kein klarer Sieger?";
        win_game_net_zero             = "Null komma nix, diese Runde!";
        win_game_net_all_zero         = "Haben wir nuklearen Winter?";
        win_game_replay_win_one       = "hat gewonnen!";
        win_game_replay_win_team      = "haben gewonnen!";
        win_game_replay_tie           = "Das Spiel ist unentschieden.";
        win_game_overwrite_title      = "Replay überschreiben?";
        win_game_overwrite_question   = "Soll dieses Replay wirklich überschrieben werden?";

        // Help texts inside the game
        gameplay_rate_minus           = "Fall-Intervall: Lix erscheinen langsamer.";
        gameplay_rate_plus            = "Fall-Intervall: Lix erscheinen schneller.";
        gameplay_pause                = "Pause.";
        gameplay_zoom                 = "Level-Zoom an- oder ausschalten.";
        gameplay_left_right_click     = "Links-/Rechtsklick:";
        gameplay_speed_back           = "1 Bild/1 Sekunde zurück.";
        gameplay_speed_ahead          = "1 Bild/1 Sekunde vor.";
        gameplay_speed_fast           = "schneller Vorlauf/Turbo.";
        gameplay_state_save           = "Aktuellen Spielstand schnellspeichern.";
        gameplay_state_load           = "Den schnellgespeicherten Spielstand laden.";
        gameplay_restart              = "Level neustarten.";
        gameplay_nuke                 = "Atombombe. Aktivierung durch Doppelklick.";
        gameplay_hint_first           = "Einen Tipp fuer diesen Level lesen.";
        gameplay_hint_next            = "Den nächsten Tipp lesen.";
        gameplay_hint_prev            = "Den vorherigen Tipp lesen.";
        gameplay_hint_off             = "Den Tipp ausblenden.";
        gameplay_spec_tribe           = "Fähigkeiten eines anderen Teams ansehen.";

        gameplay_rmb                  = "rechte Maustaste";
        gameplay_mmb                  = "mittlere Maustaste";
        gameplay_scroll_right_1       = "Scrollen: Halte ";
        gameplay_scroll_right_2       = ", bewege die Maus umher.";
        gameplay_force_dir_1          = "Halte [";
        gameplay_force_dir_2          = "] oder [";
        gameplay_force_dir_3          = "], um die Lix nach links oder rechts zu wählen.";
        gameplay_priority_1           = "Halte ";
        gameplay_priority_2           = ", um die Geherin zu wählen.";
        gameplay_queue_builders       = "Klicke die Lix erneut an, um längere Treppen zu bauen.";
        gameplay_queue_platformers    = "Klicke die Lix erneut an, um längere Plattformen zu bauen.";

        // Editor allgemein
        editor_unsaved_title          = "Änderungen speichern?";
        editor_unsaved_title_new      = "Level speichern?";
        editor_unsaved_question       = "Änderungen an diesem Level speichern?";
        editor_unsaved_question_new   = "Neuen Level speichern?";
        editor_file_name              = "Dateiname:";
        editor_level_name             = "Leveltitel:";
        editor_hotkey                 = "Hotkey:";
        editor_hotkeys                = "Hotkeys:";
        editor_hotkey_hold            = "Hotkey: halte";
        editor_bar_at                 = "an";
        editor_bar_hover              = "Objekte werden markiert.";
        editor_bar_selection          = "Objekte markiert.";
        editor_bar_movekeys_long      = "Ziehe Objekte umher oder bewege sie mit [";
        editor_bar_movekeys_short     = "Bewegen mit [";
        editor_bar_movekeys_mid       = "], [";
        editor_bar_movekeys_end       = "].";

        // SaveBrowser
        save_browser_title            = "Speichern unter";
        save_filename                 = "Dateiname:";
        save_box_overwrite_title      = "Level überschreiben?";
        save_box_overwrite_question   = "Soll dieser Level wirklich überschrieben werden?";

        // BitmapBrowser
        add_terrain                   = "Terrain hinzufügen";
        add_steel                     = "Stahl hinzufügen";
        add_hatch                     = "Klappe hinzufügen";
        add_goal                      = "Ziel hinzufügen";
        add_deco                      = "Dekoration hinzufügen";
        add_hazard                    = "Gefahr hinzufügen";

        // Kartengroesse
        win_size_title                = "Level-Abmessungen";
        win_size_l                    = "Links:";
        win_size_r                    = "Rechts:";
        win_size_u                    = "Oben:";
        win_size_d                    = "Unten:";
        win_size_x                    = "Breite:";
        win_size_y                    = "Höhe:";
        win_size_hex                  = "Hexadezimal anzeigen";
        win_size_torus_x              = "Horizontal rundumgehend";
        win_size_torus_y              = "Vertikal rundumgehend";

        // Scrollposition
        win_scroll_title              = "Aussehen des Levels";
        win_scroll_manual             = "Anfangs sichtbaren Bereich manuell"
                                        " festlegen";
        win_scroll_x                  = "X-Koordinate (0 = links)";
        win_scroll_y                  = "Y-Koordinate (0 = oben)";
        win_scroll_r                  = "Hintergrund Rot";
        win_scroll_g                  = "Hintergrund Grün";
        win_scroll_b                  = "Hintergrund Blau";
        win_scroll_jump               = "Dorthin scrollen";
        win_scroll_current            = "Aktuelle Position";

        // Variablenfenster
        win_var_title                 = "Allgemeine Leveleinstellungen";
        win_var_author                = "Autor";
        win_var_name_german           = "Deutscher Titel";
        win_var_name_english          = "Englischer Titel";
        win_var_initial               = "Anzahl Lix";
        win_var_required              = "Lix zu retten";
        win_var_spawnint_slow         = "Fall-Intervall";
        win_var_spawnint_fast         = "Fall-I. schnell";
        win_var_clock                 = "Zeit/Nachspielz.";

        // Faehigkeits-Fenster
        win_skill_title               = "Fähigkeits-Einstellungen";
        win_skill_clear               = "Alle löschen";
        win_skill_classic_8_to        = "Klassische 8 ändern:";
        win_skill_all_to              = "Alle ändern:";
        win_skill_use_fling           = "Rückstoß-Explodierer";

        // Exporting a level into a file
        export_single_lix             = "Ziel:";
        export_single_spawnint        = "Fall-Int.:";
        export_single_clock           = "Zeit:";
        export_multi_lix              = "Lix:";
        export_multi_spawnint         = "Fall-Int.:";
        export_multi_clock            = "Nachspielzeit:";

        // Netzwerk-Chatnachrichten
        net_chat_welcome_1            = "Verbinde zum Zentralserver, eröffne "
                                        "selbst ein Spiel oder gib";
        net_chat_welcome_2            = "eine IP-Adresse/einen Rechnernamen "
                                        "ein und wähle \""
                                        + win_lobby_start_client + "\".";
        net_chat_unstable_1           = "Diese Version ist experimentell. "
                                        "Eröffne selbst ein Spiel oder gib";
        net_chat_unstable_2           = net_chat_welcome_2;
        net_chat_start_server         = "Du bist der Server. Andere Spieler "
                                        "können zu dir verbinden.";
        net_chat_start_client         = "Suche Server unter";
        net_chat_start_cancel         = "Verbindungsaufbau abgebrochen.";
        net_chat_disconnection        = " hat das Netzwerk verlassen.";
        net_chat_we_too_old           = "Deine Lix-Version ist zu alt, "
                                        "um verbinden zu können.";
        net_chat_we_too_new           = "Deine Lix-Version ist zu neu,"
                                        "um verbinden zu können.";
        net_chat_someone_old          = "Jemand wollte verbinden, aber seine "
                                        "Lix-Version war zu alt.";
        net_chat_someone_new          = "Jemand wollte verbinden, aber seine "
                                        "Lix-Version war zu neu.";
        net_chat_named_guy_old        = " muss zunächst seine Lix-Version "
                                        "aktualisieren.";
        net_chat_version_yours        = "Your version is ";
        net_chat_version_server       = ", the server has ";
        net_chat_please_download      = "Download the newest version from: "
                                      + main_website;
        net_chat_we_connected         = "Du hast das Netzwerk betreten. "
                                        "Wähle einen Raum zum Spielen.";
        net_chat_we_in_room           = "Du hast Raum Nr. ";
        net_chat_we_in_room_2         = " betreten.";
        net_chat_we_in_lobby          = "Du bist in die Lobby "
                                        "zurückgegangen.";
        net_chat_player_in_room       = " hat Raum Nr. ";
        net_chat_player_in_room_2     = " betreten.";
        net_chat_player_in_lobby      = " hat die Lobby betreten.";
        net_chat_player_out_room      = " wechselt in Raum Nr. ";
        net_chat_player_out_room_2    = ".";
        net_chat_player_out_lobby     = " hat den Raum verlassen.";
        net_chat_version_yours        = "Deine Version ist ";
        net_chat_version_server       = ", der Server verlangt ";
        net_chat_server_update        = "Der Server sollte seine Lix-Version "
                                        "aktualisieren.";
        net_chat_please_download      = "Lade die neueste Version herunter: "
                                      + main_website;
        net_chat_level_change         = "hat diesen Level gewählt:";
        net_game_start                = "Das Spiel beginnt.";
        net_game_how_to_chat_1        = " Du kannst mit [";
        net_game_how_to_chat_2        = "] chatten.";
        net_game_end                  = "Das Spiel ist zu Ende.";
        net_game_end_result           = "Spielergebnis:";
        net_game_overtime_1           = "hat zu Ende gespielt und";
        net_game_overtime_2           = "Lix gerettet.";
        net_game_overtime_2_one       = "eine Lix gerettet.";
        net_game_overtime_3           = "Jetzt beginnt die Nachspielzeit von";
        net_game_overtime_4           = "Minuten!";
        net_game_overtime_4_one       = "Minute!";
        net_game_overtime_4_sec       = "Sekunden!";

        // Optionsfenster
        option_title                  = "Optionen";
        option_group[0]               = "Allgemein";
        option_group[1]               = "Steuerg.";
        option_group[2]               = "Hotkeys";
        option_group[3]               = "Editor";
        option_group[4]               = "Menü";
        option_group[5]               = "Grafik";
        option_group[6]               = "Audio";
        option_user_name              = "Spielername";
        option_user_name_ask          = "Namensfrage bei Start";
        option_replay_auto_max        = "Max. Auto-Replays";
        option_replay_auto_single     = "Auto-Replay Einzelspieler";
        option_replay_auto_multi      = "Auto-Replay Mehrspieler";
        option_replay_cancel          = "Replayende: norm. Geschw.";
        option_replay_cancel_at       = "...1/15 Sek. vor Ende";
        option_scroll_edge            = "Scrollen am Bildrand";
        option_scroll_right           = "Scrollen bei Rechtsklick";
        option_scroll_middle          = "Scrollen bei mittl. Maust.";
        option_mouse_speed            = "Mausgeschwindigkeit";
        option_scroll_speed_edge      = "Scrollgeschw. am Rand";
        option_scroll_speed_click     = "Scrollgeschw. bei Klick";
        option_allow_builder_queuing  = "Builder: Mehrfach-Klicks erlauben";
        option_avoid_builder_queuing  = "Builder: Mehrfach-Klicks vermeiden";
        option_avoid_batter_to_exploder="Exploder -> Batter vermeiden";
        option_prioinv_middle         = "Mittl. Taste invert. Priorität";
        option_prioinv_right          = "Rechte Taste invert. Priorität";

        option_screen_windowed        = "Fenstermodus*";
        option_screen_resolution      = "Vollbild-Auflösung**";
        option_screen_windowed_res    = "Fenster-Auflösung";
        option_screen_scaling         = "Vergrößerung";
        option_screen_scaling_stretch = "Gestreckt";
        option_screen_scaling_aspect  = "Seitenverhältnis";
        option_screen_scaling_integer = "Ganzzahlig";
        option_screen_border_colored  = "Bildschirmrand farbig";
        option_screen_vsync           = "V-Sync abwarten";
        option_arrows_replay          = "Pfeile im Replay";
        option_arrows_network         = "Pfeile im Netzwerkspiel";
        option_game_show_tooltips     = "Tooltips im Spiel";
        option_game_show_skill_keys   = "Fähgkt.-Hotkeys anzeigen";
        option_debris_amount          = "Trümmer-Anzahl";
        option_debris_amount_none     = "Keine";
        option_debris_amount_own      = "Lokaler Spieler";
        option_debris_amount_all      = "Alle";
        option_debris_type            = "Trümmer-Aussehen";
        option_debris_type_stars      = "Sterne, Wolken";
        option_debris_type_pixels     = "Pixel";
        option_gui_color_red          = "Menüfarbe Rot";
        option_gui_color_green        = "Menüfarbe Grün";
        option_gui_color_blue         = "Menüfarbe Blau";
        option_info                   = "*) [Alt] + [Enter] wechselt jederzeit"
                                       " zwischen Fenster- und Vollbildmodus.";
        option_gfx_zero               = "**) Setze beides auf 0, um deine norm"
                                        "ale Desktop-Auflösung zu nutzen.";
        option_sound_load_driver      = "Audiotreiber laden*";
        option_sound_volume           = "Klanglautstärke";
        option_language               = "Sprache/Language";
        option_info_sound             = "*) Diese Einstellung gilt erst nach "
                                        "einem Programmneustart.";

        option_key_unassigned         = "nichts";
        option_key_force_left         = "Nur links";
        option_key_force_right        = "Nur rechts";
        option_key_scroll             = "Scrollen";
        option_key_priority           = "Priorität inv.";
        option_key_rate_minus         = "Intervall +";
        option_key_rate_plus          = "Intervall -";
        option_key_pause              = "Pause";
        option_key_speed_back_one     = "-1 Bild";
        option_key_speed_back_many    = "-1 Sekunde";
        option_key_speed_ahead_one    = "+1 Bild";
        option_key_speed_ahead_many   = "+1 Sekunde";
        option_key_speed_fast         = "Schneller";
        option_key_speed_turbo        = "Turbo";
        option_key_restart            = "Neustart";
        option_key_state_load         = "Stand laden";
        option_key_state_save         = "Stand spei.";
        option_key_zoom               = "Zoom";
        option_key_chat               = "Chat";
        option_key_spec_tribe         = "Team ansehen";
        option_key_nuke               = "Atombombe";

        option_sorted_skills          = "Klicke zwei Bilder an, "
                                        "um ihre Reihenfolge in der "
                                        "Spielleiste zu tauschen.";

        option_key_me_okay            = "Okay/Ja";
        option_key_me_edit            = "Editieren";
        option_key_me_export          = "Exportieren";
        option_key_me_delete          = "Lösch./Nein";
        option_key_me_up_dir          = "Ordner ..";
        option_key_me_up_1            = "Rauf um 1";
        option_key_me_up_5            = "Rauf um 5";
        option_key_me_down_1          = "Runter um 1";
        option_key_me_down_5          = "Runter um 5";
        option_key_me_exit            = "Zurück/Abbr.";

        option_ed_left                = "Nach links";
        option_ed_right               = "Nach rechts";
        option_ed_up                  = "Nach oben";
        option_ed_down                = "Nach unten";
        option_ed_copy                = "Kopieren";
        option_ed_delete              = "Löschen";
        option_ed_grid                = "Raster";
        option_ed_sel_all             = "Alles ausw.";
        option_ed_sel_frame           = "Rahmen";
        option_ed_sel_add             = "Mehr ausw.";
        option_ed_foreground          = "Nach vorn";
        option_ed_background          = "Nach hinten";
        option_ed_mirror              = "Spiegeln";
        option_ed_rotate              = "Drehen";
        option_ed_dark                = "Schwarz";
        option_ed_noow                = "Ni. überschr.";
        option_ed_zoom                = "Zoom";
        option_ed_help                = "Hilfe zeigen";
        option_ed_menu_size           = "Levelgröße";
        option_ed_menu_vars           = "Allg. Menü";
        option_ed_menu_skills         = "Fähigkeiten";
        option_ed_add_terrain         = "Terrain hinzuf.";
        option_ed_add_steel           = "Stahl hinzuf.";
        option_ed_add_hatch           = "Klappe hinzuf.";
        option_ed_add_goal            = "Ziel hinzuf.";
        option_ed_add_deco            = "Deko hinzuf.";
        option_ed_add_hazard          = "Gefahr hinzuf.";
        option_ed_grid_custom         = "Benutzerdef. Rastergröße";

        // Neuen Spieler begruessen (Mini-Dialog)
        option_new_player_title       = "Lix";
        option_new_player_first       = "Hallo und Willkommen!";
        option_new_player_second      = "Bitte gib deinen Namen ein:";

// Editorhilfe
eb[Editor::FILE_NEW]     = "Neu: Karte leeren und einen ganz neuen Level bauen.";
eb[Editor::FILE_EXIT]    = "Beenden: Editor verlassen, auf Wunsch Änderungen speichern.";
eb[Editor::FILE_SAVE]    = "Speichern: Level unter dem bisherigen Dateinamen abspeichern.";
eb[Editor::FILE_SAVE_AS] = "Speichern unter: Level unter einem neuen Dateinamen abspeichern.";
eb[Editor::GRID_2]       = "Raster 2: Objekt-Koordinaten beim Umherziehen auf 2er runden.";
eb[Editor::GRID_CUSTOM]  = "Benutzerdef. Raster: In den Hauptmenü-Optionen verstellbar.";
eb[Editor::GRID_16]      = "Raster 16: Objekt-Koordinaten beim Umherziehen auf 16er runden.";
eb[Editor::SELECT_ALL]   = "Alles: Alle Objekte auf dem Bildschirm in die Auswahl setzen.";
eb[Editor::SELECT_FRAME] = "Rahmen: Mehrfachauswahl von Objekten durch Rahmenziehen.";
eb[Editor::SELECT_ADD]   = "Hinzufügen: Neue Objekte auswählen, ohne alte abzuwählen.";
eb[Editor::SELECT_COPY]  = "Kopieren: Ausgewählte Objekte klonen.";
eb[Editor::SELECT_DELETE]= "Löschen: Ausgewählte Objekte löschen.";
eb[Editor::SELECT_MINUS] = "Hinter: Ausgewählte Objekte hinter einige andere setzen.";
eb[Editor::SELECT_PLUS]  = "Hervor: Ausgewählte Objekte vor einige andere holen.";
eb[Editor::SELECT_BACK]  = "Hintergrund: Ausgewählte Objekte ganz nach hinten setzen.";
eb[Editor::SELECT_FRONT] = "Vordergrund: Ausgewählte Objekte ganz nach vorn holen.";
eb[Editor::SELECT_FLIP]  = "Spiegeln: Ausgewähltes Terrain horizontal spiegeln.";
eb[Editor::SELECT_ROTATE]= "Drehen: Ausgewähltes Terrain um ein Viertel drehen.";
eb[Editor::SELECT_DARK]  = "Dunkel: Ausgewähltes Terrain im Spiel schwarz zeichnen.";
eb[Editor::SELECT_NOOW]  = "Nicht überschreiben: Ausgew. Ter. überschr. nur dunkles Ter.";
eb[Editor::VIEW_ZOOM]    = "Zoom: Vergrößerung der Karte ein- oder ausschalten.";
eb[Editor::HELP]         = "(dies macht derzeit nichts)";
eb[Editor::MENU_SIZE]    = "Abmessungen: Festlegen der Größe und Topologie des Levels.";
eb[Editor::MENU_SCROLL]  = "Aussehen: Festlegen der Startposition und Hintergrundfarbe.";
eb[Editor::MENU_VARS]    = "Variablen: Festlegen grundsätzlicher Level-Variablen.";
eb[Editor::MENU_SKILL]   = "Fähigkeiten: Festlegen der nutzbaren Lix-Fähigkeiten.";
eb[Editor::ADD_TERRAIN]  = "Terrain: Ein durchgrabbares Terrain-Object platzieren.";
eb[Editor::ADD_STEEL]    = "Stahl: Ein nicht durchgrabbares Stahlobjekt platzieren.";
eb[Editor::ADD_HATCH]    = "Klappe: Eine Lix-Eingangsklappe platzieren.";
eb[Editor::ADD_GOAL]     = "Ziel: Ein Ziel, also einen Lix-Ausgang platzieren.";
eb[Editor::ADD_DECO]     = "Dekoration: Ein harmloses Dekorationsobjekt platzieren.";
eb[Editor::ADD_HAZARD]   = "Gefahr: Eine Falle, Wasser oder Feuer platzieren.";

        break;
    }
    // Switch

    if (CUSTOM == useR->language) {
        // false in argument means load the full set of translations.
        // We are intentionally ignoring return value--even if it
        // completely fails to load, let's keep user on their selected
        // custom language anyway; they can fix it by updating their
        // translate.txt file later and there's no reason to force
        // them to have to change the language setting back to their
        // custom language afterwards.  Since we always have
        // fallback to English for anything missing, the UI is
        // never completely empty of text.
        static_cast<void>(try_load_custom_language(false));
    }

    if (::exists(gloB->file_translations_dump.get_rootful().c_str())) {
        write_translations_dump_for_current_language();
    }
}
// Language::set()

bool Language::try_load_custom_language(bool load_language_name_only) {
    std::vector <IO::Line> lines;
    IO::fill_vector_from_file(lines, gloB->file_translations.get_rootful());

    bool has_at_least_one_translation_loaded = false;
    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) {
        switch(i->type) {
        case '$':
            {
                std::string const &varname = i->text1;
                std::string const &value_in_line = i->text2;

                if (!load_language_name_only ||
                    varname == special_varname_for_language) {

                    translate_map_type::const_iterator map_entry = map_vars_to_translate.find(varname);
                    if (map_entry != map_vars_to_translate.end()) {

                        std::string &actualStringVariable = *(map_entry->second);
                        std::string::size_type const value_in_line_length = value_in_line.size();
                        // translation text in file must start and end with " character,
                        // which are stripped to get the actual text to use
                        if (value_in_line_length >= 2 &&
                            '\"' == value_in_line[0] &&
                            '\"' == value_in_line[value_in_line_length - 1]) {

                            actualStringVariable =
                                value_in_line.substr(1, value_in_line_length - 2);
                            has_at_least_one_translation_loaded = true;
                        } else {
                            // TODO: log a warning? translator probably
                            // forgot the quotes?
                        }
                    } else {
                        // TODO: log a warning maybe? translator might have
                        // misspelled name of variable?
                    }
                }
            }
            break;

        default:
            // TODO: maybe log a warning? although translator might've
            // purposely put an inert comment in the file
            ;
        } // switch

        if (load_language_name_only && has_at_least_one_translation_loaded) {
            break;
        }
    } // for

    return has_at_least_one_translation_loaded;
}

// this type should've stayed inside write_translate_file_for_current_language(),
// but doing so apparently does not work with template instantiation for
// std::sort inside that function :(
namespace Language {
    class KeyComparer {
    public:
        inline bool operator()(translate_map_type::const_iterator const& entry1,
                               translate_map_type::const_iterator const& entry2) {
            return entry1->first < entry2->first;
        }
    };
}

void Language::write_translations_dump_for_current_language() {
    // In future, the underlying map type may change to one that doesn't
    // guarantee an iteration order (eg. hash-table based).  But for
    // usability we want to write the lines out in sorted order of
    // variable names (ie. map keys) no matter what.  So we'll do some
    // extra sorting work here even if it seems redundant in current
    // implementation.

    std::string::size_type max_variable_length_seen = 0;
    typedef translate_map_type::const_iterator map_entry_iter_type;
    typedef std::vector<map_entry_iter_type> entries_vector_type;
    entries_vector_type entries(map_vars_to_translate.size());
    map_entry_iter_type mapIt = map_vars_to_translate.begin();
    for(int i = 0; mapIt != map_vars_to_translate.end(); ++mapIt, ++i) {
        entries[i] = mapIt;
        max_variable_length_seen = std::max(max_variable_length_seen,
                                            mapIt->first.size());
    }

    std::sort(entries.begin(), entries.end(), KeyComparer());

    class {
    public:
        inline void operator()(std::ofstream& file,
                               std::string::size_type const max_variable_length_seen,
                               std::string const& varname,
                               std::string const& translated_text) {
            std::string varname_padded = varname;
            for (std::string::size_type i = varname.size();
                 i < max_variable_length_seen;
                 i++) {
                varname_padded += " ";
            }

            std::string translated_text_quoted = "\"";
            translated_text_quoted += translated_text;
            translated_text_quoted += "\"";

            file << IO::LineDollar(varname_padded, translated_text_quoted);
        }
    } writeLine;

    std::ofstream file(gloB->file_translations_dump.get_rootful().c_str());
    // Some text editors like Windows Notepad may rely on having a
    // Unicode BOM character add to start of text file to reliably
    // determine file is UTF-8.  So we'll insert one here for benefit
    // of translators.  See http://en.wikipedia.org/wiki/Byte_order_mark
    file << Help::make_utf8_seq(Help::utf8_bom)
         << std::endl;

    // special case: always write out special_varname_for_language first,
    // and use the name of the user's current language
    writeLine(file, max_variable_length_seen,
              std::string(special_varname_for_language), language_name[useR->language]);
    file << std::endl;

    for(entries_vector_type::const_iterator entryIt = entries.begin();
        entryIt != entries.end();
        ++entryIt) {

        map_entry_iter_type map_entry = *entryIt;
        if (map_entry->first != special_varname_for_language) {

            writeLine(file, max_variable_length_seen,
                      map_entry->first, *(map_entry->second));
        }
    }

    file.close();
}
