/*
 * menu/options.cpp
 *
 */

#include <sstream>

#include "options.h"

#include "../api/manager.h"
#include "../graphic/gra_lib.h" // destroy and reload everything in case
#include "../other/help.h"
#include "../other/language.h"
#include "../other/user.h"

namespace Api {

const unsigned OptionMenu::check_x   =  20;
const unsigned OptionMenu::check_nx  =  60;
const unsigned OptionMenu::other_x   = 300;
const unsigned OptionMenu::other_nx  = 460;

const unsigned OptionMenu::button_xl = 140;
const unsigned OptionMenu::key_xl    =  70;
const unsigned OptionMenu::frame_y   = 320;
const unsigned OptionMenu::frame_yl  = 100;

const unsigned OptionMenu::key_b1    =  20;
const unsigned OptionMenu::key_t1    =  40 + OptionMenu::key_xl;
const unsigned OptionMenu::key_b2    = 220;
const unsigned OptionMenu::key_t2    = 240 + OptionMenu::key_xl;
const unsigned OptionMenu::key_b3    = 420;
const unsigned OptionMenu::key_t3    = 440 + OptionMenu::key_xl;



void OptionMenu::add_skill_to_hotkey_dialogue(const LixEn::Ac& ac)
{
    KeyButton* tb = &key_skill[ac];
    tb->set_x(hotkey_add_counter < 10 ? key_b2 : key_b3);
    tb->set_y(100 + (hotkey_add_counter % 10) * 30);
    pointers[GROUP_HOTKEYS].push_back(tb);

    Label* l = &desc_key_skill[ac];
    l->set_x(tb->get_x() + tb->get_xl() + 20);
    l->set_y(tb->get_y());
    std::string s = LixEn::ac_to_string(ac);
    Help::string_to_nice_case(s);
    l->set_text(s);
    pointers[GROUP_HOTKEYS].push_back(l);

    ++hotkey_add_counter;
}



OptionMenu::OptionMenu()
:
    Window(0, 0, LEMSCR_X, LEMSCR_Y, Language::option_title),
    exit                  (false),
    hotkey_add_counter    (0),

    // GT is a define macro, see above this function
    button_okay           (LEMSCR_X/2 - 150, 440, button_xl),
    button_cancel         (LEMSCR_X/2 +  10, 440, button_xl),
    pointers              (GROUP_MAX),

    user_name             (other_x, 100, button_xl),
    user_name_ask         (check_x, 100),
    language              (other_x, 130, button_xl),
    language_nr           (useR->language),
    replay_auto_single    (check_x, 190),
    replay_auto_multi     (check_x, 220),
    replay_auto_max       (other_x, 190, button_xl, 3, 0, 500),
    desc_user_name        (other_nx, 100, Language::option_user_name),
    desc_user_name_ask    (check_nx, 100, Language::option_user_name_ask),
    desc_language         (other_nx, 130, Language::option_language),
    desc_replay_auto_single(check_nx,190, Language::option_replay_auto_single),
    desc_replay_auto_multi(check_nx, 220, Language::option_replay_auto_multi),
    desc_replay_auto_max  (other_nx, 190, Language::option_replay_auto_max),

    mouse_speed           (other_x, 100, button_xl, 2, 1, 40),
    scroll_speed_edge     (other_x, 130, button_xl, 2, 1, 40),
    scroll_speed_click    (other_x, 160, button_xl, 2, 1, 60),
    scroll_edge           (check_x, 100),
    scroll_right          (check_x, 130),
    scroll_middle         (check_x, 160),
    scroll_torus_x        (other_x, 220, button_xl),
    scroll_torus_y        (other_x, 250, button_xl),
    multiple_builders     (check_x, 220),
    batter_priority       (check_x, 250),
    prioinv_middle        (check_x, 280),
    prioinv_right         (check_x, 310),
    desc_mouse_speed      (other_nx, 100, Language::option_mouse_speed),
    desc_scroll_speed_edge(other_nx, 130, Language::option_scroll_speed_edge),
    desc_scroll_speed_click(other_nx,160, Language::option_scroll_speed_click),
    desc_scroll_edge      (check_nx, 100, Language::option_scroll_edge),
    desc_scroll_right     (check_nx, 130, Language::option_scroll_right),
    desc_scroll_middle    (check_nx, 160, Language::option_scroll_middle),
    desc_scroll_torus_x   (other_nx, 220, Language::option_scroll_torus_x),
    desc_scroll_torus_y   (other_nx, 250, Language::option_scroll_torus_y),
    desc_multiple_builders(check_nx, 220, Language::option_multiple_builders),
    desc_batter_priority  (check_nx, 250, Language::option_batter_priority),
    desc_prioinv_middle   (check_nx, 280, Language::option_prioinv_middle),
    desc_prioinv_right    (check_nx, 310, Language::option_prioinv_right),

    key_force_left        (key_b1,  100, key_xl),
    key_force_right       (key_b1,  120, key_xl),
    key_rate_minus        (key_b1,  150, key_xl),
    key_rate_plus         (key_b1,  170, key_xl),
    key_pause             (key_b1,  200, key_xl),
    key_speed_slow        (key_b1,  220, key_xl),
    key_speed_fast        (key_b1,  240, key_xl),
    key_speed_turbo       (key_b1,  260, key_xl),
    key_restart           (key_b1,  290, key_xl),
    key_state_load        (key_b1,  310, key_xl),
    key_state_save        (key_b1,  330, key_xl),
    key_zoom              (key_b1,  360, key_xl),
    key_chat              (key_b1,  380, key_xl),
    key_spec_tribe        (key_b1,  400, key_xl),
    key_nuke              (key_b3,  220, key_xl),
    // Some of the following buttons will be moved again.
    key_skill             (LixEn::AC_MAX, KeyButton(9999, 9999, key_xl)),
    desc_key_skill        (LixEn::AC_MAX, Label    (9999, 9999)),
    desc_key_force_left   (key_t1, 100, Language::option_key_force_left),
    desc_key_force_right  (key_t1, 120, Language::option_key_force_right),
    desc_key_rate_minus   (key_t1, 150, Language::option_key_rate_minus),
    desc_key_rate_plus    (key_t1, 170, Language::option_key_rate_plus),
    desc_key_pause        (key_t1, 200, Language::option_key_pause),
    desc_key_speed_slow   (key_t1, 220, Language::option_key_speed_slow),
    desc_key_speed_fast   (key_t1, 240, Language::option_key_speed_fast),
    desc_key_speed_turbo  (key_t1, 260, Language::option_key_speed_turbo),
    desc_key_restart      (key_t1, 290, Language::option_key_restart),
    desc_key_state_load   (key_t1, 310, Language::option_key_state_load),
    desc_key_state_save   (key_t1, 330, Language::option_key_state_save),
    desc_key_zoom         (key_t1, 360, Language::option_key_zoom),
    desc_key_chat         (key_t1, 380, Language::option_key_chat),
    desc_key_spec_tribe   (key_t1, 400, Language::option_key_spec_tribe),
    desc_key_nuke         (key_t3, 220, Language::option_key_nuke),
    desc_key_info_1       (key_b3, 330, Language::option_key_info_1),
    desc_key_info_2       (key_b3, 350, Language::option_key_info_2),
    desc_key_info_3       (key_b3, 370, Language::option_key_info_3),

    ed_left               (key_b1, 100, key_xl),
    ed_right              (key_b1, 120, key_xl),
    ed_up                 (key_b1, 140, key_xl),
    ed_down               (key_b1, 160, key_xl),
    ed_copy               (key_b1, 200, key_xl),
    ed_delete             (key_b1, 220, key_xl),
    ed_grid               (key_b1, 240, key_xl),
    ed_sel_all            (key_b2, 100, key_xl),
    ed_sel_frame          (key_b2, 120, key_xl),
    ed_sel_add            (key_b2, 140, key_xl),
    ed_background         (key_b2, 180, key_xl),
    ed_foreground         (key_b2, 200, key_xl),
    ed_mirror             (key_b2, 220, key_xl),
    ed_rotate             (key_b2, 240, key_xl),
    ed_dark               (key_b2, 260, key_xl),
    ed_noow               (key_b2, 280, key_xl),
    ed_zoom               (key_b1, 260, key_xl),
    ed_help               (key_b1, 280, key_xl),
    ed_menu_size          (key_b3, 100, key_xl),
    ed_menu_vars          (key_b3, 120, key_xl),
    ed_menu_skills        (key_b3, 140, key_xl),
    ed_add_terrain        (key_b3, 180, key_xl),
    ed_add_steel          (key_b3, 200, key_xl),
    ed_add_hatch          (key_b3, 220, key_xl),
    ed_add_goal           (key_b3, 240, key_xl),
    ed_add_deco           (key_b3, 260, key_xl),
    ed_add_hazard         (key_b3, 280, key_xl),
    ed_grid_custom        (key_b1, 320, button_xl, 2, 1, 64),
    desc_ed_left          (key_t1, 100, Language::option_ed_left),
    desc_ed_right         (key_t1, 120, Language::option_ed_right),
    desc_ed_up            (key_t1, 140, Language::option_ed_up),
    desc_ed_down          (key_t1, 160, Language::option_ed_down),
    desc_ed_copy          (key_t1, 200, Language::option_ed_copy),
    desc_ed_delete        (key_t1, 220, Language::option_ed_delete),
    desc_ed_grid          (key_t1, 240, Language::option_ed_grid),
    desc_ed_sel_all       (key_t2, 100, Language::option_ed_sel_all),
    desc_ed_sel_frame     (key_t2, 120, Language::option_ed_sel_frame),
    desc_ed_sel_add       (key_t2, 140, Language::option_ed_sel_add),
    desc_ed_background    (key_t2, 180, Language::option_ed_background),
    desc_ed_foreground    (key_t2, 200, Language::option_ed_foreground),
    desc_ed_mirror        (key_t2, 220, Language::option_ed_mirror),
    desc_ed_rotate        (key_t2, 240, Language::option_ed_rotate),
    desc_ed_dark          (key_t2, 260, Language::option_ed_dark),
    desc_ed_noow          (key_t2, 280, Language::option_ed_noow),
    desc_ed_zoom          (key_t1, 260, Language::option_ed_zoom),
    desc_ed_help          (key_t1, 280, Language::option_ed_help),
    desc_ed_menu_size     (key_t3, 100, Language::option_ed_menu_size),
    desc_ed_menu_vars     (key_t3, 120, Language::option_ed_menu_vars),
    desc_ed_menu_skills   (key_t3, 140, Language::option_ed_menu_skills),
    desc_ed_add_terrain   (key_t3, 180, Language::option_ed_add_terrain),
    desc_ed_add_steel     (key_t3, 200, Language::option_ed_add_steel),
    desc_ed_add_hatch     (key_t3, 220, Language::option_ed_add_hatch),
    desc_ed_add_goal      (key_t3, 240, Language::option_ed_add_goal),
    desc_ed_add_deco      (key_t3, 260, Language::option_ed_add_deco),
    desc_ed_add_hazard    (key_t3, 280, Language::option_ed_add_hazard),
    desc_ed_grid_custom   (key_b1 + button_xl + 20, 320, Language::option_ed_grid_custom),

    screen_resolution_x   (other_x, 100, button_xl/2),
    screen_resolution_y   (370,     100, button_xl/2),
    screen_windowed_x     (other_x, 130, button_xl/2),
    screen_windowed_y     (370,     130, button_xl/2),
    screen_windowed       (check_x, 130),
    screen_scaling        (other_x, 160, button_xl),
    screen_border_colored (check_x, 160),
    screen_vsync          (check_x, 100),
    arrows_replay         (check_x, 220),
    arrows_network        (check_x, 250),
    debris_amount         (other_x, 220, button_xl),
    debris_type           (other_x, 250, button_xl),
    gui_color_red         (other_x - 40, 280, button_xl + 40),
    gui_color_green       (other_x - 40, 310, button_xl + 40),
    gui_color_blue        (other_x - 40, 340, button_xl + 40),
    desc_screen_resolution(other_nx, 100, Language::option_screen_resolution),
    desc_screen_windowed_res(other_nx,130, Language::option_screen_windowed_res),
    desc_screen_windowed  (check_nx, 130, Language::option_screen_windowed),
    desc_screen_scaling   (other_nx, 160, Language::option_screen_scaling),
    desc_screen_border_colored(check_nx,160,Language::option_screen_border_colored),
    desc_screen_vsync     (check_nx, 100, Language::option_screen_vsync),
    desc_arrows_replay    (check_nx, 220, Language::option_arrows_replay),
    desc_arrows_network   (check_nx, 250, Language::option_arrows_network),
    desc_debris_amount    (other_nx, 220, Language::option_debris_amount),
    desc_debris_type      (other_nx, 250, Language::option_debris_type),
    desc_gui_color_red    (other_nx, 280, Language::option_gui_color_red),
    desc_gui_color_green  (other_nx, 310, Language::option_gui_color_green),
    desc_gui_color_blue   (other_nx, 340, Language::option_gui_color_blue),

    desc_option_gfx_zero  (20,       380, Language::option_gfx_zero),
    desc_option_gfx_info  (20,       400, Language::option_info),

    sound_load_driver     (check_x, 100),
    sound_volume          (other_x, 100, button_xl, 2, 0, 20),
    desc_sound_load_driver(check_nx, 100, Language::option_sound_load_driver),
    desc_sound_volume     (other_nx, 100, Language::option_sound_volume),
    desc_sound_info       (20,       400, Language::option_info_sound)
{
    // Standardwerte fuer alle Checkboxes und Buttons
    button_okay           .set_text(Language::ok);
    button_cancel         .set_text(Language::cancel);
    button_okay           .set_hotkey(KEY_ENTER);
    button_cancel         .set_hotkey(KEY_ESC);

    for (size_t i = 0; i < GROUP_MAX; ++i) {
        TextButton temp(i*100 + 20, 40, 100);
        temp.set_text(Language::option_group[i]);
        button_group.push_back(temp);
    }

    replay_auto_max       .set_step_sml( 5);
    replay_auto_max       .set_step_med(50);

    gui_color_red         .set_macro_color();
    gui_color_green       .set_macro_color();
    gui_color_blue        .set_macro_color();

    pointers[GROUP_GENERAL ].push_back(&user_name);
    pointers[GROUP_GENERAL ].push_back(&user_name_ask);
    pointers[GROUP_GENERAL ].push_back(&language);
    pointers[GROUP_GENERAL ].push_back(&replay_auto_single);
    pointers[GROUP_GENERAL ].push_back(&replay_auto_multi);
    pointers[GROUP_GENERAL ].push_back(&replay_auto_max);
    pointers[GROUP_GENERAL ].push_back(&desc_user_name);
    pointers[GROUP_GENERAL ].push_back(&desc_user_name_ask);
    pointers[GROUP_GENERAL ].push_back(&desc_language);
    pointers[GROUP_GENERAL ].push_back(&desc_replay_auto_single);
    pointers[GROUP_GENERAL ].push_back(&desc_replay_auto_multi);
    pointers[GROUP_GENERAL ].push_back(&desc_replay_auto_max);

    scroll_torus_x          .add_text   (Language::option_scroll_torus_never);
    scroll_torus_x          .add_text   (Language::option_scroll_torus_big);
    scroll_torus_x          .add_text   (Language::option_scroll_torus_always);
    scroll_torus_y          .add_text   (Language::option_scroll_torus_never);
    scroll_torus_y          .add_text   (Language::option_scroll_torus_big);
    scroll_torus_y          .add_text   (Language::option_scroll_torus_always);
    pointers[GROUP_CONTROLS].push_back(&scroll_edge);
    pointers[GROUP_CONTROLS].push_back(&scroll_right);
    pointers[GROUP_CONTROLS].push_back(&scroll_middle);
    pointers[GROUP_CONTROLS].push_back(&mouse_speed);
    pointers[GROUP_CONTROLS].push_back(&scroll_speed_edge);
    pointers[GROUP_CONTROLS].push_back(&scroll_speed_click);
    pointers[GROUP_CONTROLS].push_back(&scroll_torus_x);
    pointers[GROUP_CONTROLS].push_back(&scroll_torus_y);
    pointers[GROUP_CONTROLS].push_back(&multiple_builders);
    pointers[GROUP_CONTROLS].push_back(&batter_priority);
    pointers[GROUP_CONTROLS].push_back(&prioinv_middle);
    pointers[GROUP_CONTROLS].push_back(&prioinv_right);
    pointers[GROUP_CONTROLS].push_back(&desc_mouse_speed);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_speed_edge);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_speed_click);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_edge);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_right);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_middle);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_torus_x);
    pointers[GROUP_CONTROLS].push_back(&desc_scroll_torus_y);
    pointers[GROUP_CONTROLS].push_back(&desc_multiple_builders);
    pointers[GROUP_CONTROLS].push_back(&desc_batter_priority);
    pointers[GROUP_CONTROLS].push_back(&desc_prioinv_middle);
    pointers[GROUP_CONTROLS].push_back(&desc_prioinv_right);

    pointers[GROUP_HOTKEYS ].push_back(&key_force_left);
    pointers[GROUP_HOTKEYS ].push_back(&key_force_right);
    pointers[GROUP_HOTKEYS ].push_back(&key_rate_minus);
    pointers[GROUP_HOTKEYS ].push_back(&key_rate_plus);
    pointers[GROUP_HOTKEYS ].push_back(&key_pause);
    pointers[GROUP_HOTKEYS ].push_back(&key_speed_slow);
    pointers[GROUP_HOTKEYS ].push_back(&key_speed_fast);
    pointers[GROUP_HOTKEYS ].push_back(&key_speed_turbo);
    pointers[GROUP_HOTKEYS ].push_back(&key_restart);
    pointers[GROUP_HOTKEYS ].push_back(&key_state_load);
    pointers[GROUP_HOTKEYS ].push_back(&key_state_save);
    pointers[GROUP_HOTKEYS ].push_back(&key_zoom);
    pointers[GROUP_HOTKEYS ].push_back(&key_chat);
    pointers[GROUP_HOTKEYS ].push_back(&key_spec_tribe);
    pointers[GROUP_HOTKEYS ].push_back(&key_nuke);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_force_left);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_force_right);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_rate_minus);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_rate_plus);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_pause);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_speed_slow);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_speed_fast);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_speed_turbo);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_restart);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_state_load);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_state_save);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_zoom);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_chat);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_spec_tribe);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_nuke);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_info_1);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_info_2);
    pointers[GROUP_HOTKEYS ].push_back(&desc_key_info_3);
    add_skill_to_hotkey_dialogue(LixEn::WALKER);
    add_skill_to_hotkey_dialogue(LixEn::RUNNER);
    add_skill_to_hotkey_dialogue(LixEn::JUMPER);
    add_skill_to_hotkey_dialogue(LixEn::BATTER);
    add_skill_to_hotkey_dialogue(LixEn::CLIMBER);
    add_skill_to_hotkey_dialogue(LixEn::FLOATER);
    add_skill_to_hotkey_dialogue(LixEn::EXPLODER);
    add_skill_to_hotkey_dialogue(LixEn::BLOCKER);
    add_skill_to_hotkey_dialogue(LixEn::CUBER);
    add_skill_to_hotkey_dialogue(LixEn::BUILDER);
    add_skill_to_hotkey_dialogue(LixEn::PLATFORMER);
    add_skill_to_hotkey_dialogue(LixEn::BASHER);
    add_skill_to_hotkey_dialogue(LixEn::MINER);
    add_skill_to_hotkey_dialogue(LixEn::DIGGER);
    // add_skill_to_hotkey_dialogue(LixEn::ROPER);

    pointers[GROUP_EDITOR  ].push_back(&ed_left);
    pointers[GROUP_EDITOR  ].push_back(&ed_right);
    pointers[GROUP_EDITOR  ].push_back(&ed_up);
    pointers[GROUP_EDITOR  ].push_back(&ed_down);
    pointers[GROUP_EDITOR  ].push_back(&ed_copy);
    pointers[GROUP_EDITOR  ].push_back(&ed_delete);
    pointers[GROUP_EDITOR  ].push_back(&ed_grid);
    pointers[GROUP_EDITOR  ].push_back(&ed_sel_all);
    pointers[GROUP_EDITOR  ].push_back(&ed_sel_frame);
    pointers[GROUP_EDITOR  ].push_back(&ed_sel_add);
    pointers[GROUP_EDITOR  ].push_back(&ed_foreground);
    pointers[GROUP_EDITOR  ].push_back(&ed_background);
    pointers[GROUP_EDITOR  ].push_back(&ed_mirror);
    pointers[GROUP_EDITOR  ].push_back(&ed_rotate);
    pointers[GROUP_EDITOR  ].push_back(&ed_dark);
    pointers[GROUP_EDITOR  ].push_back(&ed_noow);
    pointers[GROUP_EDITOR  ].push_back(&ed_zoom);
    pointers[GROUP_EDITOR  ].push_back(&ed_help);
    pointers[GROUP_EDITOR  ].push_back(&ed_menu_size);
    pointers[GROUP_EDITOR  ].push_back(&ed_menu_vars);
    pointers[GROUP_EDITOR  ].push_back(&ed_menu_skills);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_terrain);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_steel);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_hatch);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_goal);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_deco);
    pointers[GROUP_EDITOR  ].push_back(&ed_add_hazard);
    pointers[GROUP_EDITOR  ].push_back(&ed_grid_custom);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_left);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_right);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_up);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_down);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_copy);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_delete);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_grid);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_sel_all);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_sel_frame);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_sel_add);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_foreground);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_background);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_mirror);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_rotate);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_dark);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_noow);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_zoom);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_help);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_menu_size);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_menu_vars);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_menu_skills);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_terrain);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_steel);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_hatch);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_goal);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_deco);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_add_hazard);
    pointers[GROUP_EDITOR  ].push_back(&desc_ed_grid_custom);

    screen_scaling          .add_text (Language::option_screen_scaling_stretch);
    screen_scaling          .add_text (Language::option_screen_scaling_aspect);
    screen_scaling          .add_text (Language::option_screen_scaling_integer);
    debris_amount           .add_text (Language::option_debris_amount_none);
    debris_amount           .add_text (Language::option_debris_amount_own);
    debris_amount           .add_text (Language::option_debris_amount_all);
    debris_type             .add_text (Language::option_debris_type_stars);
    debris_type             .add_text (Language::option_debris_type_pixels);
    pointers[GROUP_GRAPHICS].push_back(&screen_resolution_x);
    pointers[GROUP_GRAPHICS].push_back(&screen_resolution_y);
    pointers[GROUP_GRAPHICS].push_back(&screen_windowed_x);
    pointers[GROUP_GRAPHICS].push_back(&screen_windowed_y);
    pointers[GROUP_GRAPHICS].push_back(&screen_windowed);
    pointers[GROUP_GRAPHICS].push_back(&screen_scaling);
    pointers[GROUP_GRAPHICS].push_back(&screen_border_colored);
    pointers[GROUP_GRAPHICS].push_back(&screen_vsync);
    pointers[GROUP_GRAPHICS].push_back(&arrows_replay);
    pointers[GROUP_GRAPHICS].push_back(&arrows_network);
    pointers[GROUP_GRAPHICS].push_back(&debris_amount);
    pointers[GROUP_GRAPHICS].push_back(&debris_type);
    pointers[GROUP_GRAPHICS].push_back(&gui_color_red);
    pointers[GROUP_GRAPHICS].push_back(&gui_color_green);
    pointers[GROUP_GRAPHICS].push_back(&gui_color_blue);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_resolution);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_windowed_res);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_windowed);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_scaling);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_border_colored);
    pointers[GROUP_GRAPHICS].push_back(&desc_screen_vsync);
    pointers[GROUP_GRAPHICS].push_back(&desc_arrows_replay);
    pointers[GROUP_GRAPHICS].push_back(&desc_arrows_network);
    pointers[GROUP_GRAPHICS].push_back(&desc_debris_amount);
    pointers[GROUP_GRAPHICS].push_back(&desc_debris_type);
    pointers[GROUP_GRAPHICS].push_back(&desc_gui_color_red);
    pointers[GROUP_GRAPHICS].push_back(&desc_gui_color_green);
    pointers[GROUP_GRAPHICS].push_back(&desc_gui_color_blue);
    pointers[GROUP_GRAPHICS].push_back(&desc_option_gfx_zero);
    pointers[GROUP_GRAPHICS].push_back(&desc_option_gfx_info);

    pointers[GROUP_SOUND]   .push_back(&sound_load_driver);
    pointers[GROUP_SOUND]   .push_back(&sound_volume);
    pointers[GROUP_SOUND]   .push_back(&desc_sound_load_driver);
    pointers[GROUP_SOUND]   .push_back(&desc_sound_volume);
    pointers[GROUP_SOUND]   .push_back(&desc_sound_info);

    // Add everything as children
    for (size_t i = 0; i < GROUP_MAX; ++i) {
        add_child(button_group[i]);
        for (std::vector <Element*> ::iterator itr = pointers[i].begin();
         itr != pointers[i].end(); ++itr) {
            (**itr).set_undraw_color(color[COL_API_M]);
            add_child(**itr);
        }
    }
    add_child(button_okay);
    add_child(button_cancel);

    reset_elements();
    show_group(static_cast <OptionGroup> (useR->option_group));

    Manager::add_elder(this);
}





OptionMenu::~OptionMenu()
{
}





// Alle angezeigten Elemente auf den derzeit vom Spiel benutzten Wert setzen
// Dies ist der Wert aus gloB->? oder useR->?, wobei
void OptionMenu::reset_elements()
{
    user_name            .set_text   (gloB->user_name);
    user_name_ask        .set_checked(gloB->user_name_ask);
    language             .set_text   (Language::language_name[language_nr]);
    replay_auto_single   .set_checked(gloB->replay_auto_single);
    replay_auto_multi    .set_checked(gloB->replay_auto_multi);
    replay_auto_max      .set_number (gloB->replay_auto_max);

    scroll_edge          .set_checked(useR->scroll_edge);
    scroll_right         .set_checked(useR->scroll_right);
    scroll_middle        .set_checked(useR->scroll_middle);
    mouse_speed          .set_number (useR->mouse_speed);
    scroll_speed_edge    .set_number (useR->scroll_speed_edge);
    scroll_speed_click   .set_number (useR->scroll_speed_click);
    scroll_torus_x       .set_number (useR->scroll_torus_x);
    scroll_torus_y       .set_number (useR->scroll_torus_y);
    multiple_builders    .set_checked(useR->multiple_builders);
    batter_priority      .set_checked(useR->batter_priority);
    prioinv_middle       .set_checked(useR->prioinv_middle);
    prioinv_right        .set_checked(useR->prioinv_right);

    key_force_left       .set_scancode(useR->key_force_left);
    key_force_right      .set_scancode(useR->key_force_right);
    key_rate_minus       .set_scancode(useR->key_rate_minus);
    key_rate_plus        .set_scancode(useR->key_rate_plus);
    key_pause            .set_scancode(useR->key_pause);
    key_speed_slow       .set_scancode(useR->key_speed_slow);
    key_speed_fast       .set_scancode(useR->key_speed_fast);
    key_speed_turbo      .set_scancode(useR->key_speed_turbo);
    key_restart          .set_scancode(useR->key_restart);
    key_state_load       .set_scancode(useR->key_state_load);
    key_state_save       .set_scancode(useR->key_state_save);
    key_zoom             .set_scancode(useR->key_zoom);
    key_nuke             .set_scancode(useR->key_nuke);
    key_spec_tribe       .set_scancode(useR->key_spec_tribe);
    key_chat             .set_scancode(useR->key_chat);
    for (size_t i = 0; i < key_skill.size(); ++i)
     key_skill[i]        .set_scancode(useR->key_skill[i]);

    ed_left              .set_scancode(useR->key_ed_left);
    ed_right             .set_scancode(useR->key_ed_right);
    ed_up                .set_scancode(useR->key_ed_up);
    ed_down              .set_scancode(useR->key_ed_down);
    ed_copy              .set_scancode(useR->key_ed_copy);
    ed_delete            .set_scancode(useR->key_ed_delete);
    ed_grid              .set_scancode(useR->key_ed_grid);
    ed_sel_all           .set_scancode(useR->key_ed_sel_all);
    ed_sel_frame         .set_scancode(useR->key_ed_sel_frame);
    ed_sel_add           .set_scancode(useR->key_ed_sel_add);
    ed_background        .set_scancode(useR->key_ed_background);
    ed_foreground        .set_scancode(useR->key_ed_foreground);
    ed_mirror            .set_scancode(useR->key_ed_mirror);
    ed_rotate            .set_scancode(useR->key_ed_rotate);
    ed_dark              .set_scancode(useR->key_ed_dark);
    ed_noow              .set_scancode(useR->key_ed_noow);
    ed_zoom              .set_scancode(useR->key_ed_zoom);
    ed_help              .set_scancode(useR->key_ed_help);
    ed_menu_size         .set_scancode(useR->key_ed_menu_size);
    ed_menu_vars         .set_scancode(useR->key_ed_menu_vars);
    ed_menu_skills       .set_scancode(useR->key_ed_menu_skills);
    ed_add_terrain       .set_scancode(useR->key_ed_add_terrain);
    ed_add_steel         .set_scancode(useR->key_ed_add_steel);
    ed_add_hatch         .set_scancode(useR->key_ed_add_hatch);
    ed_add_goal          .set_scancode(useR->key_ed_add_goal);
    ed_add_deco          .set_scancode(useR->key_ed_add_deco);
    ed_add_hazard        .set_scancode(useR->key_ed_add_hazard);
    ed_grid_custom       .set_number  (useR->editor_grid_custom);

    std::ostringstream s1; s1 << gloB->screen_resolution_x;
    std::ostringstream s2; s2 << gloB->screen_resolution_y;
    screen_resolution_x  .set_text(s1.str());
    screen_resolution_y  .set_text(s2.str());
    std::ostringstream s3; s3 << gloB->screen_windowed_x;
    std::ostringstream s4; s4 << gloB->screen_windowed_y;
    screen_windowed_x    .set_text(s3.str());
    screen_windowed_y    .set_text(s4.str());

    screen_windowed      .set_checked(useR->screen_windowed);
    screen_scaling       .set_number (useR->screen_scaling);
    screen_border_colored.set_checked(useR->screen_border_colored);
    screen_vsync         .set_checked(gloB->screen_vsync);
    arrows_replay        .set_checked(useR->arrows_replay);
    arrows_network       .set_checked(useR->arrows_network);
    debris_amount        .set_number (useR->debris_amount);
    debris_type          .set_number (useR->debris_type);
    gui_color_red        .set_number (useR->gui_color_red);
    gui_color_green      .set_number (useR->gui_color_green);
    gui_color_blue       .set_number (useR->gui_color_blue);

    sound_load_driver    .set_checked(gloB->sound_load_driver);
    sound_volume         .set_number (useR->sound_volume);
}





void OptionMenu::show_group(const OptionGroup grp)
{
    if (grp == GROUP_MAX) return;
    useR->option_group = grp;

    for (int i = 0; i < GROUP_MAX; ++i)
     for (std::vector <Element*> ::iterator itr = pointers[i].begin();
     itr != pointers[i].end(); ++itr) {
        if (i == grp) (**itr).set_hidden(false);
        else          (**itr).set_hidden(true);
    }

    // Korrekten Button gedrueckt darstellen
    for (std::vector <TextButton> ::iterator itr = button_group.begin();
     itr != button_group.end(); ++itr) itr->set_off();
    button_group[grp].set_on();
}





void OptionMenu::calc_self()
{
    // Bei geaendertem Spielernamen die neue Userdatei laden.
    // Die alte wird jedoch nicht gespeichert. Dazu muss man erst auf OK
    // klicken und das Optionsmenue erneut aufrufen.
    // Ob der Name zu lang ist, prueft User::get_name().
    if (user_name.get_text() != gloB->user_name) {
        gloB->user_name    = user_name.get_text();
        useR->load();
        useR->option_group = GROUP_GENERAL; // da sind wir ja
        reset_elements();
    }

    /////////////////////////
    // Angeklickte Buttons //
    /////////////////////////

    if (button_okay.get_clicked() || hardware.get_mr()) {
        const int res_fx = atoi(screen_resolution_x.get_text().c_str());
        const int res_fy = atoi(screen_resolution_y.get_text().c_str());
        const int res_wx = atoi(screen_windowed_x.get_text().c_str());
        const int res_wy = atoi(screen_windowed_y.get_text().c_str());
        const bool full  = gloB->screen_fullscreen_now;

        // Only call set_screen_mode() if it's necessary. The main menu
        // flickers for a very short time. A different screen_scaling option
        // should also call the function, because the function sets
        // glob_gfx.h's clear_screen_at_next_blit to true, to clear option
        // menu remainders on the screen borders.
        bool call_ssm = false;
        if (       useR->screen_scaling != screen_scaling.get_number()
         ||(  full && gloB->screen_resolution_x != res_fx)
         ||(  full && gloB->screen_resolution_y != res_fy)
         ||(! full && gloB->screen_windowed_x   != res_wx)
         ||(! full && gloB->screen_windowed_y   != res_wy)) call_ssm = true;

        bool reload_gralib = (
            useR->gui_color_red   != gui_color_red  .get_number()
         || useR->gui_color_green != gui_color_green.get_number()
         || useR->gui_color_blue  != gui_color_blue .get_number() );

        // Die Werte aller Checkboxen und Buttons in die Optionen schreiben
        // Die Konfigurationsdatei wird gegen eventuelle Abstuerze oder
        // Allegro-Abschuesse schon am Ende dieser Abarbeitung gespeichert.
        gloB->user_name_ask          = user_name_ask.get_checked();
        gloB->replay_auto_single     = replay_auto_single.get_checked();
        gloB->replay_auto_multi      = replay_auto_multi.get_checked();
        gloB->replay_auto_max        = replay_auto_max.get_number();

        if (useR->language != language_nr)
         Language::set(useR->language = language_nr);

        useR->mouse_speed       = mouse_speed       .get_number();
        useR->scroll_speed_edge = scroll_speed_edge .get_number();
        useR->scroll_speed_click= scroll_speed_click.get_number();
        useR->scroll_edge       = scroll_edge       .get_checked();
        useR->scroll_right      = scroll_right      .get_checked();
        useR->scroll_middle     = scroll_middle     .get_checked();
        useR->scroll_torus_x    = scroll_torus_x    .get_number();
        useR->scroll_torus_y    = scroll_torus_y    .get_number();
        useR->multiple_builders = multiple_builders .get_checked();
        useR->batter_priority   = batter_priority   .get_checked();
        useR->prioinv_middle    = prioinv_middle    .get_checked();
        useR->prioinv_right     = prioinv_right     .get_checked();

        useR->key_force_left  = key_force_left .get_scancode();
        useR->key_force_right = key_force_right.get_scancode();
        useR->key_rate_minus  = key_rate_minus .get_scancode();
        useR->key_rate_plus   = key_rate_plus  .get_scancode();
        useR->key_pause       = key_pause      .get_scancode();
        useR->key_speed_slow  = key_speed_slow .get_scancode();
        useR->key_speed_fast  = key_speed_fast .get_scancode();
        useR->key_speed_turbo = key_speed_turbo.get_scancode();
        useR->key_restart     = key_restart    .get_scancode();
        useR->key_state_load  = key_state_load .get_scancode();
        useR->key_state_save  = key_state_save .get_scancode();
        useR->key_zoom        = key_zoom       .get_scancode();
        useR->key_chat        = key_chat       .get_scancode();
        useR->key_spec_tribe  = key_spec_tribe .get_scancode();
        useR->key_nuke        = key_nuke       .get_scancode();
        for (size_t i = 0; i < key_skill.size(); ++i)
         useR->key_skill[i]   = key_skill[i]   .get_scancode();
        // There's only the Exploder hotkey in the options menu, but
        // it shall control both hotkeys.
        useR->key_skill[LixEn::EXPLODER2]
         = key_skill[LixEn::EXPLODER].get_scancode();

        useR->key_ed_left        = ed_left       .get_scancode();
        useR->key_ed_right       = ed_right      .get_scancode();
        useR->key_ed_up          = ed_up         .get_scancode();
        useR->key_ed_down        = ed_down       .get_scancode();
        useR->key_ed_copy        = ed_copy       .get_scancode();
        useR->key_ed_delete      = ed_delete     .get_scancode();
        useR->key_ed_grid        = ed_grid       .get_scancode();
        useR->key_ed_sel_all     = ed_sel_all    .get_scancode();
        useR->key_ed_sel_frame   = ed_sel_frame  .get_scancode();
        useR->key_ed_sel_add     = ed_sel_add    .get_scancode();
        useR->key_ed_background  = ed_background .get_scancode();
        useR->key_ed_foreground  = ed_foreground .get_scancode();
        useR->key_ed_mirror      = ed_mirror     .get_scancode();
        useR->key_ed_rotate      = ed_rotate     .get_scancode();
        useR->key_ed_dark        = ed_dark       .get_scancode();
        useR->key_ed_noow        = ed_noow       .get_scancode();
        useR->key_ed_zoom        = ed_zoom       .get_scancode();
        useR->key_ed_help        = ed_help       .get_scancode();
        useR->key_ed_menu_size   = ed_menu_size  .get_scancode();
        useR->key_ed_menu_vars   = ed_menu_vars  .get_scancode();
        useR->key_ed_menu_skills = ed_menu_skills.get_scancode();
        useR->key_ed_add_terrain = ed_add_terrain.get_scancode();
        useR->key_ed_add_steel   = ed_add_steel  .get_scancode();
        useR->key_ed_add_hatch   = ed_add_hatch  .get_scancode();
        useR->key_ed_add_goal    = ed_add_goal   .get_scancode();
        useR->key_ed_add_deco    = ed_add_deco   .get_scancode();
        useR->key_ed_add_hazard  = ed_add_hazard .get_scancode();
        useR->editor_grid_custom = ed_grid_custom.get_number();

        useR->screen_windowed       = screen_windowed      .get_checked();
        useR->screen_scaling        = screen_scaling       .get_number();
        useR->screen_border_colored = screen_border_colored.get_checked();
        useR->arrows_replay         = arrows_replay        .get_checked();
        useR->arrows_network        = arrows_network       .get_checked();
        useR->debris_amount         = debris_amount        .get_number();
        useR->debris_type           = debris_type          .get_number();
        useR->gui_color_red         = gui_color_red        .get_number();
        useR->gui_color_green       = gui_color_green      .get_number();
        useR->gui_color_blue        = gui_color_blue       .get_number();

        gloB->screen_resolution_x = atoi(screen_resolution_x.get_text().c_str());
        gloB->screen_resolution_y = atoi(screen_resolution_y.get_text().c_str());
        gloB->screen_windowed_x   = atoi(screen_windowed_x  .get_text().c_str());
        gloB->screen_windowed_y   = atoi(screen_windowed_y  .get_text().c_str());
        gloB->screen_vsync        = screen_vsync            .get_checked();

        gloB->sound_load_driver = sound_load_driver.get_checked();
        useR->sound_volume = sound_volume     .get_number();

        // Use new resolution that's already written to globals
        if (call_ssm) set_screen_mode(gloB->screen_fullscreen_now);
        if (reload_gralib) {
            make_all_colors();
            GraLib::deinitialize();
            GraLib::initialize();
            // the mouse cursor must be recerated by the Menu class
        }

        gloB->save();
        useR->save();
        exit = true;
    }
    else if (button_cancel.get_clicked()) {
        exit = true;
    }

    for (size_t i = 0; i < GROUP_MAX; ++i)
     if (button_group[i].get_clicked())
     show_group(static_cast <OptionGroup> (i));

    // Eigentliche Schalter einer ausgewaehlten Optionsgruppe
    switch (useR->option_group) {

    case GROUP_GENERAL:
        if (language.get_clicked()) {
            if (++language_nr == Language::MAX)
             language_nr       = Language::ENGLISH;
            language.set_text(Language::language_name[language_nr]);
        }
        break;

    case GROUP_CONTROLS: {
        if (!scroll_edge && !scroll_right && !scroll_middle) {
            if (scroll_edge.get_clicked()) scroll_right.set_checked();
            else                           scroll_edge .set_checked();
        }
        break; }

    case GROUP_HOTKEYS:
    case GROUP_GRAPHICS:
    case GROUP_SOUND:
        break;

    default:
        break;
    }
}

}
// Ende Namensraum Api
