/*
 * lix/digger.cpp
 *
 *  special_x
 *
 *      Is 1 immediately after becoming digger. Is set to 0 upon first swing.
 *      When 1, the first swing clears out extra terrain over his feet.
 *
 */

#include "ac.h"

void become_digger(Lixxie& l)
{
    l.become_default(LixEn::DIGGER);
    l.set_special_x(1);
}



void update_digger(Lixxie& l, const UpdateArgs& ua)
{
    // there's only one frame with all the digging action
    if (l.get_frame() != 11) {
        l.next_frame();
        return;
    }

    // we're in frame 11, where the action happens
    for (int y = 0; y < 4; ++y) {
        // Stop digging if there is steel to the left as well as to the right
        // of the digger's center, or in the 3 double pixels of the center.
        // The following two rectangles overlap, that implements the center.
        int steel_left   = l.count_steel(-8, 2, 3, 2);
        int steel_right  = l.count_steel(-2, 2, 9, 2);

        if (steel_left > 0 && steel_right > 0) {
            l.become(LixEn::WALKER);
            l.play_sound(ua, Sound::STEEL);
            l.get_ef()->add_dig_hammer(ua.st.update, l.get_tribe(), ua.id,
                l.get_ex(), l.get_ey() - 10);
            return; // from update_digger() entirely
            // if steel cancelling happens on the very first swing, the upswing
            // (next if) doesn't happen
        }
        l.remove_rectangle(-8, 2, 9, 2);
        if (l.get_special_x() == 1) {
            // Remove extra earth upon first swing: four hi-res rows, like L1.
            l.remove_rectangle(-8, -2, 9, 1);
            l.set_special_x(0);
        }
        l.move_down(1);

        if (! l.is_solid(-2, 2)
         && ! l.is_solid( 0, 2)
         && ! l.is_solid( 2, 2)
        ) {
            l.become(LixEn::FALLER);
            return;
        }
    }
    l.next_frame();
}
