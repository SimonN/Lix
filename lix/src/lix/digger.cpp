/*
 * lemming/digger.cpp
 *
 * special_x
 *
 *   This is 1 if the digger has just been activated. Some earth will
 *   be dug away over its head on the first swing.
 *
 */

#include "ac.h"

void assign_digger(Lixxie& l)
{
    l.set_special_x(1);
    l.set_special_y(0);
    l.set_ac(LixEn::DIGGER);

    // Start digging faster. This is important in multiplayer to stop
    // opponent's diggers with one's own.
    l.set_frame(1);
}



void update_digger(Lixxie& l, const UpdateArgs& ua)
{
    // In zwei aufeinanderfolgenden Frames wird jeweils eine Pixelschicht
    // weggegraben, somit wird in einem Durchlauf ein Doppelpixel an Höhe
    // entfernt.
    // Es kann nur dann nicht durch Stahl gegraben werden, wenn sowohl links
    // als auch rechts vom Lemming zu viel Stahl ist. Ansonsten bekommt man
    // einen mindestens halb so grossen Tunnel wie beim freien Graben.
    if (l.get_frame() % 8 == 3
     || l.get_frame() % 8 == 4) {
        if (l.count_steel(-8, 2, 1, 2)
         && l.count_steel( 0, 2, 9, 2)) {
            l.turn();
            l.assign(LixEn::WALKER);
            l.set_frame(1);
            l.play_sound(ua, Sound::STEEL);
        }
        else {
            l.remove_rectangle(-8, 2, 9, 2);
            l.move_down(1);
            l.next_frame();
        }
    }
    // Sonst immer: Es wird geprüft, ob der Lemming zu Fallen beginnen
    // soll. Dazu gucke man nach, ob es noch Nicht-Stahl-Pixel gibt.
    // The args were -4, 1, 5, 2, but were shortened horizontally to make it
    // easier to overtake opponent's diggers.
    else if (l.count_solid(-2, 1, 3, 2)
          -  l.count_steel(-2, 1, 3, 2) <= 0
         && !l.is_solid(0, 2))
    {
        l.move_down();
        l.assign(LixEn::FALLER);
        while (l.is_solid()) {
            l.move_up(1);
            l.assign(LixEn::LANDER);
        }
    }

    // Falls der Lemming gerade erst angeklickt worden ist,
    // muss etwas Erde über seiner Position entfernt werden.
    else if (l.get_special_x() == 1) {
        l.set_special_x(0);
        l.remove_rectangle(-8, -4, 9, 1);
        l.next_frame();
    }
    else l.next_frame();
}
