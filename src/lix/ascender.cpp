/*
 * Ascender: The transistion from climber to walker, or a walker getting
 * up a high ledge.
 *
 */

#include "ac.h"


void become_ascender(Lixxie& l)
{
    l.become_default(LixEn::ASCENDER);

    // determine the height to step up. Seek a floor pixel with air above it.
    int swh = 0;
    while (swh < 26 && ! (l.is_solid(0, 2 - swh) && ! l.is_solid(0, 1 - swh)))
        ++swh;
    int frame = 5; // this is the last frame
    switch (swh) {
        case 0:  case 1:
        case 2:  case 3:  frame = 5; break;
        case 4:  case 5:  frame = 4; break;
        case 6:  case 7:  frame = 3; break;
        case 8:  case 9:  frame = 2; break;
        case 10: case 11: frame = 1; break;
        default:          frame = 0; break;
    }
    int swh_wanted = 10 - frame * 2;
    l.set_frame(frame);

    // Move up, [check for even cliff,] move down again
    l.move_up(swh);
    // This was in earlier versions to look good, but the community frowned
    // on it for inconsistency; and sometimes the skipped x-pixels have indeed
    // yielded problems.
    // if (! l.is_solid(2, 0) && ! l.is_solid(2, 1)) l.move_ahead();
    l.move_down(swh_wanted);
}



void update_ascender(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // normally, move up once, but not on the last frame
    if (l.get_frame() != 5) l.move_up();

    if (l.is_last_frame()) l.become(LixEn::WALKER);
    else l.next_frame();
}
