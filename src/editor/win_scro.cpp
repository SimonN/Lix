/*
 * editor/win_scro.cpp
 *
 */

#include "win_scro.h"

#include "../other/help.h"
#include "../other/language.h"
#include "../other/user.h"

namespace Api {

const unsigned WindowScroll::this_xl(420);
const unsigned WindowScroll::this_yl(300);
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
    manual     (20, 40),
    desc_manual(60, 40, Language::win_scroll_manual),
    x(this_xl-20-nrxl,  80, nrxl, 4, 0, level.size_x, level.start_x, true),
    y(this_xl-20-nrxl, 100, nrxl, 4, 0, level.size_y, level.start_y, true),
    jump   (20,              140, rgxl),
    current(this_xl-20-nrxl, 140, nrxl),
    bg_r(this_xl-20-nrxl, 180, nrxl),
    bg_g(this_xl-20-nrxl, 200, nrxl),
    bg_b(this_xl-20-nrxl, 220, nrxl),
    desc_win_scroll_x(20,  80, Language::win_scroll_x),
    desc_win_scroll_y(20, 100, Language::win_scroll_y),
    desc_win_scroll_r(20, 180, Language::win_scroll_r),
    desc_win_scroll_g(20, 200, Language::win_scroll_g),
    desc_win_scroll_b(20, 220, Language::win_scroll_b),
    ok     (20,              260, rgxl),
    cancel (this_xl-20-nrxl, 260, nrxl)
{
    add_child(manual);
    add_child(desc_manual);
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

    manual.set_checked(l.start_manual);

    x                .set_hidden(! manual.get_checked());
    y                .set_hidden(! manual.get_checked());
    desc_win_scroll_x.set_hidden(! manual.get_checked());
    desc_win_scroll_y.set_hidden(! manual.get_checked());
    jump             .set_hidden(! manual.get_checked());
    current          .set_hidden(! manual.get_checked());

    x.set_step_sml  ( 2);
    y.set_step_sml  ( 2);
    x.set_step_med  (16);
    y.set_step_med  (16);
    x.set_step_big  (80);
    y.set_step_big  (80);
    x.set_white_zero();
    y.set_white_zero();

    x      .set_undraw_color(color[COL_API_M]);
    y      .set_undraw_color(color[COL_API_M]);
    jump   .set_undraw_color(color[COL_API_M]);
    current.set_undraw_color(color[COL_API_M]);

    bg_r.set_macro_color(level.bg_red);
    bg_g.set_macro_color(level.bg_green);
    bg_b.set_macro_color(level.bg_blue);
    jump   .set_text(Language::win_scroll_jump   );
    current.set_text(Language::win_scroll_current);
    ok     .set_text(Language::ok                );
    cancel .set_text(Language::cancel            );
    ok     .set_hotkey(useR->key_me_okay);
    cancel .set_hotkey(useR->key_me_exit);
}

WindowScroll::~WindowScroll()
{
}





void WindowScroll::calc_self()
{
    if (manual.get_clicked()) {
        x                .set_hidden(! manual.get_checked());
        y                .set_hidden(! manual.get_checked());
        desc_win_scroll_x.set_hidden(! manual.get_checked());
        desc_win_scroll_y.set_hidden(! manual.get_checked());
        jump             .set_hidden(! manual.get_checked());
        current          .set_hidden(! manual.get_checked());
    }
    if (jump.get_clicked()) {
        map.set_screen_x(x.get_number());
        map.set_screen_y(y.get_number());
    }
    if (current.get_clicked()) {
        int ax = (map.get_screen_x()+step_small/2) / step_small * step_small;
        int ay = (map.get_screen_y()+step_small/2) / step_small * step_small;
        x.set_number(Help::mod(ax, level.size_x));
        y.set_number(Help::mod(ay, level.size_y));
        map.set_screen_x(x.get_number());
        map.set_screen_y(y.get_number());
    }
    if (ok.get_clicked() || hardware.get_mr()) {
        level.start_manual = manual.get_checked();
        level.start_x      = x.get_number();
        level.start_y      = y.get_number();
        level.bg_red   = bg_r.get_number();
        level.bg_green = bg_g.get_number();
        level.bg_blue  = bg_b.get_number();
        set_exit();
    }
    else if (cancel.get_clicked()) set_exit();
}

} // Ende Namensraum Api
