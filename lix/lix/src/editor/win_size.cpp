/*
 * editor/win_size.cpp
 *
 */

#include "win_size.h"

#include "../other/help.h"
#include "../other/language.h"

namespace Api {

const unsigned WindowSize::this_xl (300);
const unsigned WindowSize::this_yl (320);
const unsigned WindowSize::nrxl    (200); // Number X-Length
const unsigned WindowSize::x_offset(this_xl - 20 - nrxl);

const unsigned WindowSize::y_l      (                        40);
const unsigned WindowSize::y_r      (WindowSize::y_l       + 20);
const unsigned WindowSize::y_u      (WindowSize::y_r       + 40);
const unsigned WindowSize::y_d      (WindowSize::y_u       + 20);
const unsigned WindowSize::y_x      (WindowSize::y_d       + 40);
const unsigned WindowSize::y_y      (WindowSize::y_x       + 20);
const unsigned WindowSize::y_torus_x(WindowSize::y_y       + 40);
const unsigned WindowSize::y_torus_y(WindowSize::y_torus_x + 20);

WindowSize::WindowSize(Level& lv, Map& mp)
:
    Window(60, 60, this_xl,
     this_yl, Language::win_size_title),

    level(lv),
    map  (mp),

    exit_with_x_to_adjust(0),
    exit_with_y_to_adjust(0),

    // Minimum und Maximum dieser vier Einstell-Zahlen werden in
    // calc() staendig neu gesetzt.
    l(x_offset, y_l, nrxl, 5, -Level::max_xl, Level::max_xl, 0, true),
    r(x_offset, y_r, nrxl, 5, -Level::max_xl, Level::max_xl, 0, true),
    u(x_offset, y_u, nrxl, 5, -Level::max_yl, Level::max_yl, 0, true),
    d(x_offset, y_d, nrxl, 5, -Level::max_yl, Level::max_yl, 0, true),

    torus_x(20, y_torus_x),
    torus_y(20, y_torus_y),

    ok    ( 20, this_yl - 40, 120),
    cancel(160, this_yl - 40, 120),

    desc_l(20, l.get_y(), Language::win_size_l),
    desc_r(20, r.get_y(), Language::win_size_r),
    desc_u(20, u.get_y(), Language::win_size_u),
    desc_d(20, d.get_y(), Language::win_size_d),
    desc_x(20, y_x,       Language::win_size_x),
    desc_y(20, y_y,       Language::win_size_y),
    desc_torus_x(60, y_torus_x, Language::win_size_torus_x),
    desc_torus_y(60, y_torus_y, Language::win_size_torus_y)
{
    add_child(l);
    add_child(r);
    add_child(u);
    add_child(d);
    add_child(torus_x);
    add_child(torus_y);
    add_child(ok);
    add_child(cancel);
    add_child(desc_l);
    add_child(desc_r);
    add_child(desc_u);
    add_child(desc_d);
    add_child(desc_x);
    add_child(desc_y);
    add_child(desc_torus_x);
    add_child(desc_torus_y);

    l.set_step_sml(2); l.set_step_med(20); l.set_step_big(Level::min_xl);
    r.set_step_sml(2); r.set_step_med(20); r.set_step_big(Level::min_xl);
    u.set_step_sml(2); u.set_step_med(20); u.set_step_big(Level::min_yl);
    d.set_step_sml(2); d.set_step_med(20); d.set_step_big(Level::min_yl);

    l.set_show_sign();
    r.set_show_sign();
    u.set_show_sign();
    d.set_show_sign();

    torus_x.set_checked(level.torus_x);
    torus_y.set_checked(level.torus_y);

    ok    .set_text(Language::ok);
    cancel.set_text(Language::cancel);
    ok    .set_hotkey(KEY_ENTER);
    cancel.set_hotkey(KEY_ESC);
}



WindowSize::~WindowSize()
{
}



void WindowSize::calc_self()
{
    if (map.get_xl() + l.get_number() + r.get_number() < Level::min_xl)
     l.set_number(Level::min_xl - map.get_xl() - r.get_number());
    if (map.get_xl() + l.get_number() + r.get_number() < Level::min_xl)
     r.set_number(Level::min_xl - map.get_xl() - l.get_number());
    if (map.get_xl() + l.get_number() + r.get_number() > Level::max_xl)
     l.set_number(Level::max_xl - map.get_xl() - r.get_number());

    if (map.get_xl() + l.get_number() + r.get_number() > Level::max_xl)
     r.set_number(Level::max_xl - map.get_xl() - l.get_number());

    if (map.get_yl() + u.get_number() + d.get_number() < Level::min_yl)
     u.set_number(Level::min_yl - map.get_yl() - d.get_number());
    if (map.get_yl() + u.get_number() + d.get_number() < Level::min_yl)
     d.set_number(Level::min_yl - map.get_yl() - u.get_number());
    if (map.get_yl() + u.get_number() + d.get_number() > Level::max_yl)
     u.set_number(Level::max_yl - map.get_yl() - d.get_number());

    if (map.get_yl() + u.get_number() + d.get_number() > Level::max_yl)
     d.set_number(Level::max_yl - map.get_yl() - u.get_number());

    if (ok.get_clicked() || hardware.get_mr()) {
        if (l.get_number() == 0 && r.get_number() == 0
         && u.get_number() == 0 && d.get_number() == 0
         && level.torus_x == torus_x
         && level.torus_y == torus_y) set_exit();
        else {
            exit_with_x_to_adjust = l.get_number();
            exit_with_y_to_adjust = u.get_number();
            map.set_torus_x(torus_x);
            map.set_torus_y(torus_y); // Hier ok: resize callt zoom()
            map.resize(map.get_xl() + l.get_number() + r.get_number(),
                       map.get_yl() + u.get_number() + d.get_number());
            map.set_screen_x(map.get_screen_x() + exit_with_x_to_adjust);
            map.set_screen_y(map.get_screen_y() + exit_with_y_to_adjust);
            level.size_x = map.get_xl();
            level.size_y = map.get_yl();
            level.start_x += exit_with_x_to_adjust;
            level.start_y += exit_with_y_to_adjust;
            level.torus_x = torus_x;
            level.torus_y = torus_y;
            if (torus_x) level.start_x = Help::mod(level.start_x,level.size_x);
            if (torus_y) level.start_y = Help::mod(level.start_y,level.size_y);
            if (!torus_x && level.start_x > level.size_x - map.get_screen_xl())
                            level.start_x = level.size_x - map.get_screen_xl();
            if (!torus_y && level.start_y > level.size_y - map.get_screen_yl())
                            level.start_y = level.size_y - map.get_screen_yl();
            set_exit();
        }
    }
    else if (cancel.get_clicked()) set_exit();
}



void WindowSize::draw_self()
{
    Window::draw_self();
    draw_calculation(get_y_here() + y_x,
     map.get_xl(), l.get_number() + r.get_number());
    draw_calculation(get_y_here() + y_y,
     map.get_yl(), u.get_number() + d.get_number());
}



// old, plus
void WindowSize::draw_calculation(const int y, const int o, const int p)
{
    std::string s_old(4, ' ');
    std::string s_add(4, ' ');
    std::string s_new(4, ' ');
    std::string s_pls(p < 0 ? "-" : "+");
    std::string s_eql("=");

    Torbit& tb = get_torbit();
    BITMAP* bp = tb.get_al_bitmap();

    // Alte Zahl wegputzen
    rectfill(bp, x_offset, y, this_xl-2,y+19,color[COL_API_M]);

    for (int i = 3, temp = o; i >= 0 && temp > 0; --i) {
        s_old[i] = '0' + temp%10;
        temp /= 10;
    }
    for (int i = 3, temp = o+p; i >= 0 && temp > 0; --i) {
        s_new[i] = '0' + temp%10;
        temp /= 10;
    }
    s_add[3] = '0';
    for (int i = 3, temp = (p < 0) ? -p : p; i >= 0 && temp > 0; --i) {
        s_add[i] = '0' + temp%10;
        temp /= 10;
    }
    const int x_here = get_x_here() + x_offset;
    Help::draw_shadow_fixed_text(tb, font_med, s_old, x_here+  0, y, color[COL_TEXT]);
    Help::draw_shadow_fixed_text(tb, font_med, s_pls, x_here+ 55, y, color[COL_TEXT]);
    Help::draw_shadow_fixed_text(tb, font_med, s_add, x_here+ 85, y, color[COL_TEXT]);
    Help::draw_shadow_fixed_text(tb, font_med, s_eql, x_here+135, y, color[COL_TEXT]);
    Help::draw_shadow_fixed_text(tb, font_med, s_new, x_here+160, y, color[COL_TEXT_ON]);
}

} // Ende Namensraum Api
