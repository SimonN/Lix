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

    // Prevent moving up a giant amount if there is no wall at all
    // in front of the lix, which led to a bug reported by Nepster in 2014-06.
    // This assumes that all pixels are empty. It might create a further bug
    // when all pixels are solid, but I don't think become_ascender is ever
    // called when that is the case.
    if (swh == 26) {
        l.become(LixEn::FALLER);
        return;
    }

    // if there is a good step
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

    bool change_to_walker = false;

    if (l.is_last_frame()) change_to_walker = true;
    else l.next_frame();

    // compute distance to hoist point based on current frame
    int distance_to_top = 0;
    if (l.get_frame() < 5) {
        distance_to_top = 2 * (5 - l.get_frame());
    } 
    
    // check from hoist point downwards for terrain
    int air_at_top = 0;
    while (air_at_top <= distance_to_top
          && ! l.is_solid(0, -distance_to_top + 1 + air_at_top) 
          && ! l.is_solid(0, -distance_to_top + 2 + air_at_top))
    {
        air_at_top += 2;
    }

    if (air_at_top > distance_to_top) {
        // no terrain at all between ascender and hoist point
        change_to_walker = false;
        l.become(LixEn::FALLER);
    }
    else if (air_at_top > 0) {
        // advance frame by air_at_top and thus set hoist point 
        // down by air_at_top
        l.set_frame(l.get_frame() + air_at_top/2);

        if (! l.is_solid(0, 1)) {
            // then (0, 2) is solid and we move the lix down 1 px
            l.move_down(1);
        }
    }

    if (change_to_walker) {
        l.become(LixEn::WALKER);
    }

}
