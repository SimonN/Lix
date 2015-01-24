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
 *   1000    may be killed and scored by next gameplay update
 *
 *   if > 1000, then it's the same as special_x - 2000; with the exception
 *   that the lix doesn't count towards the number saved, because the goals
 *   had been closed upon starting the exiter animation. Thus:
 *
 *   > 1000  move 1 pixel to the left while exiting, add 1
 *   2000    do the exit animation straight
 *   < 3000  move 1 pixel to the right while exiting, deduct 1
 *
 *   3000    may be killed and scored on the saved-too-late counter
 *
 * special_y
 *
 *   Nimmt die Nummer des Ziels im Ziel-Vektor auf. Allein die Gameplay-Klasse
 *   kann mit dieser Info etwas anfangen. Die in dieser Datei stehenden Funk-
 *   tionen scheren sich nicht um special_x.
 *
 */

#include "ac.h"

void become_exiter(Lixxie& l)
{
    l.become_default(LixEn::EXITER);
    l.set_updates_since_bomb(0);
}



void update_exiter(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    int x = l.get_special_x();
    if (x < 0 || (x > 1000 && x < 2000)) {
        l.set_x(l.get_x() + 1);
        ++x;
    }
    else if ((x > 0 && x < 1000) || (x > 2000 && x < 3000)) {
        l.set_x(l.get_x() - 1);
        --x;
    }
    l.set_special_x(x);

    if (!l.is_last_frame()) l.next_frame();
    else {
        if (l.get_special_x() < 1000) l.set_special_x(1000);
        else                          l.set_special_x(3000);
    }
}
