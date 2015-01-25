/*
 * lix/exiter.cpp
 *
 * special_x
 *
 *   If the lix will be eligible for scoring at the end of the animation:
 *
 *   < 0     move 1 pixel to the left while exiting, add 1
 *   0       do the exiting animation straight
 *   < 1000  move 1 pixel to the right while exiting, deduct 1
 *
 *   1000    may be killed. They have already been scored.
 *
 * special_y
 *
 *   unused since exiters are scored at start, not at end, of anim
 *
 */

#include "ac.h"

void become_exiter(Lixxie& l)
{
    if (l.get_ac() != LixEn::EXITER) {
        l.become_default(LixEn::EXITER);
        l.set_updates_since_bomb(0);
        --l.get_tribe().lix_out;
        ++l.get_tribe().lix_exiting;
    }
}



void update_exiter(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    int x = l.get_special_x();
    if (x < 0) {
        l.set_x(l.get_x() + 1);
        ++x;
    }
    else if (x > 0 && x < 1000) {
        l.set_x(l.get_x() - 1);
        --x;
    }
    l.set_special_x(x);

    if (!l.is_last_frame()) l.next_frame();
    else l.set_special_x(1000);
}
