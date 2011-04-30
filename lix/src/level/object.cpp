/*
 * level/special.cpp
 *
 */

#include "object.h"
#include "../graphic/glob_gfx.h" // colors for the selbox determination

Object::Object(const Cutbit& c, Type t, int st)
:
    cb        (c),
    type      (t),
    subtype   (st),
    selbox_x  (cb.get_xl()), // Initializing these with the smallest selbox
    selbox_y  (cb.get_yl()), // starting at the furthest point is necessary
    selbox_xl (0),           // for determine_selbox().
    selbox_yl (0),
    trigger_x (0),
    trigger_y (0),
    trigger_xl(0),
    trigger_yl(0),
    sound     (Sound::NOTHING)
{
    switch (type) {
    case HATCH:
        trigger_x  = cb.get_xl() / 2;
        trigger_y  = std::max(20, cb.get_yl() - 24);
        break;
    case GOAL:
        trigger_x  = cb.get_xl() / 2 - 5;
        trigger_y  = cb.get_yl() - 7;
        trigger_xl = 10;
        trigger_yl = 10;
        break;
    case TRAP:
        trigger_x  = cb.get_xl() / 2     - 3;
        trigger_y  = cb.get_yl() * 4 / 5 - 3;
        trigger_xl = 6;
        trigger_yl = 6;
        sound      = Sound::SPLAT;
        break;
    case WATER:
        trigger_x  = 0;
        trigger_y  = 20;
        trigger_xl = cb.get_xl();
        trigger_yl = cb.get_yl() - 20;
        // Feuer?
        if (subtype) {
            trigger_y  = 0;
            trigger_yl = cb.get_yl() + 12;
        }
        break;
    default:
        break;
    }

    determine_selbox();
}



Object::~Object()
{
}



Object::Type Object::perm(const int n)
{
    // don't start with 0 because most classes start with TERRAIN, not EMPTY.
    return n == 1 ? Object::HATCH
     :     n == 2 ? Object::GOAL
     :     n == 3 ? Object::DECO
     :     n == 4 ? Object::TRAP
     :     n == 5 ? Object::WATER
     :     n == 6 ? Object::FLING
     :     n == 7 ? Object::TRAMPOLINE
     :     n == 8 ? Object::TERRAIN
     //:   n == 9 ? Object::ONEWAY
     :              Object::EMPTY; // Ich weiss keine Liste mehr -> TERRAIN
}



// This assumes that the selbox is initially starting at (0, 0) and stretching
// across the whole object. The selbox is set this way in the constructor.
void Object::determine_selbox()
{
    if (!cb) return;

    for  (int xf = 0; xf < cb.get_x_frames(); ++xf)
     for (int yf = 0; yf < cb.get_y_frames(); ++yf) {
        int  x_min = -1;
        int  x_max = cb.get_xl();
        int  y_min = -1;
        int  y_max = cb.get_yl();
        bool stop = false;
        while (!stop && x_max >= 0) {
            x_max -= 1;
            for (int y = 0; y < cb.get_yl(); y += 1)
             if (cb.get_pixel(xf, yf, x_max, y) != color[COL_PINK]) stop=true;
        }
        stop = false;
        while (!stop && x_min < x_max) {
            x_min += 1;
            for (int y = 0; y < cb.get_yl(); y += 1)
             if (cb.get_pixel(xf, yf, x_min, y) != color[COL_PINK]) stop=true;
        }
        stop = false;
        while (!stop && y_max >= 0) {
            y_max -= 1;
            for (int x = 0; x < cb.get_xl(); x += 1)
             if (cb.get_pixel(xf, yf, x, y_max) != color[COL_PINK]) stop=true;
        }
        stop = false;
        while (!stop && y_min < y_max) {
            y_min += 1;
            for (int x = 0; x < cb.get_xl(); x += 1)
             if (cb.get_pixel(xf, yf, x, y_min) != color[COL_PINK]) stop=true;
        }
        selbox_x  = std::min(selbox_x, x_min);
        selbox_y  = std::min(selbox_y, y_min);
        selbox_xl = std::max(selbox_xl, x_max - x_min + 1);
        selbox_yl = std::max(selbox_yl, y_max - y_min + 1);
    }
}
