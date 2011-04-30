/*
 * language.cpp
 *
 */

#include "language.h"
#include "user.h"

#include "../editor/editor.h"

// Alles nochmal deklarieren, weil es im Header als extern markiert ist
namespace Language {

// Geheime Variable
std::vector <std::string> language_name( 4);
std::vector <std::string> option_group ( 6);
std::vector <std::string> editor_button(Editor::MAX);
std::vector <std::string>& eb = editor_button; // nur intern in language.cpp

std::string

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
win_lobby_exit,
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
win_size_torus_x,
win_size_torus_y,

// Karten-Startausschnitt
win_scroll_title,
win_scroll_x,
win_scroll_y,
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
net_chat_start_server,
net_chat_start_client,
net_chat_start_cancel,
net_chat_disconnection,
net_chat_we_too_old,
net_chat_we_too_new,
net_chat_someone_old,
net_chat_someone_new,
net_chat_named_guy_old,
net_chat_version_yours,
net_chat_version_server,
net_chat_server_update,
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
option_key_nuke,
option_key_chat,
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

// Neuen Spieler begruessen (Mini-Dialog)
option_new_player_title,
option_new_player_first,
option_new_player_second,

last_string;

}


// Rueckgabewert   Funktion
Language::Language Language::get()
{
    return static_cast <Language::Language> (useR->language);
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
    language_name[MAX]     = "Max";

    if (lang < MAX) useR->language = lang;

    switch (useR->language) {

    case NONE:
        // Grundlegende Dinge
        main_name_of_the_game         = "Lix";
        main_loading_1                = "---------- Loading Lix ----------";
        main_loading_2                = "---------- Please Wait ----------";
        main_version                  = "Version:";
        main_website                  = "lplusplus.co.cc";
        break;

    case MAX:
        break; // Sollte nicht vorkommen, es wird bei Spielstart gefragt

    case ENGLISH:

        // Grundlegende Dinge
        main_name_of_the_game         = "Lix";
        main_loading_1                = "---------- Loading Lix ----------";
        main_loading_2                = "---------- Please Wait ----------";
        main_version                  = "Version:";
        main_website                  = "lplusplus.co.cc";

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
        ok                            = "Okay";
        cancel                        = "Cancel";
        yes                           = "Yes";
        no                            = "No";
        back                          = "Back";
        exit                          = "Exit";
        dir_parent                    = "..",
        dir_flip_page                 = "(more...)",

        // Browser
        browser_single_title          = "Single Player";
        browser_network_title         = "Select Multiplayer level";
        browser_replay_title          = "Replays";
        browser_play                  = "Play";
        browser_edit                  = "Edit";
        browser_replay                = "Replay";
        browser_delete                = "Delete";
        browser_extract               = "Extract level";

        browser_info_lems             = "Lix:";
        browser_info_required         = "To save:";
        browser_info_clock_1          = "Time:";
        browser_info_hatches          = "Hatches:";
        browser_info_goals            = "Goals:";
        browser_info_skills           = "Skills:";
        browser_info_clock_2          = "Overtime:";
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

        browser_box_delete_title      = "Delete replay?";
        browser_box_delete_question   = "Do you really want to delete this replay?";

        // Lobby
        win_lobby_title               = "Network Game";
        win_lobby_exit                = "Leave network";
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
        win_game_lems_saved           = "Lix saved:";
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
        win_game_overwrite_title      = "Overwrite Replay?";
        win_game_overwrite_question   = "Do you really want to overwrite this replay?";

        // Editor allgemein
        editor_unsaved_title          = "Save changes?";
        editor_unsaved_title_new      = "Save level?";
        editor_unsaved_question       = "Would you like to save the changes on this level?";
        editor_unsaved_question_new   = "Would you like to save this new level?";
        editor_file_name              = "File name:";
        editor_level_name             = "Level name:";
        editor_hotkey                 = "Hotkey:";
        editor_hotkey_hold            = "Hotkey: hold";

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
        win_size_torus_x              = "Wrap around horizontally";
        win_size_torus_y              = "Wrap around vertically";

        // Scrollposition
        win_scroll_title              = "Initial Scrolling Position";
        win_scroll_x                  = "X Coordinate (0 = left)";
        win_scroll_y                  = "Y Coordinate (0 = top)";
        win_scroll_jump               = "Scroll there";
        win_scroll_current            = "Current Position";

        // Variablenfenster
        win_var_title                 = "General Level Settings";
        win_var_author                = "Author";
        win_var_name_german           = "German";
        win_var_name_english          = "English";
        win_var_lems                  = "No. of lix";
        win_var_required              = "To save";
        win_var_rate                  = "Release rate";
        win_var_clock                 = "Time";

        // Faehigkeits-Fenster
        win_skill_title               = "Skill Settings";
        win_skill_clear               = "Clear all";
        win_skill_classic_8           = "Classic 8";
        win_skill_classic_12          = "Modern 12";
        win_skill_all_to              = "Adjust all:";

        // Netzwerk-Chatnachrichten
        net_chat_welcome_1            = "You can join the central server, "
                                        "host a game by yourself,";
        net_chat_welcome_2            = "or enter an IP address/a host name "
                                        "and click \"" + win_lobby_start_client + "\".";
        net_chat_start_server         = "You are the server. Other "
                                        "players can connect to you now.";
        net_chat_start_client         = "Searching a server at";
        net_chat_start_cancel         = "Connection attempt cancelled.";
        net_chat_disconnection        = " has quit the network.";
        net_chat_we_too_old           = "You have too old an Lix version "
                                        "to connect to the server.";
        net_chat_we_too_new           = "You have too new an Lix version. "
                                        "The server should upgrade.";
        net_chat_someone_old          = "Someone with too old an Lix version "
                                        "tried to connect.";
        net_chat_someone_new          = "Someone with too new an Lix version "
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
        option_group[4]               = "Graphics";
        option_group[5]               = "Sound";
        option_user_name              = "Player name";
        option_user_name_ask          = "Ask for name on startup";
        option_language               = "Language";
        option_scroll_edge            = "Scroll at screen border";
        option_scroll_right           = "Scroll with right click";
        option_scroll_middle          = "Scroll with middle click";
        option_mouse_speed            = "Mouse speed";
        option_scroll_speed_edge      = "Edge scroll speed";
        option_scroll_speed_click     = "Right/mid. scroll spd.";
        option_scroll_torus_x         = "Torus scrolling (horz.)";
        option_scroll_torus_y         = "Torus scrolling (vert.)";
        option_scroll_torus_never     = "Never";
        option_scroll_torus_big       = "On big maps";
        option_scroll_torus_always    = "Always";
        option_multiple_builders      = "Builders: mult. clicks";
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
        option_debris_amount          = "Debris amount";
        option_debris_amount_none     = "None";
        option_debris_amount_own      = "Own";
        option_debris_amount_all      = "All";
        option_debris_type            = "Debris type";
        option_debris_type_stars      = "Stars, clouds";
        option_debris_type_pixels     = "Pixels";
        option_info                   = "*) Hit [Alt] + [Enter] to toggle full"
                                        "screen/windowed mode at any time.";
        option_gfx_zero               = "**) Enter 0 in both fields to use "
                                        "your normal desktop resolution.";
        option_sound_load_driver      = "Load sound driver*";
        option_sound_volume           = "Sound volume";
        option_info_sound             = "*) This option requires a program "
                                        "restart to tkae effect.";

        option_key_unassigned         = "none";
        option_key_force_left         = "Force left";
        option_key_force_right        = "Force right";
        option_key_rate_minus         = "Rate down";
        option_key_rate_plus          = "Rate up";
        option_key_pause              = "Pause";
        option_key_speed_slow         = "Slow motion";
        option_key_speed_fast         = "Fast forw.";
        option_key_speed_turbo        = "Turbo speed";
        option_key_restart            = "Restart";
        option_key_state_load         = "Load state";
        option_key_state_save         = "Save state";
        option_key_zoom               = "Zoom";
        option_key_nuke               = "Nuke";
        option_key_chat               = "Chat";
        option_key_info_1             = "A hotkey assigned to";
        option_key_info_2             = "multiple skills will";
        option_key_info_3             = "alternate between them.";

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
eb[Editor::GRID_10]
 = "Grid 10: Rounds object coordinates to multiples of 10.";
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
 = "Flip: Mirrors the selected terrain vertically.";
eb[Editor::SELECT_ROTATE]
 = "Rotate: Performs a quarter turn on the selected terrain.";
eb[Editor::SELECT_DARK]
= "Dark: Paints the selected terrain in black inside the game.";
eb[Editor::SELECT_NOOW]
= "No overwrite: Selected terrain overwrites only dark objects.";
eb[Editor::VIEW_ZOOM]
= "Zoom: Activate or deactivate the map zoom.";
eb[Editor::HELP]
 = "Help: Activates/deactivates these help texts for buttons.";
eb[Editor::MENU_SIZE]
 = "Measures: Set the size and the topology for the level.";
eb[Editor::MENU_SCROLL]
 = "Scrolling position: Set the Single Player scrolling position.";
eb[Editor::MENU_LEMMING]
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
        main_name_of_the_game         = "Lix";
        main_loading_1                = "------- Lix wird geladen -------";
        main_loading_2                = "--------- Bitte warten ---------";
        main_version                  = "Version:";
        main_website                  = "lplusplus.co.cc";

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
        ok                            = "Okay";
        cancel                        = "Abbrechen";
        yes                           = "Ja";
        no                            = "Nein";
        back                          = "Zurueck";
        exit                          = "Beenden";
        dir_parent                    = "..",
        dir_flip_page                 = "(weitere...)",

        // Browser
        browser_single_title          = "Einzelspieler";
        browser_network_title         = "Netzwerk-Level aussuchen";
        browser_replay_title          = "Replays";
        browser_play                  = "Spielen";
        browser_edit                  = "Editieren";
        browser_replay                = "Abspielen";
        browser_delete                = "Loeschen";
        browser_extract               = "Level extrah.";

        browser_info_lems             = "Anzahl Lix:";
        browser_info_required         = "Zu retten:";
        browser_info_clock_1          = "Zeit:";
        browser_info_hatches          = "Klappen:";
        browser_info_goals            = "Ausgaenge:";
        browser_info_skills           = "Faehigktn.:";
        browser_info_clock_2          = "Nachzeit:";
        browser_info_clock_none       = "--";

        browser_info_result_saved     = "Gerettet:";
        browser_info_result_skills    = "Faehigktn.:";
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

        browser_box_delete_title      = "Replay loeschen?";
        browser_box_delete_question   = "Soll dieses Replay wirklich geloescht werden?";

        // Lobby
        win_lobby_title               = "Netzwerkspiel";
        win_lobby_exit                = "Netz verlassen";
        win_lobby_start_central       = "Verbinden zu zentralem Server";
        win_lobby_start_server        = "Selbst ein Spiel hosten";
        win_lobby_start_client        = "Verbinden zu:";
        win_lobby_chat                = "Chat:";
        win_lobby_select_level        = "Level aussuchen";
        win_lobby_ready               = "Bereit zum Start";
        win_lobby_room_number         = "Raum Nr. ";
        win_lobby_room_player         = "Spieler";
        win_lobby_room_players        = "Spieler";
        win_lobby_room_create         = "Neuen Raum eroeffnen";
        win_lobby_room_leave          = "Raum verlassen";

        // Spielfenster
        win_game_title                = "Spielmenue";
        win_game_result               = "Ergebnis";
        win_game_lems_saved           = "Lix gerettet:";
        win_game_resume               = "Weiterspielen";
        win_game_restart              = "Level neustarten";
        win_game_save_replay          = "Replay speichern";
        win_game_menu                 = "Beenden";
        win_game_comment_perfect      = "Perfekt! Alle gerettet!";
        win_game_comment_more         = "Super, mehr als notwendig!";
        win_game_comment_exactly      = "Ziel haargenau erreicht!";
        win_game_comment_less         = "Das sind leider zu wenige.";
        win_game_comment_none         = "Das war wohl nix. Nochmal?";
        win_game_result_skills        = "Faehigkeiten:";
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
        win_game_overwrite_title      = "Replay ueberschreiben?";
        win_game_overwrite_question   = "Soll dieses Replay wirklich ueberschrieben werden?";

        // Editor allgemein
        editor_unsaved_title          = "Aenderungen speichern?";
        editor_unsaved_title_new      = "Level speichern?";
        editor_unsaved_question       = "Moechtest du die Aenderungen an diesem Level speichern?";
        editor_unsaved_question_new   = "Moechtest du diesen neuen Level speichern?";
        editor_file_name              = "Dateiname:";
        editor_level_name             = "Levelname:";
        editor_hotkey                 = "Hotkey:";
        editor_hotkey_hold            = "Hotkey: halte";

        // SaveBrowser
        save_browser_title            = "Speichern unter";
        save_filename                 = "Dateiname:";
        save_box_overwrite_title      = "Level ueberschreiben?";
        save_box_overwrite_question   = "Soll dieser Level wirklich ueberschrieben werden?";

        // BitmapBrowser
        add_terrain                   = "Terrain hinzufuegen";
        add_steel                     = "Stahl hinzufuegen";
        add_hatch                     = "Klappe hinzufuegen";
        add_goal                      = "Ziel hinzufuegen";
        add_deco                      = "Dekoration hinzufuegen";
        add_hazard                    = "Gefahr hinzufuegen";

        // Kartengroesse
        win_size_title                = "Level-Abmessungen";
        win_size_l                    = "Links:";
        win_size_r                    = "Rechts:";
        win_size_u                    = "Oben:";
        win_size_d                    = "Unten:";
        win_size_x                    = "Breite:";
        win_size_y                    = "Hoehe:";
        win_size_torus_x              = "Horizontal rundumgehend";
        win_size_torus_y              = "Vertikal rundumgehend";

        // Scrollposition
        win_scroll_title              = "Start-Scrollposition";
        win_scroll_x                  = "X-Koordinate (0 = links)";
        win_scroll_y                  = "Y-Koordinate (0 = oben)";
        win_scroll_jump               = "Dorthin scrollen";
        win_scroll_current            = "Aktuelle Position";

        // Variablenfenster
        win_var_title                 = "Allgemeine Leveleinstellungen";
        win_var_author                = "Autor";
        win_var_name_german           = "Deutsch";
        win_var_name_english          = "Englisch";
        win_var_lems                  = "Anzahl Lix";
        win_var_required              = "Zu retten";
        win_var_rate                  = "Fallrate";
        win_var_clock                 = "Zeit";

        // Faehigkeits-Fenster
        win_skill_title               = "Faehigkeits-Einstellungen";
        win_skill_clear               = "Alle loeschen";
        win_skill_classic_8           = "Alte 8";
        win_skill_classic_12          = "Moderne 12";
        win_skill_all_to              = "Alle aendern:";

        // Netzwerk-Chatnachrichten
        net_chat_welcome_1            = "Verbinde zum Zentralserver, eroeffne "
                                        "selbst ein Spiel oder gib";
        net_chat_welcome_2            = "eine IP-Adresse/einen Rechnernamen ein und waehle"
                                        " \"" + win_lobby_start_client + "\".";
        net_chat_start_server         = "Du bist der Server. Andere Spieler "
                                        "koennen zu dir verbinden.";
        net_chat_start_client         = "Suche Server unter";
        net_chat_start_cancel         = "Verbindungsaufbau abgebrochen.";
        net_chat_disconnection        = " hat das Netzwerk verlassen.";
        net_chat_we_too_old           = "Deine Lix-Version ist zu alt, "
                                        "um verbinden zu koennen.";
        net_chat_we_too_new           = "Deine Lix-Version ist zu neu,"
                                        "um verbinden zu koennen.";
        net_chat_someone_old          = "Jemand wollte verbinden, aber seine "
                                        "Lix-Version war zu alt.";
        net_chat_someone_new          = "Jemand wollte verbinden, aber seine "
                                        "Lix-Version war zu neu.";
        net_chat_named_guy_old        = " muss zunaechst seine Lix-Version "
                                        "aktualisieren.";
        net_chat_server_update        = "The server should always run the "
                                        "most recent Lix version.";
        net_chat_version_yours        = "Your version is ";
        net_chat_version_server       = ", the server has ";
        net_chat_please_download      = "Download the newest version from: "
                                      + main_website;
        net_chat_we_connected         = "Du hast das Netzwerk betreten. "
                                        "Waehle einen Raum zum Spielen.";
        net_chat_we_in_room           = "Du hast Raum Nr. ";
        net_chat_we_in_room_2         = " betreten.";
        net_chat_we_in_lobby          = "Du bist in die Lobby "
                                        "zurueckgegangen.";
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
        net_chat_level_change         = "hat diesen Level gewaehlt:";
        net_game_start                = "Das Spiel beginnt.";
        net_game_how_to_chat_1        = " Du kannst mit [";
        net_game_how_to_chat_2        = "] chatten.";
        net_game_end                  = "Das Spiel ist zu Ende.";
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
        option_group[1]               = "Steuerung";
        option_group[2]               = "Hotkeys";
        option_group[3]               = "Editor";
        option_group[4]               = "Grafik";
        option_group[5]               = "Audio";
        option_user_name              = "Spielername";
        option_user_name_ask          = "Namensfrage bei Start";
        option_scroll_edge            = "Scrollen am Bildrand";
        option_scroll_right           = "Scrollen bei Rechtsklick";
        option_scroll_middle          = "Scrollen bei mittl. Maust.";
        option_mouse_speed            = "Mausgeschwindigkeit";
        option_scroll_speed_edge      = "Scrollgeschw. am Rand";
        option_scroll_speed_click     = "Scrollgeschw. bei Klick";
        option_scroll_torus_x         = "Torus-Scrollen (horz.)";
        option_scroll_torus_y         = "Torus-Scrollen (vert.)";
        option_scroll_torus_never     = "Niemals";
        option_scroll_torus_big       = "Grosse Level";
        option_scroll_torus_always    = "Immer";
        option_multiple_builders      = "Builder mehrm. klickbar";
        option_screen_windowed        = "Fenstermodus*";
        option_screen_resolution      = "Vollbild-Aufloesung**";
        option_screen_windowed_res    = "Fenster-Aufloesung";
        option_screen_scaling         = "Vergroesserung";
        option_screen_scaling_stretch = "Gestreckt";
        option_screen_scaling_aspect  = "Seitenverhaeltnis";
        option_screen_scaling_integer = "Ganzzahlig";
        option_screen_border_colored  = "Bildschirmrand farbig";
        option_screen_vsync           = "V-Sync abwarten";
        option_arrows_replay          = "Pfeile im Replay";
        option_arrows_network         = "Pfeile im Netzwerkspiel";
        option_debris_amount          = "Truemmer-Anzahl";
        option_debris_amount_none     = "Keine";
        option_debris_amount_own      = "Lokaler Spieler";
        option_debris_amount_all      = "Alle";
        option_debris_type            = "Truemmer-Aussehen";
        option_debris_type_stars      = "Sterne, Wolken";
        option_debris_type_pixels     = "Pixel";
        option_info                   = "*) [Alt] + [Enter] wechselt jederzeit"
                                       " zwischen Fenster- und Vollbildmodus.";
        option_gfx_zero               = "**) Setze beides auf 0, um deine norm"
                                        "ale Desktop-Aufloesung zu nutzen.";
        option_sound_load_driver      = "Audiotreiber laden*";
        option_sound_volume           = "Klanglautstaerke";
        option_language               = "Sprache/Language";
        option_info_sound             = "*) Diese Einstellung gilt erst nach "
                                        "einem Programmneustart.";

        option_key_unassigned         = "nichts";
        option_key_force_left         = "Nur links";
        option_key_force_right        = "Nur rechts";
        option_key_rate_minus         = "Rate runter";
        option_key_rate_plus          = "Rate rauf";
        option_key_pause              = "Pause";
        option_key_speed_slow         = "Zeitlupe";
        option_key_speed_fast         = "Schneller";
        option_key_speed_turbo        = "Turbo";
        option_key_restart            = "Neustart";
        option_key_state_load         = "Stand laden";
        option_key_state_save         = "Stand spei.";
        option_key_zoom               = "Zoom";
        option_key_nuke               = "Atombombe";
        option_key_chat               = "Chat";
        option_key_info_1             = "Ein mehrfach zugewiesener";
        option_key_info_2             = "Hotkey alterniert zwischen";
        option_key_info_3             = "seinen Faehigkeiten.";

        option_ed_left                = "Nach links";
        option_ed_right               = "Nach rechts";
        option_ed_up                  = "Nach oben";
        option_ed_down                = "Nach unten";
        option_ed_copy                = "Kopieren";
        option_ed_delete              = "Loeschen";
        option_ed_grid                = "Raster";
        option_ed_sel_all             = "Alles ausw.";
        option_ed_sel_frame           = "Rahmen";
        option_ed_sel_add             = "Mehr ausw.";
        option_ed_foreground          = "Nach vorn";
        option_ed_background          = "Nach hinten";
        option_ed_mirror              = "Spiegeln";
        option_ed_rotate              = "Drehen";
        option_ed_dark                = "Schwarz";
        option_ed_noow                = "Ni. ueberschr.";
        option_ed_zoom                = "Zoom";
        option_ed_help                = "Hilfe zeigen";
        option_ed_menu_size           = "Levelgroesse";
        option_ed_menu_vars           = "Allg. Menue";
        option_ed_menu_skills         = "Faehigkeiten";
        option_ed_add_terrain         = "Terrain hinzuf.";
        option_ed_add_steel           = "Stahl hinzuf.";
        option_ed_add_hatch           = "Klappe hinzuf.";
        option_ed_add_goal            = "Ziel hinzuf.";
        option_ed_add_deco            = "Deko hinzuf.";
        option_ed_add_hazard          = "Gefahr hinzuf.";

        // Neuen Spieler begruessen (Mini-Dialog)
        option_new_player_title       = "Lix";
        option_new_player_first       = "Hallo und Willkommen!";
        option_new_player_second      = "Bitte gib deinen Namen ein:";

// Editorhilfe
eb[Editor::FILE_NEW]     = "Neu: Karte leeren und einen ganz neuen Level bauen.";
eb[Editor::FILE_EXIT]    = "Beenden: Editor verlassen, auf Wunsch Aenderungen speichern.";
eb[Editor::FILE_SAVE]    = "Speichern: Level unter dem bisherigen Dateinamen abspeichern.";
eb[Editor::FILE_SAVE_AS] = "Speichern unter: Level unter einem neuen Dateinamen abspeichern.";
eb[Editor::GRID_2]       = "Raster 2: Objekt-Koordinaten beim Umherziehen auf 2er runden.";
eb[Editor::GRID_10]      = "Raster 10: Objekt-Koordinaten beim Umherziehen auf 10er runden.";
eb[Editor::GRID_16]      = "Raster 16: Objekt-Koordinaten beim Umherziehen auf 16er runden.";
eb[Editor::SELECT_ALL]   = "Alles: Alle Objekte auf dem Bildschirm in die Auswahl setzen.";
eb[Editor::SELECT_FRAME] = "Rahmen: Mehrfachauswahl von Objekten durch Rahmenziehen.";
eb[Editor::SELECT_ADD]   = "Hinzufuegen: Neue Objekte auswaehlen, ohne alte abzuwaehlen.";
eb[Editor::SELECT_COPY]  = "Kopieren: Ausgewaehlte Objekte klonen.";
eb[Editor::SELECT_DELETE]= "Loeschen: Ausgewaehlte Objekte loeschen.";
eb[Editor::SELECT_MINUS] = "Zurueck: Ausgewaehlte Objekte hinter einige andere setzen.";
eb[Editor::SELECT_PLUS]  = "Hervor: Ausgewaehlte Objekte vor einige andere holen.";
eb[Editor::SELECT_BACK]  = "Hintergrund: Ausgewaehlte Objekte ganz nach hinten setzen.";
eb[Editor::SELECT_FRONT] = "Vordergrund: Ausgewaehlte Objekte ganz nach vorn holen.";
eb[Editor::SELECT_FLIP]  = "Spiegeln: Ausgewaehltes Terrain vertikal kippen.";
eb[Editor::SELECT_ROTATE]= "Drehen: Ausgewaehltes Terrain um ein Viertel drehen.";
eb[Editor::SELECT_DARK]  = "Dunkel: Ausgewaehltes Terrain im Spiel schwarz zeichnen.";
eb[Editor::SELECT_NOOW]  = "Nicht ueberschreiben: Ausgew. Ter. ueberschr. nur dunkles Ter.";
eb[Editor::VIEW_ZOOM]    = "Zoom: Vergroesserung der Karte ein- oder ausschalten.";
eb[Editor::HELP]         = "Hilfe: Aktiviert/deaktiviert diese Button-Hilfstexte.";
eb[Editor::MENU_SIZE]    = "Abmessungen: Festlegen der Groesse und Topologie des Levels.";
eb[Editor::MENU_SCROLL]  = "Scrollposition: Festlegen der Einzelspieler-Scroll-Startposition";
eb[Editor::MENU_LEMMING] = "Variablen: Festlegen grundsaetzlicher Level-Variablen.";
eb[Editor::MENU_SKILL]   = "Faehigkeiten: Festlegen der nutzbaren Lix-Faehigkeiten.";
eb[Editor::ADD_TERRAIN]  = "Terrain: Ein durchgrabbares Terrain-Object platzieren.";
eb[Editor::ADD_STEEL]    = "Stahl: Ein nicht durchgrabbares Stahlobjekt platzieren.";
eb[Editor::ADD_HATCH]    = "Klappe: Eine Lix-Eingangsklappe platzieren.";
eb[Editor::ADD_GOAL]     = "Ziel: Ein Ziel, also einen Lix-Ausgang platzieren.";
eb[Editor::ADD_DECO]     = "Dekoration: Ein harmloses Dekorationsobjekt platzieren.";
eb[Editor::ADD_HAZARD]   = "Gefahr: Eine Falle, Wasser oder Feuer platzieren.";

        break;
    }
    // Switch
}
// Language::set()
