/*
 * lix/cuber.cpp
 *
 */

#include "ac.h"

void become_cuber(Lixxie& l)
{
    l.become_default(LixEn::CUBER);
    if (l.get_dir() < 0) {
        l.turn();       // move_ahead() makes the two directions balanced,
        l.move_ahead(); // just as the hatch spawn positions' move_ahead()
    }
}

void update_cuber(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) {
        l.become(LixEn::NOTHING);
        l.draw_frame_to_map(7, LixEn::CUBER - 1,
                            8, 12, 16, 16,
                            -8, 2-16);
        // 7 is the frame containing the block
    } else {
        /* Measure against getting stuck.
         * For each late frame, 2 additional pixel layers of the solid block
         * are drawn and get solid. */
        int height = 2*l.get_frame()-2;
        if (height > 0) l.draw_frame_to_map(7, LixEn::CUBER - 1,
                            8, 12+16-height, 16, height,
                            -8, 2-height);
        l.next_frame();
    }
}
