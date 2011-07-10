/*
 * lemming/platform.cpp
 *
 */

#include "ac.h"

void assign_platformer(Lixxie& l)
{
    l.set_special_y(0);
    if (l.get_ac() == LixEn::PLATFORMER) {
        l.set_special_x(l.get_special_x() + 12);
    }
    else {
        l.set_special_x(12);
        // Der Shrugger soll auf gleicher Hoehe weiterbauen
        if (l.get_ac() == LixEn::SHRUGGER2
         && l.get_frame() < 11) {
            l.set_frame(16);
        }
        else l.set_frame(0);
        l.set_ac(LixEn::PLATFORMER);
    }
}



void update_platformer(Lixxie& l, const UpdateArgs& ua)
{
    // Der Platformer hat zwei Zyklen. Im ersten Zyklus baut der den
    // Stein oberhalb der Laufhoehe, im allen nachfolgenden Zyklen weiter
    // unten.
    switch (l.get_frame()) {

    // Bauen
    case  1:
        l.set_special_x(l.get_special_x() - 1);
        l.draw_brick   (0, 0, 7, 1);
        if (l.get_special_x() < 3) l.play_sound_if_trlo(ua, Sound::BRICK);
        break;

    case 17:
        l.set_special_x(l.get_special_x() - 1);
        l.draw_brick   (4, 2, 9, 3);
        if (l.get_special_x() < 3) l.play_sound_if_trlo(ua, Sound::BRICK);
        break;

    case  4:
        // Sinnvolle Steinverlegung wie bei Frame 25 pruefen:
        // Kompletten naechsen Stein vorausplanen, bei Kollision allerdings
        // anders als in Frame 25 umdrehen. Man kann also direkt vor eine
        // Wand bauen, um umzudrehen.
        if (l.is_solid(6, 0) && l.is_solid(8, 0) && l.is_solid(10, 0)) {
            l.assign(LixEn::WALKER);
            l.turn();
        }
        break;

    case  6:
        if (!l.is_solid(0, -2)) l.move_up();
        else {
            l.assign(LixEn::SHRUGGER2);
            l.set_frame(8);
            l.turn();
        }
        // faellt durch

    case  7:
    case 21:
    case 22:
    case 23:
        // 2, 1 statt 2, 0, weil wir direkt ueber dem Boden pruefen wollen,
        // ob da ein Pixel ist. Sonst laufen die Walker durch.
        if (!l.is_solid(2, 1)) l.move_ahead();
        else {
            l.assign(LixEn::SHRUGGER2);
            l.set_frame(9);
        }
        break;

    case 25: // durchaus auch das letzte Frame
        // Kann noch ein Stein verlegt werden?
        if (l.get_special_x() == 0) {
            l.assign(LixEn::SHRUGGER2);
            l.set_special_y(2); // Bei Klick 2 tiefer anfng. = weiterbauen
        }
        else if (l.is_solid(2, 2)
         &&      l.is_solid(4, 2)
         &&      l.is_solid(6, 2)) {
            // In die aktuelle Richtung weiterlaufen! Dies unterscheidet
            // sich von der Vorausplanung nach dem allerersten Stein, dort
            // wird gedreht.
            l.assign(LixEn::SHRUGGER2);
            l.set_frame(9);
        }
        break;
    }

    if (l.get_ac() == LixEn::PLATFORMER) {
        if (l.is_last_frame()) l.set_frame(10);
        else                   l.next_frame();
    }
}



// Diese Funktion ist in builder.cpp definiert
void update_shrugger (Lixxie& l, const UpdateArgs& ua);

void update_shrugger2(Lixxie& l, const UpdateArgs& ua)
{
    update_shrugger(l, ua);
}
