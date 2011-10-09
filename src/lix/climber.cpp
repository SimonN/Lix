/*
 * lix/climber.cpp
 *
 * Enthaelt alles zum Climber und das Wenige zum Ascender, auch wenn der
 * Ascender oft vom Walker mitbenutzt wird.
 *
 * special_x
 *
 *   Ist 1, wenn die Lix um 1 in X-Richtung versetzt gezeichnet werden
 *   muss. Hat Einfluss auf die Kontrolle der X-Koordinaten. Damit ist der
 *   Climber die einzige Faehigkeit, die sich an horizontalen Pixeln stoert.
 *
 */

#include "ac.h"

void assclk_climber(Lixxie& l)
{
    l.set_climber();
}



void become_climber(Lixxie& l)
{
    // this is a backup for not becoming a climber after all
    int walker_frame_backup = -999;
    if (l.get_ac() == LixEn::WALKER
     || l.get_ac() == LixEn::RUNNER) walker_frame_backup = l.get_frame();

    l.become_default(LixEn::CLIMBER);
    l.set_frame(3); // so this is the default for walkers -> climbers.

    // Korrektur der Anzeige, damit der Climber nicht mit einer Pixelreihe im
    // Felsen sitzt: In diesem Fall wird special_x = 1 gesetzt. Wenn diese
    // Korrektur beim Climber-Update stattfinden muss, wird man Faller!
    l.set_ex(l.get_ex());
    if ((l.get_dir() > 0 &&  l.is_solid_single(1, -6))
     || (l.get_dir() < 0 && !l.is_solid_single(1, -6)) ) {
        l.set_x(l.get_x() - 1);
        l.set_special_x(1);
    }

    // Und prüfen, ob sie eventuell sofort Ascender werden muss!
    for (int i = 8; i < 18; ++i) {
        // copied this also from update_climber() to remedy climbers
        // passing through platforms on cubes due to immediately ascending
        const bool diff       = l.get_special_x();
        const bool solid_here = l.is_solid_single(0, -i);
        const bool solid_diff = l.is_solid_single(1, -i);
        if ((l.get_dir() > 0 && (solid_here || (solid_diff && !diff)) )
         || (l.get_dir() < 0 && (solid_here || (solid_diff &&  diff)) ) ) {
            l.turn();
            if (l.is_solid(0, 2)) {
                l.become(LixEn::WALKER);
                if (walker_frame_backup >= 0) l.set_frame(walker_frame_backup);
            }
            else l.become(LixEn::FALLER);
            break;
        }
        // end of "copied this also from update_climber()
        else if (!l.is_solid(2, -i)) {
            l.move_ahead();
            l.become(LixEn::ASCENDER);
            break;
        }
    }
    // Ende von For
}



void update_climber(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    int up_by = 0;
    switch ((l.get_frame() - 4) % 8) {
        case 4: up_by = 2; break;
        case 5: up_by = 4; break;
        case 6: up_by = 2; break;
        default:           break;
    }

    if (l.get_frame() >= 4 && (l.get_frame() - 4) % 8 > 3)
     for (int i = 0; i < up_by; ++i) {
        l.move_up(1);
        // Fallen? Dann sofort aus der Schleife ausbrechen!
        // Object ist 1, wenn die Lix einen Pixel zurück liegt
        // solid_diff wird gebraucht, wenn das Lixbild um einen Pixel nach
        // rechts verschoben worden ist. Das muss jeweils unterschiedlich
        // gehandhabt werden, je nach Richtung.
        const bool diff       = l.get_special_x();
        const bool solid_here = l.is_solid_single(0, -16);
        const bool solid_diff = l.is_solid_single(1, -16);
        if ((l.get_dir() > 0 && (solid_here || (solid_diff && !diff)) )
         || (l.get_dir() < 0 && (solid_here || (solid_diff &&  diff)) ) ) {
            l.move_down(1);
            l.turn();
            l.become(LixEn::FALLER);
            break;
        }
        // Ist der obere Rand erreicht zum Ascender-Werden?
        else if (!l.is_solid(2, -16)) {
            l.move_ahead();
            l.become(LixEn::ASCENDER);
            break;
        }
    }

    // Wenn noch Climber...
    if (l.get_ac() == LixEn::CLIMBER) {
        if (l.is_last_frame()) l.set_frame(4);
        else                   l.next_frame();

        // Korrektur der Anzeige, damit der Climber nicht mit einer Pixel-
        // reihe im Felsen sitzt
        l.set_ex(l.get_ex());
        if ((l.get_dir() > 0 &&  l.is_solid_single(1, -6))
         || (l.get_dir() < 0 && !l.is_solid_single(1, -6)) )
            l.set_x(l.get_x() - 1);
    }
}
