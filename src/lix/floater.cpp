/*
 * lix/floater.cpp
 *
 */

#include "ac.h"

#include "../other/help.h" // /2*2

void assclk_floater(Lixxie& l)
{
    l.set_floater();
}


void update_floater(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // special_y und special_x steuern, wie viel Pixel die Lix jeweils
    // faellt. Die Vorgaben sind 0|0, sie sollten nach assign vom Faller
    // und vom Jumper geaendert werden.
    switch (l.get_frame()) {
        case 0: l.set_special_y(l.get_special_y() > 10 ? 10 : 6); break;
        case 1: l.set_special_y(6); break;
        case 2: l.set_special_y(2); break;
        case 3:
        case 4: l.set_special_y(0); break;
        case 5: l.set_special_y(2); break;
        case 6: l.set_special_y(4); break; // Das ist das normale Fallen.
        default:                    break;
    }

    if (l.get_special_x() > 0 && l.get_frame() > 0) {
        l.set_special_x(Help::even(l.get_special_x() - 2));
    }
    // special x and y have been set to new values for the motion further
    // down in this function. They have ignored terrain in the way so far,
    // that must be corrected during the motion.

    // The following code is a little like jumper_and_tumbler_collision.
    // I don't dare to use that code for floaters too. It's already very
    // complicated and many of its corner cases don't apply to floaters.
    // spx tracks the special_x to be used this update, which may not always
    // be the same as l.get_special_x() if a wall is hit.
          int spx    = l.get_special_x();
    const int spy    = l.get_special_y();
    const int sp_max = std::max(spx, spy);

    bool floor_hit = false;
    bool wall_hit  = false;

    for (int i = 0; i < sp_max; ++i) {
        // move either 1 pixel down, 1 pixel ahead, or 1 pixel both. This
        // takes max(spx, spy) chess king moves to get to the target square.
        // Therefore i runs to max(spx, spy).
        // We don't move during this loop, except upon hitting floor. The
        // normal movement occurs after this loop. This is unlike jumper_and_
        // _tumbler_collision.
        const int x = (wall_hit ? spx : spx * i / sp_max);
        const int y =                   spy * i / sp_max;
        // check for floor
        if (l.is_solid(x, y + 2)) {
            l.move_ahead(x);
            l.move_down(y);
            l.become(LixEn::LANDER);
            floor_hit = true;
            break;
        }
        // check for wall
        else if (l.is_solid(x + 2, y)) {
            wall_hit = true;
            l.set_special_x(0);
            spx = x;
            // now the current value of x is frozen for this update, see
            // definition of const int x = (wall hit ? spx : ...), and all
            // future updates will use l.get_special_x() == spx == x == 0.
        }
    }
    if (! floor_hit) {
        l.move_ahead(spx);
        l.move_down (spy);
    }
    // During motion, we could have become a lander. Otherwise:
    if (l.get_ac() == LixEn::FLOATER) {
        if (l.is_last_frame()) l.set_frame(9);
        else l.next_frame();
    }
}
