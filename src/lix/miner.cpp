/*
 * lix/miner.cpp
 *
 */

#include "ac.h"

void update_miner(Lixxie& l, const UpdateArgs& ua)
{
    int steel_hit = 0;
    int downwards_movement_this_frame = 0; // miner->faller gets no free pixels
    bool allow_one_air_under_foot = false; // make the terrain check as coarse
                                           // as before commit 4fc7b0b1ab9
    switch (l.get_frame()) {
    case 1:
        // This is the only frame with terrain removal

        steel_hit += l.remove_rectangle(  3, -20,  7, -20);
        steel_hit += l.remove_rectangle(  1, -19,  9, -19);

        steel_hit += l.remove_rectangle(  0, -18, 10, -18);
        steel_hit += l.remove_rectangle(  0, -17, 11, -17);

        steel_hit += l.remove_rectangle(  0, -16, 12, -15);
        steel_hit += l.remove_rectangle(  0, -14, 13, -11);

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

        break;

    case 6:
        l.move_ahead();
        l.move_down(1);
        downwards_movement_this_frame = 1;
        allow_one_air_under_foot = true;
        // the spritesheet currently look as if she moves according to a
        // low-res mining slope, let's fix that like so for now
        l.set_y(l.get_y() + 1);
        break;

    case 7:
        l.move_ahead();
        l.move_down(1);
        downwards_movement_this_frame = 1;
        // here, the graphics match again
        break;

    case 9:
        l.move_ahead();
        l.move_down(1);
        downwards_movement_this_frame = 1;
        allow_one_air_under_foot = true;
        // the spritesheet currently look as if she moves according to a
        // low-res mining slope, let's fix that like so for now
        l.set_y(l.get_y() + 1);
        break;

    case 10:
        l.move_ahead();
        l.move_down(1);
        downwards_movement_this_frame = 1;
        // here, the graphics match again
        break;

    }

    // if we haven't thrown the axe earlier
    if (l.get_ac() == LixEn::MINER) {
        bool done_mining = false;
        if (allow_one_air_under_foot) {
             done_mining = !l.is_solid() && !l.is_solid(0, 3);
        }
        else done_mining = !l.is_solid();

        if (done_mining) {
            l.become(LixEn::FALLER);
            l.set_special_x(downwards_movement_this_frame);
        }
        else l.next_frame();
    }

}
