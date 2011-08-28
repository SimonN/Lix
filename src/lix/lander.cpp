/*
 * lix/lander.cpp
 *
 */

#include "ac.h"

void update_lander(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) l.become(LixEn::WALKER);
    else l.next_frame();
}
