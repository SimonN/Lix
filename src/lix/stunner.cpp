/*
 * lix/stunner.cpp
 *
 */

#include "ac.h"

void update_stunner(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Falls kein Boden da ist, wieder zum Tumbler werden. Der Code
    // ist vom Walker-Heruntergehen kopiert, aber wir messen nicht allzu
    // weit nach unten. Stattdessen soll lieber ein Tumbler mit wenig
    // Y-Beschleunigung erzeugt werden.
    // Waehrend einigen der ersten Frames liegt der Stunner nicht auf
    // dem Boden, in dem Fall wird auch keine Fallpruefung durchgefuehrt.
    if (l.get_frame() > 5) {
        int moved_down_by = 0;
        for (int i = 3; i < 7; ++i) {
            if (!l.is_solid()) {
                l.move_down(1);
                ++moved_down_by;
            }
            else break;
        }
        if (!l.is_solid()) {
            l.move_up(moved_down_by - 1); // against getting stuck in terrain
            l.become(LixEn::TUMBLER);                     // after 2011-08-17
            l.set_special_y(2);
            l.set_special_x(0);
        }
    }
    // Ende der Fallgeschichten

    // Normales Frame-Management
    if (l.get_ac() == LixEn::STUNNER) {
        // Das Liege-Frame wurde in L2 von Frame 8 bis einschliesslich
        // Frame 24 gezeigt, also in 17 Frames.
        if (l.get_frame() == 8) {
            l.set_special_x(l.get_special_x() + 1);
            if (l.get_special_x() == 17) l.next_frame();
        }
        else if (l.is_last_frame()) l.become(LixEn::WALKER);
        else l.next_frame();
    }
}
