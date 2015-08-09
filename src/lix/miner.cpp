/*
 * lix/miner.cpp
 *
 * special_x
 *
 *   Bit field. In the 5th frame of the miner, right before it advances
 *   forward, we check if the pixels the lix will move to are solid.
 *   So the i-th bit of this bitfield tells us if the pixel at the i-th future
 *   position is solid.
 *   Basically, the miner behaves as it has always behaved before August 2015
 *   if no terrain is removed under him. It is only allowed to move down in
 *   two cases:
 *   (1) It's not moving forward.
 *   (2) It's moving forward, but the path was solid before it started moving.
 *
 * special_y
 *
 *   Counts how many pixels the miner has been moved down during the frames
 *   where the lix is not moving forward. This can happen due to terrain
 *   being removed below the lix. When this counter exceeds 4, the lix stops
 *   mining. This counter is reset after a miner swing.
 *
 */

#include <algorithm> // std::max

#include "ac.h"

static const int MAX_GAP_DEPTH = 3;

static int move_miner_down(Lixxie& l)
{
    int downwards_movement_this_frame = 0;

    // check the up to MAX_GAP_DEPTH (or less if already fallen earlier) pizels
    while (!l.is_solid()
        && downwards_movement_this_frame + l.get_special_y() < MAX_GAP_DEPTH
    ) {
        l.move_down(1);
        ++downwards_movement_this_frame;
    }
    if (!l.is_solid())
        // still no ground after moving down -> move back up and
        // cancel miner later in the code
        l.move_up(MAX_GAP_DEPTH - l.get_special_y());
    else
        l.set_special_y(l.get_special_y() + downwards_movement_this_frame);

    return downwards_movement_this_frame;
}



void update_miner(Lixxie& l, const UpdateArgs& ua)
{
    int steel_hit = 0;
    bool done_mining = false;
    int downwards_movement_this_frame = 0; // miner->faller gets no free pixels
    //bool allow_one_air_under_foot = false;// make the terrain check as coarse
                                           // as before commit 4fc7b0b1ab9
    switch (l.get_frame()) {
    case 1:
        // this was in case 0 previously. But we want to remove all the
        // terrain in a single frame now.
        steel_hit += l.remove_rectangle(  3, -20,  7, -20);
        steel_hit += l.remove_rectangle(  1, -19,  9, -19);

        steel_hit += l.remove_rectangle(  0, -18, 10, -18);
        steel_hit += l.remove_rectangle(  0, -17, 11, -17);

        steel_hit += l.remove_rectangle(  0, -16, 12, -15);
        steel_hit += l.remove_rectangle(  0, -14, 13, -11);

        // Nach diesem Frame ist die Hacke unten,
        // dann wird auch der Hauptteil der Erde entfernt.
        steel_hit += l.remove_pixel    (11, -18);
        steel_hit += l.remove_rectangle(12, -17, 13, -17);

        steel_hit += l.remove_rectangle(13, -16, 14, -16);
        steel_hit += l.remove_rectangle(13, -15, 15, -15);

        steel_hit += l.remove_rectangle(14, -14, 16, -13);
        steel_hit += l.remove_rectangle(14, -12, 17, -11);

        steel_hit += l.remove_rectangle( 0, -10, 17, - 1);
        steel_hit += l.remove_rectangle( 0,   0, 16, + 1);

        steel_hit += l.remove_rectangle( 2,   2, 15,   2);
        steel_hit += l.remove_rectangle( 4,   3, 14,   3);

        steel_hit += l.remove_rectangle( 6,   4, 13,   4);
        steel_hit += l.remove_rectangle( 8,   5, 11,   5);

        if (steel_hit > 0) {
            // Hacke muss je nach Frame und Drehung, die noch nicht stattfand,
            // richtig positioniert und gedreht werden (0 = Hackenkopf unten
            // rechts, ferner +1 = Drehung um 90 Grad nach rechts).
            int axe_rotation = l.get_dir() < 0;
            // this cannot occur when remembering in frame 0 to cancel later:
            // if (l.get_frame() == 0) axe_rotation = 3 - axe_rotation;
            l.get_ef()->add_pickaxe(ua.st.update, l.get_tribe(), ua.id,
             l.get_ex() + 10 * l.get_dir(),
             l.get_frame() == 1 ? l.get_ey()-2 : l.get_ey()-16, axe_rotation);
            l.turn();
            l.become(LixEn::WALKER);
            l.play_sound(ua, Sound::STEEL);
        }

        // reset the counter for allowed downward movement per frame.
        l.set_special_y(0);
        downwards_movement_this_frame = move_miner_down(l);
        done_mining = (l.get_special_y() > MAX_GAP_DEPTH) || !l.is_solid();
        break;

    // This lists all other frames that have no case. We don't want to use
    // 'default:' to make explicit the two ranges of frames.
    case 2:
    case 3:
    case 4:

    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case  0:
        downwards_movement_this_frame = move_miner_down(l);
        done_mining = (l.get_special_y() > MAX_GAP_DEPTH) || !l.is_solid();
        break;

    case 5:
        // for the next 4 columns of pixels, check if the terrain under the
        // future position of the lix is solid.
        // We have to use l.get_special_y() - j because the lix might have
        // been moved down this frame already, and then it advances
        // horizontally for a few frames
        l.set_special_x(0);
        for (int j = 0; j < 4; ++j) {
            if (l.is_solid(2*j + 2,
                           2 + (j+1) + std::max(l.get_special_y() - j, 0))
            ) {
                l.set_special_x(l.get_special_x() | (1 << j));
            }
        }
        downwards_movement_this_frame = move_miner_down(l);
        done_mining = (l.get_special_y() > MAX_GAP_DEPTH) || !l.is_solid();
        break;

    case 6:
    case 9:
        l.move_ahead();
        // if we already moved down after the miner stroke,
        // then move horizontally now
        if (l.get_special_y() == 0) {
            l.move_down(1);
            downwards_movement_this_frame = 1;
            // the spritesheet currently look as if she moves according to a
            // low-res mining slope, let's fix that like so for now
            l.set_y(l.get_y() + 1);
        }
        else {
            l.set_special_y(l.get_special_y() - 1);
        }

        //  && !l.is_solid(0, 3) --> allow_one_air_under_foot
        if (l.get_frame() == 6) {
            if (l.get_special_x() & (1 << 0))
                downwards_movement_this_frame = move_miner_down(l);
            done_mining = !l.is_solid()
                       && !l.is_solid(0, 3)
                       && !(l.get_special_x() & (1 << 0));
        }
        else {
            if (l.get_special_x() & (1 << 2))
                downwards_movement_this_frame = move_miner_down(l);
            done_mining = !l.is_solid()
                       && !l.is_solid(0, 3)
                       && !(l.get_special_x() & (1 << 2));
        }

        break;

    case 7:
    case 10:
        l.move_ahead();
        // if we already moved down after the miner stroke,
        // then move horizontally now
        if (l.get_special_y() == 0) {
            l.move_down(1);
            downwards_movement_this_frame = 1;
        }
        else {
            l.set_special_y(l.get_special_y() - 1);
        }
        // here, the graphics match again

        // don't allow_one_air_under_foot
        if (l.get_frame() == 7) {
            if (l.get_special_x() & (1 << 1))
                downwards_movement_this_frame = move_miner_down(l);
            done_mining = !l.is_solid() && !(l.get_special_x() & (1 << 1));
        }
        else {
            if (l.get_special_x() & (1 << 3))
                downwards_movement_this_frame = move_miner_down(l);
            done_mining = !l.is_solid() && !(l.get_special_x() & (1 << 3));
        }
        break;

    case 8:
        if (l.get_special_x() & (1 << 1))
            downwards_movement_this_frame = move_miner_down(l);
        // don't allow_one_air_under_foot
        done_mining = !l.is_solid() && !(l.get_special_x() & (1 << 1));

    }

    // if we haven't thrown the axe earlier
    if (l.get_ac() == LixEn::MINER) {
        if (done_mining) {
            l.become(LixEn::FALLER);
            l.set_special_x(downwards_movement_this_frame);
        }
        else l.next_frame();
    }

}
// end function update_miner
