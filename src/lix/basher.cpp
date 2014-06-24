/*
 * lix/basher.cpp
 *
 * Steel more than 16 pixels over the floor is ignored for sake of
 * basher cancelling. This allows much more nicely aligned steel.
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
 *   This is a bit field.
 *
 * special_y & 1
 *
 *   This is 0 until steel was hit, then it becomes 1 and is never reset to
 *   0 in the future. The lix will stop bashing after the cycle.
 *
 * special_y & 2
 *
 *   If this is set, then leave no basher remains. We usually don't want
 *   basher remains on the last swing.
 *
 */

#include "ac.h"

static bool nothing_more_to_bash(Lixxie& l, const int px)
{
    // px: use 0 if you want the normal check. Use 10 if you want to check
    // whether there was nothing to bash if we were 10 pixels ahead already.

    // Gibt es noch etwas zu Bohren?
    // Hier nehmen wir nicht alle Pixel am Fuß
    // oder am Kopf der Lix mit.

    // Auch die von der Lix am weitesten entfernten Doppelpixel
    // werden nicht überbrückt.

    // Alles wäre dagegen 14, -16, 23, +1
    bool ret = false;
    if (l.count_solid(14 + px, -14, 21 + px, -3) < 15) {
        ret = true;
        // Tills Bohr-Problem: Nochmal auf hauchduenne Waende pruefen
        for (int x = 14 + px; x <= 21 + px; x += 2)
            if (l.is_solid(x, -12)) ret = false;
    }
    return ret;
}



void update_basher(Lixxie& l, const UpdateArgs& ua)
{
    int steel_hit = 0;

    switch (l.get_frame()) {
    case 0:
        // do the check whether to leave remains or not. We will leave remains
        // unless this is the last swing, and the following column is empty.
        {
            // The 0 in the latter argument is actually correct, since the
            // check to become walker will be done from this location also.
            bool omit_remains = nothing_more_to_bash(l, 0)
                         && l.count_solid(14, -16, 15, 1) == 0;
            if (omit_remains) l.set_special_y(l.get_special_y() | 2);
            else              l.set_special_y(l.get_special_y() & ~2);
        }
                     l.remove_rectangle(  0, -16,   5, -16);
                     l.remove_rectangle(  0, -15,   4, -15);
        steel_hit += l.remove_rectangle(  0, -14,   3, -14);
        steel_hit += l.remove_rectangle(  0, -13,   2, -13);
        steel_hit += l.remove_rectangle(  0, -12,   1, -12);
        steel_hit += l.remove_rectangle(  0, -11,   0, -11);
        break;

    case 1:
                     l.remove_rectangle(  6, -16,   9, -16);
                     l.remove_rectangle(  5, -15,   8, -15);
        steel_hit += l.remove_rectangle(  4, -14,   7, -14);
        steel_hit += l.remove_rectangle(  3, -13,   6, -13);
        steel_hit += l.remove_rectangle(  2, -12,   4, -12);
        steel_hit += l.remove_rectangle(  0, -11,   1, -11);
        break;

    case 2:
        if (l.get_special_y() & 2) {
                     l.remove_rectangle( 10, -16,  13, -16);
                     l.remove_rectangle( 12, -15,  13, -15);
        }
                     l.remove_rectangle(  9, -15,  11, -15);
        steel_hit += l.remove_rectangle(  8, -14,  12, -14);
        steel_hit += l.remove_rectangle(  7, -13,  10, -13);
        steel_hit += l.remove_rectangle(  5, -12,   6, -12);
        break;

    case 3:
        if (l.get_special_y() & 2) {
          steel_hit+=l.remove_rectangle( 13, -14,  14, -13);
        }
        steel_hit += l.remove_rectangle( 13, -13,  13, -13); // less relics
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
        steel_hit += l.remove_rectangle(  0,  -6,   4,  -6);
        steel_hit += l.remove_rectangle(  3,  -5,   8,  -5);
        steel_hit += l.remove_rectangle(  5,  -4,  13,  -4);
        steel_hit += l.remove_rectangle(  7,  -3,  13,  -3);
        steel_hit += l.remove_rectangle(  9,  -2,  12,  -2);
        steel_hit += l.remove_rectangle( 11,  -1,  12,  -1);
        steel_hit += l.remove_rectangle( 13,  -2,  13,  -2); // less relics
        if (l.get_special_y() & 2) {
          steel_hit+=l.remove_rectangle( 13,  -1,  13,  -1);
        }
        break;

    case 6:
        steel_hit += l.remove_rectangle(  0,  -5,   2,  -5); // everything
        steel_hit += l.remove_rectangle(  0,  -4,   4,  -4); // everything
        steel_hit += l.remove_rectangle(  4,  -3,   6,  -3);
        steel_hit += l.remove_rectangle(  5,  -2,   8,  -2);
        steel_hit += l.remove_rectangle(  5,  -1,  10,  -1);
        steel_hit += l.remove_rectangle(  6,   0,  11,   0);
        steel_hit += l.remove_rectangle(  7,   1,   9,   1);
        if (l.get_special_y() & 2) {
          steel_hit+=l.remove_rectangle( 12,   0,  13,   0);
          steel_hit+=l.remove_rectangle( 10,   1,  13,   1);
        }
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
        // stop if we hit steel or if there's nothing more to bash
        // the sound is not played here, it's played upon detecting steel
        // for the first time in an earlier frame
        if (l.get_special_y() & 1) {
            l.turn();
            l.become(LixEn::WALKER);
        }
        else if (nothing_more_to_bash(l, 0)) {
            l.become(LixEn::WALKER);
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

    if (steel_hit && ! (l.get_special_y() & 1)) {
        l.set_special_y(l.get_special_y() | 1);
        l.play_sound(ua, Sound::STEEL);
        // do not cancel the basher yet, this will be done in the switch above
    }

    // Object misst, wie viele Pixel kürzlich hinunter gestiegen wurden
    // Ab 9 Object-Punkten wird die Lix zum Faller.
    // Die Pixel, in dem die Lix steht, müssen auch hohl sein, denn
    // sonst fällt ein Basher im Zipfel seines Ganges, aber die Kollegen
    // nicht!
    int downwards_movement_this_frame = 0;
    while (!l.is_solid() && !l.is_solid(0, 1) && !l.is_solid(0, 0)
             && l.get_special_x() < 9) {
        l.move_down(1);
        ++downwards_movement_this_frame;
        l.set_special_x(l.get_special_x() + 2);
    }
    // Get up again to counter the Horus bug, but never go up further than
    // the basher started initially.
    while (l.get_special_x() > 0 && l.is_solid() && l.is_solid(0, 1)) {
        l.move_up(1);
        --downwards_movement_this_frame;
        l.set_special_x(l.get_special_x() - 2);
        if (l.get_special_x() < 0) l.set_special_x(0);
    }

    // Object zu hoch? Dann Faller werden, sonst special_x senken.
    if (l.get_special_x() > 8) {
        l.move_up(1);
        --downwards_movement_this_frame;
        l.become(LixEn::FALLER);
        // count the downward movement towards max fall distance
        l.set_special_x(downwards_movement_this_frame);
    }
    else if (l.get_special_x() > 0) l.set_special_x(l.get_special_x() - 1);

    // Wenn wirklich alles okay war, nächstes Basher-Frame setzen!
    if (l.get_ac() == LixEn::BASHER) l.next_frame();
}
