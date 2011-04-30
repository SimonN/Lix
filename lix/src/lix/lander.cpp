/*
 * lemming/lander.cpp
 *
 */

#include "ac.h"

void update_lander(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) {
        // Wir nutzen nicht Lemming::assign(LixEn::WALKER), weil das zum
        // staendigen Drehen fuehrt, aehnlich wie die Walker-Zuweisung bei
        // einem normalen Walker.
        l.set_ac(LixEn::WALKER);
        l.set_special_x(0);
        l.set_special_y(0);
        l.set_frame(0);
    }
    else l.next_frame();
}
