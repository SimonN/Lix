/*
 * lemming/blocker.cpp
 *
 */

#include "ac.h"

void update_blocker(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Faller werden oder Blocker bleiben?
    if      (!l.is_solid())       l.assign(LixEn::FALLER);
    else if (l.get_frame() == 19) l.set_frame(4);

    // Der zum Walker gemachte Blocker bleibt kurze Zeit Blocker!
    else if (l.is_last_frame())   l.assign(LixEn::WALKER);
    else                          l.next_frame();
}
