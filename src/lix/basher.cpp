/*
 * lix/basher.cpp
 *
 * The basher will bash away the lowest rectangle of most frames again during
 * the next frame. This is to prevent the horizontal lines when he's bashing
 * inside a digger's pit. These lines will be commented with "re-bash".
 *
 * DEBUGGING: implement re-bashing for the Lix again.
 *
 * special_x
 *
 *   Misst, wie weit eine Lix hinuntergestiegen ist beim Bohren. Wenn das
 *   zu viel war, hoert sie auf. Siehe ausfuehrlichen Kommentar unten.
 *
 *   The basher will go up again if he's walked through a very small pit while
 *   bashing, to counter Horus' bug.
 *
 * special_y
 *
 *   This takes the values 0 or 1.
 *
 *   0: This is 0 until the lix has completed half of its first swing.
 *   While it's 0, steel doesn't cancel the basher.
 *
 *   1: This is 1 during the second half of the first swing, until the check
 *   to continue bashing has been completed.
 *
 *   The idea was originally to make another value for this, 2, after the
 *   first check for enough earth to continue bashing. We want bridges to be
 *   destroyed by bashers, but currently, the regular earth check will ensure
 *   bridges get destroyed anyway.
 *
 */

#include "ac.h"

void update_basher(Lixxie& l, const UpdateArgs& ua)
{
    int steel_hit = 0;

    switch (l.get_frame()) {
    case 0:
        steel_hit += l.remove_rectangle(  0, -16,   5, -16);
        steel_hit += l.remove_rectangle(  0, -15,   4, -15);
        steel_hit += l.remove_rectangle(  0, -14,   3, -14);
        steel_hit += l.remove_rectangle(  0, -13,   2, -13);
        steel_hit += l.remove_rectangle(  0, -12,   1, -12);
        steel_hit += l.remove_rectangle(  0, -11,   0, -11);
        break;

    case 1:
        steel_hit += l.remove_rectangle(  6, -16,   9, -16);
        steel_hit += l.remove_rectangle(  5, -15,   8, -15);
        steel_hit += l.remove_rectangle(  4, -14,   7, -14);
        steel_hit += l.remove_rectangle(  3, -13,   6, -13);
        steel_hit += l.remove_rectangle(  2, -12,   4, -12);
        steel_hit += l.remove_rectangle(  0, -11,   1, -11);
        break;

    case 2:
        steel_hit += l.remove_rectangle(  9, -15,  11, -15);
        steel_hit += l.remove_rectangle(  8, -14,  12, -14);
        steel_hit += l.remove_rectangle(  7, -13,  10, -13);
        steel_hit += l.remove_rectangle(  5, -12,   6, -12);
        break;

    case 3:
        steel_hit += l.remove_rectangle( 11, -13,  12, -13);
        steel_hit += l.remove_rectangle(  7, -12,  13, -12);
        steel_hit += l.remove_rectangle(  2, -11,  13, -11);
        steel_hit += l.remove_rectangle(  0, -10,  13, -10);
        break;

    case 4:
        steel_hit += l.remove_rectangle(  0,  -9,  13,  -9);
        steel_hit += l.remove_rectangle(  0,  -8,  13,  -8);
        steel_hit += l.remove_rectangle(  0,  -7,  13,  -7);
        steel_hit += l.remove_rectangle(  5,  -6,  13,  -6);
        steel_hit += l.remove_rectangle(  9,  -5,  13,  -5);
        break;

    case 5:
        if (l.get_special_y() == 0) l.set_special_y(1);

        steel_hit += l.remove_rectangle(  0,  -6,   4,  -6);
        steel_hit += l.remove_rectangle(  3,  -5,   8,  -5);
        steel_hit += l.remove_rectangle(  5,  -4,  13,  -4);
        steel_hit += l.remove_rectangle(  7,  -3,  13,  -3);
        steel_hit += l.remove_rectangle(  9,  -2,  12,  -2);
        steel_hit += l.remove_rectangle( 11,  -1,  12,  -1);
        break;

    case 6:
        steel_hit += l.remove_rectangle(  0,  -5,   2,  -5); // everything
        steel_hit += l.remove_rectangle(  0,  -4,   4,  -4); // everything
        steel_hit += l.remove_rectangle(  4,  -3,   6,  -3);
        steel_hit += l.remove_rectangle(  5,  -2,   8,  -2);
        steel_hit += l.remove_rectangle(  5,  -1,  10,  -1);
        steel_hit += l.remove_rectangle(  6,   0,  11,   0);
        steel_hit += l.remove_rectangle(  7,   1,   9,   1);
        break;

    case 7:
        // all the remaining land
        steel_hit += l.remove_rectangle(  0,  -3,   3,  -3);
        steel_hit += l.remove_rectangle(  0,  -2,   4,  -2);
        steel_hit += l.remove_rectangle(  0,  -1,   4,  -1);
        steel_hit += l.remove_rectangle(  0,   0,   5,   0);
        steel_hit += l.remove_rectangle(  0,   1,   6,   1);
        break;

    case 8:
        // Gibt es noch etwas zu Bohren?
        // Hier nehmen wir nicht alle Pixel am Fuß
        // oder am Kopf der Lix mit.

        // Auch die von der Lix am weitesten entfernten Doppelpixel
        // werden nicht überbrückt.

        // Alles wäre dagegen 12, -16, 23, +1

        if (l.count_solid(12, -14, 21, -3) < 15) {
            bool stop_bashing = true;
            // Tills Bohr-Problem: Nochmal auf hauchduenne Waende pruefen
            for (int x = 12; x <= 23; x += 2)
                if (l.solid_wall_height(x) > 8) stop_bashing = false;
            if (stop_bashing) l.become(LixEn::WALKER);
        }
        break;

    case  9:
    case 10:
    case 11:
    case 12:
    case 13:
        l.move_ahead();
        break;
    }

    // Wurde Stahl getroffen?
    // Die Object-Sachen können ruhig noch ausgeführt werden.
    if (steel_hit && l.get_special_y() != 0) {
        l.turn();
        l.become(LixEn::WALKER);
        l.play_sound(ua, Sound::STEEL);
    }

    // Object misst, wie viele Pixel kürzlich hinunter gestiegen wurden
    // Ab 9 Object-Punkten wird die Lix zum Faller.
    // Die Pixel, in dem die Lix steht, müssen auch hohl sein, denn
    // sonst fällt ein Basher im Zipfel seines Ganges, aber die Kollegen
    // nicht!
    while (!l.is_solid() && !l.is_solid(0, 1) && !l.is_solid(0, 0)
             && l.get_special_x() < 9) {
        l.move_down(1);
        l.set_special_x(l.get_special_x() + 2);
    }
    // Get up again to counter the Horus bug, but never go up further than
    // the basher started initially.
    while (l.get_special_x() > 0 && l.is_solid() && l.is_solid(0, 1)) {
        l.move_up(1);
        l.set_special_x(l.get_special_x() - 2);
        if (l.get_special_x() < 0) l.set_special_x(0);
    }

    // Object zu hoch? Dann Faller werden, sonst special_x senken.
    if (l.get_special_x() > 8) {
        l.move_up(1);
        l.become(LixEn::FALLER);
    }
    else if (l.get_special_x() > 0) l.set_special_x(l.get_special_x() - 1);

    // Wenn wirklich alles okay war, nächstes Basher-Frame setzen!
    if (l.get_ac() == LixEn::BASHER) l.next_frame();
}
