/*
 * editor/win_scro.cpp
 *
 */

#include "win_scro.h"

#include "../other/help.h"
#include "../other/language.h"

namespace Api {

const unsigned WindowScroll::this_xl(420);
const unsigned WindowScroll::this_yl(280);
const unsigned WindowScroll::nrxl(180); // Number X-Length
const unsigned WindowScroll::rgxl(180); // Regular X-Length
const unsigned WindowScroll::step_small(10); // Fuer Rasterbestimmung

WindowScroll::WindowScroll(Level& l, Map& m)
:
    Window(LEMSCR_X/2 - this_xl/2,
           LEMSCR_Y/2 - this_yl/2 - 30,
     this_xl, this_yl, Language::win_scroll_title),

    level(l),
    map  (m),
    desc_win_scroll   (20,  40, Language::win_scroll_desc),
    x(this_xl-20-nrxl, 60, nrxl, 4, 0,
     level.torus_x ? level.size_x : level.size_x - m.get_screen_xl(),
     level.start_x, true),
    y(this_xl-20-nrxl, 80, nrxl, 4, 0,
     level.torus_y ? level.size_y : level.size_y - m.get_screen_yl(),
     level.start_y, true),
    jump   (20,              120, rgxl),
    current(this_xl-20-nrxl, 120, nrxl),
    bg_r(this_xl-20-nrxl, 160, nrxl),
    bg_g(this_xl-20-nrxl, 180, nrxl),
    bg_b(this_xl-20-nrxl, 200, nrxl),
    desc_win_scroll_x(20,  60, Language::win_scroll_x),
    desc_win_scroll_y(20,  80, Language::win_scroll_y),
    desc_win_scroll_r(20, 160, Language::win_scroll_r),
    desc_win_scroll_g(20, 180, Language::win_scroll_g),
    desc_win_scroll_b(20, 200, Language::win_scroll_b),
    ok     (20,              240, rgxl),
    cancel (this_xl-20-nrxl, 240, nrxl)
{
    add_child(desc_win_scroll);
    add_child(x);
    add_child(y);
    add_child(bg_r);
    add_child(bg_g);
    add_child(bg_b);
    add_child(jump);
    add_child(current);
    add_child(ok);
    add_child(cancel);
    add_child(desc_win_scroll_x);
    add_child(desc_win_scroll_y);
    add_child(desc_win_scroll_r);
    add_child(desc_win_scroll_g);
    add_child(desc_win_scroll_b);

    x.set_step_sml  ( 16);
    y.set_step_sml  ( 16);
    x.set_step_med  ( 80);
    y.set_step_med  ( 80);
    x.set_step_big  (m.get_screen_xl());
    y.set_step_big  (m.get_screen_yl());
    x.set_white_zero();
    y.set_white_zero();
    bg_r.set_macro_color(level.bg_red);
    bg_g.set_macro_color(level.bg_green);
    bg_b.set_macro_color(level.bg_blue);
    jump   .set_text(Language::win_scroll_jump   );
    current.set_text(Language::win_scroll_current);
    ok     .set_text(Language::ok                );
    cancel .set_text(Language::cancel            );
    ok     .set_hotkey(KEY_ENTER);
    cancel .set_hotkey(KEY_ESC);
}

WindowScroll::~WindowScroll()
{
}





void WindowScroll::calc_self()
{
    if (jump.get_clicked()) {
        map.set_screen_x(x.get_number());
        map.set_screen_y(y.get_number());
    }
    else if (current.get_clicked()) {
        x.set_number((map.get_screen_x()+step_small/2)/step_small*step_small);
        y.set_number((map.get_screen_y()+step_small/2)/step_small*step_small);
        map.set_screen_x(x.get_number());
        map.set_screen_y(y.get_number());
    }
    else if (ok.get_clicked() || hardware.get_mr()) {
        level.start_x  = Help::mod(x.get_number(), level.size_x);
        level.start_y  = Help::mod(y.get_number(), level.size_y);
        level.bg_red   = bg_r.get_number();
        level.bg_green = bg_g.get_number();
        level.bg_blue  = bg_b.get_number();
        set_exit();
    }
    else if (cancel.get_clicked()) set_exit();
}

} // Ende Namensraum Api
