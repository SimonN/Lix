/*
 * lix/basher.cpp
 *
 * Steel more than 16 pixels over the floor is ignored for sake of
 * basher cancelling. This allows much more nicely aligned steel.
 *
 * special_x
 *
 *   Measures how much the lix has descended during bashing. If this becomes
 *   too much, she will stop.
 *
 *   The basher will go up again if he's walked through a very small pit while
 *   bashing, to counter Horus's bug. (This was in 2009, I have since forgot
 *   what that bug was.)
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

void become_basher(Lixxie& l)
{
    l.become_default(LixEn::BASHER);
    l.set_frame(2);
}




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
        // Do the check whether to leave remains or not. We will leave remains
        // unless this is the last swing, and the following column is empty.
        {
            // The 0 in the latter argument is correct, since the
            // check to become walker will be done from this location also.
            bool omit_remains = nothing_more_to_bash(l, 0)
                         && l.count_solid(14, -16, 15, 1) == 0;
            if (omit_remains) l.set_special_y(l.get_special_y() | 2);
            else              l.set_special_y(l.get_special_y() & ~2);
        }
        break;

    case 6:
        // This is the only frame with terrain removal.
        // Steel is only checked in the bottom 16 pixels. The basher tunnel
        // has a height of 18, we want to ignore hitting steel in the top rows.

        if ((l.get_special_y() & 2) == 0) {
            // leave relics
                         l.remove_rectangle(  0, -16,   9, -16); // top relic
                         l.remove_rectangle(  0, -15,  11, -15); // top relic
            steel_hit += l.remove_rectangle(  0, -14,  12, -14); // top relic
            steel_hit += l.remove_rectangle(  0, -13,  13,  -2); // large rect
            steel_hit += l.remove_rectangle(  0,  -1,  12,  -1); // bottom
            steel_hit += l.remove_rectangle(  0,   0,  11,   0); // bottom
            steel_hit += l.remove_rectangle(  0,   1,   9,   1); // bottom
        }
        else {
            // Leave no relics. Remove a large rectangle.
                         l.remove_rectangle(  0, -16,  13, -15);
            steel_hit += l.remove_rectangle(  0, -14,  13,   1);
        }

        if (steel_hit && ! (l.get_special_y() & 1)) {
            l.set_special_y(l.get_special_y() | 1);
            l.play_sound(ua, Sound::STEEL);
            // do not cancel the basher yet, this will happen later
        }
        break;

    case 9:
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

    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        l.move_ahead();
        break;

    default:
        break;
    }

    // How many pixels have we descended? 9 pixels or more make her a faller.
    // The pixels inside the foot must be air, too. Otherwise, a basher can
    // fall in the tip of his tunnel, but the walkers following would not.
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

    // Lix too high? Then become faller, otherwise set lower special_x.
    if (l.get_special_x() > 8) {
        l.move_up(1);
        --downwards_movement_this_frame;
        l.become(LixEn::FALLER);
        // count the downward movement towards max fall distance
        l.set_special_x(downwards_movement_this_frame);
    }
    else if (l.get_special_x() > 0) {
        l.set_special_x(l.get_special_x() - 1);
    }

    // If nothing has happened, select the next basher frame.
    if (l.get_ac() == LixEn::BASHER)
        l.next_frame();
}
