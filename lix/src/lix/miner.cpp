/*
 * lemming/miner.cpp
 *
 * special_x
 *
 *   1 during the very first frames. Steel doesn't have an effect during the
 *   first part of the first swing. This is set to 0 after the first swing.
 *
 */

#include "ac.h"

void assign_miner(Lixxie& l)
{
    l.assign_default(LixEn::MINER);
    l.set_special_x(1);
}

void update_miner(Lixxie& l, const UpdateArgs& ua)
{
    int steel_hit = 0;

    switch (l.get_frame()) {
    case 0:
        steel_hit += l.remove_rectangle(  3, -20,  7, -20);
        steel_hit += l.remove_rectangle(  1, -19,  9, -19);

        steel_hit += l.remove_rectangle(  0, -18, 10, -18);
        steel_hit += l.remove_rectangle(  0, -17, 11, -17);

        steel_hit += l.remove_rectangle(  0, -16, 12, -15);
        steel_hit += l.remove_rectangle(  0, -14, 13, -11);

        // Don't stop mining before the very first swing is complete at least
        if (l.get_special_x() == 1) steel_hit = 0;
        l.set_special_x(0);
        break;

    case 1:
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
        break;

    case 6:
        l.move_ahead();
        l.move_down();
        break;

    case 7:
        l.move_ahead();
        break;

    case 9:
        l.move_ahead();
        l.move_down();
        break;

    case 10:
        l.move_ahead();
        break;

    }

    // Stahl getroffen?
    // Sonst: Wenn er noch gräbt, das nächste Frame auswählen
    if (steel_hit) {
        // Hacke muss je nach Frame und Drehung, die noch nicht stattfand,
        // richtig positioniert und gedreht werden (0 = Hackenkopf unten
        // rechts, ferner +1 = Drehung um 90 Grad nach rechts).
        int axe_rotation = l.get_dir() < 0;
        if (l.get_frame() == 0) axe_rotation = 3 - axe_rotation;
        l.get_ef()->add_pickaxe(ua.upd, l.get_tribe(), ua.id,
         l.get_ex() + 10, l.get_frame() == 1 ? l.get_ey() - 2 : l.get_ey() -16,
         axe_rotation);
        l.turn();
        l.assign(LixEn::WALKER);
        l.play_sound(ua, Sound::STEEL);
    }

    // Fertig gegraben?
    else if (!l.is_solid( 0, 2) && !l.is_solid( 0, 1)) {
        l.assign(LixEn::FALLER);
    }
    else if (l.get_ac() == LixEn::MINER) l.next_frame();
}
