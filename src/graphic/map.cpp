/*
 * map.cpp
 *
 * LEMSCR_Y - 60, weil das Panel 60 Pixel hoch ist
 *
 */

#include "map.h"

#include "../level/level.h"    // Mindestmasse
#include "../other/hardware.h" // Rechte Maustaste fuer Scrolling
#include "../other/globals.h"  // Scrollspeed-Einstellung
#include "../other/help.h"     // Timer-Ticks zur Scrollspeed-Berechnung
#include "../other/user.h"

Map::Map(int w, int h, int scr_xl, int scr_yl)
:
    Torbit(w, h),
    scroll_speed_edge (2 * useR->scroll_speed_edge
                       / Help::get_timer_ticks_per_draw()),
    scroll_speed_click(useR->scroll_speed_click),
    screen_xl   (scr_xl),
    screen_yl   (scr_yl),
    zoom        (false)
{
    if (w < Level::min_xl) w = Level::min_xl;
    if (h < Level::min_yl) h = Level::min_yl;

    min_screen_x = 0;
    min_screen_y = 0;
    max_screen_x = w - screen_xl;
    max_screen_y = h - screen_yl;
    if (max_screen_x < min_screen_x) max_screen_x = min_screen_x;
    if (max_screen_y < min_screen_y) max_screen_y = min_screen_y;

    clear_to_color(color[COL_PINK]);
}



void Map::resize(int w, int h)
{
    if (w < Level::min_xl) w = Level::min_xl;
    if (h < Level::min_yl) h = Level::min_yl;

    Torbit::resize(w, h);

    // Nicht ueber den Rand scrollen:
    set_zoom(zoom);
}



void Map::set_torus_x(const bool b)
{
    Torbit::set_torus_x(b);
    set_screen_x(get_screen_x());
}
void Map::set_torus_y(const bool b)
{
    Torbit::set_torus_y(b);
    set_screen_y(get_screen_y());
}




bool Map::get_scrollable()
{
    return get_scrollable_up()
     ||    get_scrollable_right()
     ||    get_scrollable_down()
     ||    get_scrollable_left();
}
bool Map::get_scrollable_up()
{
    if (screen_y > min_screen_y
     || (get_torus_y() && (useR->scroll_torus_y == 2
     || (useR->scroll_torus_y == 1 && get_yl() > screen_yl)) ) )
     return true;
    else return false;
}
bool Map::get_scrollable_right()
{
    if (screen_x < max_screen_x
     || (get_torus_x() && (useR->scroll_torus_x == 2
     || (useR->scroll_torus_x == 1 && get_xl() > screen_xl)) ) )
     return true;
    else return false;
}
bool Map::get_scrollable_down()
{
    if (screen_y < max_screen_y
     || (get_torus_y() && (useR->scroll_torus_y == 2
     || (useR->scroll_torus_y == 1 && get_yl() > screen_yl)) ) )
     return true;
    else return false;
}
bool Map::get_scrollable_left()
{
    if (screen_x > min_screen_x
     || (get_torus_x() && (useR->scroll_torus_x == 2
     || (useR->scroll_torus_x == 1 && get_xl() > screen_xl)) ) )
     return true;
    else return false;
}



void Map::set_screen_x(const int x)
{
    screen_x = x;
    if (get_torus_x() && (useR->scroll_torus_x == 2
     || (useR->scroll_torus_x == 1 && get_xl() > screen_xl))) {
        screen_x = Help::mod(screen_x, get_xl());
    }
    else {
        if (screen_x < min_screen_x) screen_x = min_screen_x;
        if (screen_x > max_screen_x) screen_x = max_screen_x;
    }
}

void Map::set_screen_y(const int y)
{
    screen_y = y;
    if (get_torus_y() && (useR->scroll_torus_y == 2
     || (useR->scroll_torus_y == 1 && get_yl() > screen_yl))) {
        // If-Abfrage war: > min_h*3/2, aber der ADmiral und ich meinen
        // beide, dass es nur auf Einbildschirmkarten sinnig ist. Sonst
        // geht zu viel Vorteil verloren gegenueber der besseren Uebersicht.
        screen_y = Help::mod(screen_y, get_yl());
    }
    else {
        if (screen_y < min_screen_y) screen_y = min_screen_y;
        if (screen_y > max_screen_y) screen_y = max_screen_y;
    }
}



void Map::set_zoom(const bool b)
{
    zoom = b;
    min_screen_x =                           - zoom*screen_xl/4;
    min_screen_y =                           - zoom*screen_yl/4;
    max_screen_x = get_xl() - screen_xl + zoom*screen_xl/4;
    max_screen_y = get_yl() - screen_yl + zoom*screen_yl/4;
    if (max_screen_x < min_screen_x) max_screen_x = min_screen_x;
    if (max_screen_y < min_screen_y) max_screen_y = min_screen_y;
    // Verhindern, dass ueber den Rand gescrollt ist
    set_screen_x(screen_x);
    set_screen_y(screen_y);
}



int Map::get_mouse_x()
{
    int ret = screen_x;
    ret += (zoom ? screen_xl/4    : 0      );
    ret += (zoom ? hardware.get_mx()/2 : hardware.get_mx());
    if (!get_torus_x() && screen_xl > get_xl() * (zoom+1))
     ret -= (screen_xl - get_xl() * (zoom+1)) / 2; // image is centered
    if (get_torus_x()) ret = Help::mod(ret, get_xl());
    return ret;
}

int Map::get_mouse_y()
{
    int ret = screen_y;
    ret += (zoom ? screen_yl/4    : 0      );
    ret += (zoom ? hardware.get_my()/2 : hardware.get_my());
    if (!get_torus_y() && screen_yl > get_yl() * (zoom+1))
     ret -= (screen_yl - get_yl() * (zoom+1)); // image is at lower edge
    if (get_torus_y()) ret = Help::mod(ret, get_yl());
    return ret;
}



void Map::calc_scrolling()
{
    // Scrollen am Rand oder (immer aktiv) mit dem Numblock
    int scrd = scroll_speed_edge;
    if (hardware.get_mrh()    || key[KEY_5_PAD])  scrd *= 2;
    if (zoom)                                    (scrd += 1) /= 2;
    if ((useR->scroll_edge && hardware.get_my() == 0)
     || key[KEY_8_PAD]
     || key[KEY_7_PAD]
     || key[KEY_9_PAD]) set_screen_y(screen_y - scrd);
    if ((useR->scroll_edge && hardware.get_mx() == LEMSCR_X-1)
     || key[KEY_6_PAD]
     || key[KEY_3_PAD]
     || key[KEY_9_PAD]) set_screen_x(screen_x + scrd);
    if ((useR->scroll_edge && hardware.get_my() == LEMSCR_Y-1)
     || key[KEY_2_PAD]
     || key[KEY_1_PAD]
     || key[KEY_3_PAD]) set_screen_y(screen_y + scrd);
    if ((useR->scroll_edge && hardware.get_mx() == 0)
     || key[KEY_4_PAD]
     || key[KEY_1_PAD]
     || key[KEY_7_PAD]) set_screen_x(screen_x - scrd);

    // Rechtsklick-Scrolling
    if (useR->scroll_right
     || useR->scroll_middle) {
        if ((hardware.get_mr() && useR->scroll_right)
         || (hardware.get_mm() && useR->scroll_middle)) {
            // Merken, auf welcher Hoehe die Maus war
            scroll_click_x = hardware.get_mx();
            scroll_click_y = hardware.get_my();
        }
        if ((hardware.get_mrh() && useR->scroll_right)
         || (hardware.get_mmh() && useR->scroll_middle)) {
            const bool xp = get_scrollable_right();
            const bool xm = get_scrollable_left();
            const bool yp = get_scrollable_down();
            const bool ym = get_scrollable_up();
            // Bildschirm tatsaechlich scrollen und ggf. Maus festhalten
            if ((xm && hardware.get_mx      () <= scroll_click_x
                    && hardware.get_mickey_x() <  0)
             || (xp && hardware.get_mx      () >= scroll_click_x
                    && hardware.get_mickey_x() >  0)) {
                set_screen_x(screen_x
                 + hardware.get_mickey_x() * scroll_speed_click / 4);
                hardware.freeze_mouse_x();
            }
            if ((ym && hardware.get_my      () <= scroll_click_y
                    && hardware.get_mickey_y() <  0)
             || (yp && hardware.get_my      () >= scroll_click_y
                    && hardware.get_mickey_y() >  0)) {
                set_screen_y(screen_y
                 + hardware.get_mickey_y() * scroll_speed_click / 4);
                hardware.freeze_mouse_y();
            }
        }
        // Ende mrh
    }
    // Ende Rechtsklick-Scrolling
}
// Ende calc_scrolling()
