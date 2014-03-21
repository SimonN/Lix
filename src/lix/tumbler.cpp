/*
 * lix/tumbler.cpp
 *
 */

#include "ac.h"

void become_tumbler(Lixxie& l)
{
    if (l.is_solid(0, 0)) {
        if (l.get_ac() == LixEn::ASCENDER) {
            // unglitch out of wall, but only back and up
            for (int dist = 1; dist <= 12; ++dist) {
                if (!l.is_solid(0, -dist)) {
                    l.move_up(dist);
                    break;
                }
                else if (!l.is_solid(- Help::even(dist), 0)) {
                    l.move_ahead(- Help::even(dist));
                    break;
                }
            }
            if (l.is_solid(0, 0)) {
                // Don't comment this out because others will happily make
                // this a tumbler, then set special_x/y, and those have
                // different meanings for a stunner. See whether this yields
                // any bugs first.
                // l.become(LixEn::STUNNER);
                // return;
            }
        }
        else {
            // Not ascender? Don't tumble at all
            // See comment above
            // l.become(LixEn::STUNNER);
            // return;
        }
    }
    if (l.get_ac() != LixEn::JUMPER
     && l.get_ac() != LixEn::TUMBLER) l.become_default(LixEn::TUMBLER);
    else {
        const int old_spx = l.get_special_x();
        const int old_spy = l.get_special_y();
        l.become_default(LixEn::TUMBLER);
        l.set_special_x(old_spx);
        l.set_special_y(old_spy);
    }
}


void update_jumper (Lixxie& l, const UpdateArgs& ua);
void update_tumbler(Lixxie& l, const UpdateArgs& ua)
{
    update_jumper(l, ua);
}





static const int tumbler_max_special_y = 18; // Ab hier wird er zum Splatter

static void tumbler_land(Lixxie& l)
{
    if (l.get_special_y() > tumbler_max_special_y) {
        const int frame_to_get_second_splatter = 9;
        const int second_splatter_frame = 10;

        // This floater check is just to catch last-moment assignments
        if (l.get_floater()) l.become(LixEn::LANDER);
        else {
            bool second_splatter = (l.get_ac() == LixEn::TUMBLER
                && l.get_frame() >= frame_to_get_second_splatter);
            l.become(LixEn::SPLATTER);
            if ( second_splatter) l.set_frame(second_splatter_frame);
        }
    }
    // become a lander or stunner
    else {
        bool short_anim = (l.get_special_y() < 12);
        if (l.get_ac() == LixEn::JUMPER) {
            l.become(LixEn::LANDER);
            if (short_anim) l.next_frame();
        }
        else {
            l.become(LixEn::STUNNER);
        }
    }
}



// Prueft Boden, Decken und Waende nach jedem einzelnen vorgerueckten Pixel.
// Liefert genau dann wahr, wenn es eine Kollision gab, damit das Vorruecken
// update_jumper/update_tumbler abgebrochen werden kann.

// return values:
//   0 = Wothing hit, keep moving during the same frame if necessary.
//   1 = We hit something and have to stop moving, but we like to remain where
//       we are. Don't rollback the position or encounters.
//   2 = We hit something, but we've already handled positioning out of a
//       wall in this function already. Only reset encounters, then check
//       for new encounters at the position we are.
//   3 = We hit something, we didn't handle the position ourselves. The
//       caller should move back one step to get into air again, and reset
//       encounters.
static inline bool wall  (Lixxie& l, int i) { return l.is_solid( 0, i); }
static inline bool behind(Lixxie& l, int i) { return l.is_solid(-2, i)
                                                  && l.is_solid( 0, i); }

int jumper_and_tumbler_collision(Lixxie& l)
{
    int wall_count   = 0;
    int wall_count_t = 0; // for turning at a wall
    int swh          = 0;
    int lowest_floor = -999; // a default value for "no floor here at all"
    int behind_count = 0;
    for (int i = 1; i > -16; --i) {
        if (wall(l, i)) {
            ++wall_count;
            // i <= -1 is tested because the lowest floor check also starts
            // at -1 and goes further into the negatives. If we don't do that,
            // we might enter not the ascender check, but instead the climber
            // check while being sure that we can't ascend,
            // leading to the ascender in midair bug.
            if (i <= -1 && i > -11) ++wall_count_t;
        }
    }
    // how high is a solid wall starting above (0, 2)?
    for (int i = 1; i > -16; --i) {
        if (wall(l, i)) ++swh;
        else break;
    }

    for (int i = -1; i > -9; --i) {
        behind_count += behind(l, i);
    }

    for (int i = -1; i > -15; --i) {
        if (! wall(l, i-1) && wall(l, i)) {
            lowest_floor = i;
            break;
        }
    }

    // Das Vorruecken zum jeweiligen Pixel ist bereits gemacht.

    // bump head or some body part into really unreasonable ceilings
    if ((behind_count > 0 && l.get_special_y() < 2)
     || (wall(l, -12) && ! wall_count_t && l.get_special_y() <= 0)) {
        if (l.get_ac() != LixEn::TUMBLER) l.become(LixEn::TUMBLER);
        l.set_special_y(4);
        l.set_special_x(l.get_special_x() / 4);
        l.set_special_x(l.get_special_x() * 2);
        if (l.is_solid(0, 0)) return 3;
        else                  return 1;
    }

    // Boden-Kollision
    const bool down = (l.get_special_y() > 0);
    if ( (swh <= 2 && l.is_solid(0, 1) && (l.is_solid(2, 0) || down))
      || (swh <= 2 && l.is_solid(0, 2) && (l.is_solid(2, 1) || down)) )
    {
        while (l.is_solid(0, 1)) l.move_up(1);
        tumbler_land(l);
        return 2;
    }

    // Stepping up a step we jumped onto
    if (lowest_floor != -999 && l.get_ac() == LixEn::JUMPER) {
        if (lowest_floor > -9 || l.get_climber()) {
            l.become(LixEn::ASCENDER);
            return 2;
        }
    }

    // Jumping against a wall
    if (wall_count_t) {
        // Stick to the surface of the wall which we're inside right now
        if (l.get_ac() == LixEn::JUMPER) {
            if (l.get_climber()) {
                l.move_ahead(-2);
                l.become(LixEn::CLIMBER);
                l.set_frame(0);
                return 1;
            }
            else {
                // move out of the wall we're in
                l.turn();
                return 3;
            }
        }
        // we're a tumbler
        else if (wall(l, 1) || (wall(l,  0) && ! behind(l,  0))
                            || (wall(l, -1) && ! behind(l, -1))
                            || (wall(l, -2) && ! behind(l, -2)) ) {
            l.turn();
            return 3;
        }
    }

    // No collisions found at this pixel. We should continue with the remaining
    // motion during this frame, and thus return 0. To remedy glitching into
    // walls though, we return the suggestion to move back if we're stuck in
    // the terrain.
    if (l.is_solid(0, 0)) {
        // The second check for bumping the head, see comment near beginning
        // function.
        if (! l.is_solid(0, 2)) {
            if (l.get_special_y() < 4) l.set_special_y(4);
        }
        return 3;
    }

    // Trampolines stop motion, a bit kludgy, but the results should be good
    if (l.get_foot_encounters() & Lookup::bit_trampoline
          && l.get_special_y() > 0) {
        // stop the motion here, so the trampoline can be used
        return 1;
    }

    return 0;
}



// ############################################################################
// ############################################################################
// ############################################################################



void tumbler_frame_selection(Lixxie& l)
{
    // Last 3 args are the same as in update_jumper().
    int y = l.get_special_y();
    int x = l.get_special_x();
    if (x < 0) x *= -1;
    if (x < 2) x =   2;

    int  tan = y * 12 / x;
    int  tf  = 0;          // tf == target frame
    bool anim = false;     // some poses consist of 2 alternating frames

    if      (tan >  18) tf = 13, anim = true;
    else if (tan >   9) tf = 11, anim = true;
    else if (tan >   3) tf =  9, anim = true;
    else if (tan >   1) tf =  8;
    else if (tan >  -1) tf =  7;
    else if (tan >  -4) tf =  6;
    else if (tan > -10) tf =  5;
    else if (tan > -15) tf =  4;
    else if (tan > -30) tf =  3;
    else if (tan > -42) tf =  2;
    else                tf =  0, anim = true;

    if (l.get_frame() > 0
     && tf > l.get_frame() + (anim ? 2 : 1)) {
        tf = l.get_frame() + (anim ? 2 : 1);
    }

    if (tf == l.get_frame() && anim) ++tf;

    l.set_frame(tf);
}
