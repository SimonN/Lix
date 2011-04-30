/*
 * lemming/climber.cpp
 *
 * Enthaelt alles zum Climber und das Wenige zum Ascender, auch wenn der
 * Ascender oft vom Walker mitbenutzt wird.
 *
 * special_x
 *
 *   Ist 1, wenn der Lemming um 1 in X-Richtung versetzt gezeichnet werden
 *   muss. Hat Einfluss auf die Kontrolle der X-Koordinaten. Damit ist der
 *   Climber die einzige Faehigkeit, die sich an horizontalen Pixeln stoert.
 *
 */

#include "ac.h"

void assign_climber(Lixxie& l)
{
    l.set_ac(LixEn::CLIMBER);
    l.set_frame    (0);
    l.set_special_x(0);
    l.set_special_y(0);

    // Verhindern, dass er in der Wand zu klettern beginnt
    // Y = -10 [Edit im August 09: -12],
    // denn sonst überklettert er einen sehr niedrigen
    // Rand und setzt von Neuem zum Klettern an
    // before 2010-03-10 this had "&& l.is_solid(1, -12)" in the while
    // condition, this lead to a bug visible in Rubix' video of
    // "Still everything to play for" (L1 2-player level number 2)
    while (l.is_solid(0, -12)) l.move_ahead(-2);

    // Korrektur der Anzeige, damit der Climber nicht mit einer Pixelreihe im
    // Felsen sitzt: In diesem Fall wird special_x = 1 gesetzt. Wenn diese
    // Korrektur beim Climber-Update stattfinden muss, wird man Faller!
    l.set_ex(l.get_ex());
    if (l.get_dir() > 0 &&  l.is_solid(1, -6)
     || l.get_dir() < 0 && !l.is_solid(1, -6)) {
        l.set_x(l.get_x() - 1);
        l.set_special_x(1);
    }

    // Und prüfen, ob er eventuell nicht sofort Ascender werden muss!
    for (int i = 8; i < 18; ++i) {
        if (!l.is_solid(2, -i)) {
            l.move_ahead();
            // Nochmal vorwärts, wenn es keine schroffe Klippe ist
            if (!l.is_solid(2, -i-2)) l.move_ahead();
            l.move_down(11-i);
            l.assign(LixEn::ASCENDER);
            break;
        }
    }
    // Ende von For
}



void update_climber(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    int up_by = 0;
    switch (l.get_frame() % 8) {
        case 4: up_by = 2; break;
        case 5: up_by = 4; break;
        case 6: up_by = 2; break;
        default:           break;
    }

    if (l.get_frame()%8 > 3)
     for (int i = 0; i < up_by; ++i) {
        l.move_up(1);
        // Fallen? Dann sofort aus der Schleife ausbrechen!
        // Object ist 1, wenn der Lemming einen Pixel zurück liegt
        // solid_diff wird gebraucht, wenn das Lemmingbild um einen Pixel nach
        // rechts verschoben worden ist. Das muss jeweils unterschiedlich
        // gehandhabt werden, je nach Richtung.
        const bool diff       = l.get_special_x();
        const bool solid_here = l.is_solid(0, -16);
        const bool solid_diff = l.is_solid(1, -16);
        if (l.get_dir() > 0 && (solid_here || solid_diff && !diff)
         || l.get_dir() < 0 && (solid_here || solid_diff &&  diff)) {
            l.move_down(1);
            l.turn();
            l.assign(LixEn::FALLER);
            break;
        }
        // Ist der obere Rand erreicht zum Ascender-Werden?
        else if (!l.is_solid(2, -16)) {
            l.assign(LixEn::ASCENDER);
            l.move_ahead();
            l.move_up(5);
            // Ist's eine schroffe Klippe? Sonst noch einen nach vorne.
            if (!l.is_solid(2, -18)) l.move_ahead();
            break;
        }
    }

    // Wenn noch Climber...
    if (l.get_ac() == LixEn::CLIMBER) {
        l.next_frame();

        // Korrektur der Anzeige, damit der Climber nicht mit einer Pixel-
        // reihe im Felsen sitzt
        l.set_ex(l.get_ex());
        if (l.get_dir() > 0 &&  l.is_solid(1, -6)
         || l.get_dir() < 0 && !l.is_solid(1, -6))
            l.set_x(l.get_x() - 1);
    }
}



void update_ascender(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    l.move_up();

    if (l.is_last_frame()) {
        l.assign(LixEn::WALKER);
        // Das letzte Walker-Frame passt am besten
        l.set_frame(8);
    }
    else l.next_frame();
}
