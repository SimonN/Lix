/*
 * language.h
 *
 * Diese Datei enthaelt alle Stringvariablen, die waehrend des Spiels
 * ausgelesen werden. Beim Start des Programms oder beim Wechseln der Sprache
 * werden die Variablen durch die hier deklarierten Funktionen neu beschrieben.
 *
 */

#pragma once

#include <string>
#include <vector>

namespace Language {

enum Language {
    NONE,
    ENGLISH,
    GERMAN,
    MAX
};

// Laden und abrufen
void     set(const Language);
void     set(const unsigned);
Language get();

// language_name[GERMAN] ist etwa "Deutsch", bei ENGLISH "English".
extern std::vector <std::string> language_name;
extern std::vector <std::string> editor_button;
extern std::vector <std::string> option_group;

extern std::string

// Grundlegende Dinge
main_name_of_the_game,
main_loading_1,
main_loading_2,
main_version,
main_website,

// Logging
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
log_level_file_saved,

// Verschiedene Dialoge
ok, cancel, yes, no, back, exit,
dir_parent,
dir_flip_page,

// Browser
browser_single_title,
browser_network_title,
browser_replay_title,
browser_play,
browser_edit,
browser_replay,
browser_delete,
browser_extract,

browser_info_lems,
browser_info_required,
browser_info_clock_1,
browser_info_hatches,
browser_info_goals,
browser_info_skills,
browser_info_clock_2,
browser_info_clock_none,

browser_info_result_saved,
browser_info_result_skills,
browser_info_result_time,
browser_info_result_old_1,
browser_info_result_old_2,

browser_info_player,
browser_info_version,
browser_info_built,
browser_info_new,
browser_info_same,
browser_info_old,
browser_info_holds_level,

browser_box_delete_title,
browser_box_delete_question,

// Lobby
win_lobby_title,
win_lobby_title_lobby,
win_lobby_title_room,
win_lobby_exit,
win_lobby_unstable_central,
win_lobby_start_central,
win_lobby_start_server,
win_lobby_start_client,
win_lobby_chat,
win_lobby_select_level,
win_lobby_ready,
win_lobby_room_number,
win_lobby_room_player,
win_lobby_room_players,
win_lobby_room_create,
win_lobby_room_leave,

// Spielfenster
win_game_title,
win_game_result,
win_game_lems_saved,
win_game_resume,
win_game_restart,
win_game_save_replay,
win_game_menu,
win_game_comment_perfect,
win_game_comment_more,
win_game_comment_exactly,
win_game_comment_less,
win_game_comment_none,
win_game_result_skills,
win_game_result_time,
win_game_net_first,
win_game_net_second,
win_game_net_middle,
win_game_net_last,
win_game_net_first_tie,
win_game_net_last_tie,
win_game_net_all_tie,
win_game_net_zero,
win_game_net_all_zero,
win_game_replay_win_one,
win_game_replay_win_team,
win_game_replay_tie,
win_game_overwrite_title,
win_game_overwrite_question,

// Editor allgemein
editor_unsaved_title,
editor_unsaved_title_new,
editor_unsaved_question,
editor_unsaved_question_new,
editor_file_name,
editor_level_name,
editor_hotkey,
editor_hotkey_hold,

// SaveBrowser
save_browser_title,
save_filename,
save_box_overwrite_title,
save_box_overwrite_question,

// BitmapBrowser
add_terrain,
add_steel,
add_hatch,
add_goal,
add_deco,
add_hazard,

// Kartengroesse
win_size_title,
win_size_l,
win_size_r,
win_size_u,
win_size_d,
win_size_x,
win_size_y,
win_size_hex,
win_size_torus_x,
win_size_torus_y,

// Karten-Startausschnitt
win_scroll_title,
win_scroll_desc,
win_scroll_x,
win_scroll_y,
win_scroll_r,
win_scroll_g,
win_scroll_b,
win_scroll_jump,
win_scroll_current,

// Variablenfenster
win_var_title,
win_var_author,
win_var_name_german,
win_var_name_english,
win_var_lems,
win_var_required,
win_var_rate,
win_var_clock,

// Faehigkeits-Fenster
win_skill_title,
win_skill_clear,
win_skill_classic_8,
win_skill_classic_12,
win_skill_all_to,

// Netzwerk-Chatnachrichten
net_chat_welcome_1,
net_chat_welcome_2,
net_chat_unstable_1,
net_chat_unstable_2,
net_chat_start_server,
net_chat_start_client,
net_chat_start_cancel,
net_chat_disconnection,
net_chat_we_too_old,
net_chat_we_too_new,
net_chat_someone_old,
net_chat_someone_new,
net_chat_named_guy_old,
net_chat_server_update,
net_chat_version_yours,
net_chat_version_server,
net_chat_please_download,
net_chat_we_connected,
net_chat_we_in_room,
net_chat_we_in_room_2,
net_chat_we_in_lobby,
net_chat_player_in_room,
net_chat_player_in_room_2,
net_chat_player_in_lobby,
net_chat_player_out_room,
net_chat_player_out_room_2,
net_chat_player_out_lobby,
net_chat_level_change,
net_game_start,
net_game_how_to_chat_1,
net_game_how_to_chat_2,
net_game_end,
net_game_end_result,
net_game_overtime_1,
net_game_overtime_2,
net_game_overtime_2_one,
net_game_overtime_3,
net_game_overtime_4,
net_game_overtime_4_one,
net_game_overtime_4_sec,

// Optionsfenster
option_title,
option_user_name,
option_user_name_ask,
option_language,
option_replay_auto_max,
option_replay_auto_single,
option_replay_auto_multi,
option_mouse_speed,
option_scroll_speed_edge,
option_scroll_speed_click,
option_scroll_edge,
option_scroll_right,
option_scroll_middle,
option_scroll_torus_x,
option_scroll_torus_y,
option_scroll_torus_never,
option_scroll_torus_big,
option_scroll_torus_always,
option_multiple_builders,
option_batter_priority,
option_prioinv_middle,
option_prioinv_right,
option_screen_resolution,
option_screen_windowed_res,
option_screen_windowed,
option_screen_scaling,
option_screen_scaling_stretch,
option_screen_scaling_aspect,
option_screen_scaling_integer,
option_screen_border_colored,
option_screen_vsync,
option_arrows_replay,
option_arrows_network,
option_debris_amount,
option_debris_amount_none,
option_debris_amount_own,
option_debris_amount_all,
option_debris_type,
option_debris_type_stars,
option_debris_type_pixels,
option_gui_color_red,
option_gui_color_green,
option_gui_color_blue,
option_info,
option_gfx_zero,
option_sound_load_driver,
option_sound_volume,
option_info_sound,

option_key_unassigned,
option_key_force_left,
option_key_force_right,
option_key_rate_minus,
option_key_rate_plus,
option_key_pause,
option_key_speed_slow,
option_key_speed_fast,
option_key_speed_turbo,
option_key_restart,
option_key_state_load,
option_key_state_save,
option_key_zoom,
option_key_chat,
option_key_spec_tribe,
option_key_nuke,
option_key_info_1,
option_key_info_2,
option_key_info_3,

option_ed_left,
option_ed_right,
option_ed_up,
option_ed_down,
option_ed_copy,
option_ed_delete,
option_ed_grid,
option_ed_sel_all,
option_ed_sel_frame,
option_ed_sel_add,
option_ed_foreground,
option_ed_background,
option_ed_mirror,
option_ed_rotate,
option_ed_dark,
option_ed_noow,
option_ed_zoom,
option_ed_help,
option_ed_menu_size,
option_ed_menu_vars,
option_ed_menu_skills,
option_ed_add_terrain,
option_ed_add_steel,
option_ed_add_hatch,
option_ed_add_goal,
option_ed_add_deco,
option_ed_add_hazard,
option_ed_grid_custom,

// Neuen Spieler begruessen (Mini-Dialog)
option_new_player_title,
option_new_player_first,
option_new_player_second,

last_string;

}
