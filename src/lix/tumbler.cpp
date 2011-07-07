/*
 * lemming/tumbler.cpp
 *
 */

#include "ac.h"

void update_jumper (Lixxie& l, const UpdateArgs& ua);
void update_tumbler(Lixxie& l, const UpdateArgs& ua)
{
    update_jumper(l, ua);
}

// Prueft Boden, Decken und Waende nach jedem einzelnen vorgerueckten Pixel.
// Liefert genau dann wahr, wenn es eine Kollision gab, damit das Vorruecken
// update_jumper/update_tumbler abgebrochen werden kann.
bool jumper_and_tumbler_collision(Lixxie& l)
{
    const int max_special_y = 18; // Ab hier wird er zum Splatter
    const int swh           = l.solid_wall_height();

    // Das Vorruecken zum jeweiligen Pixel ist bereits gemacht.

    // Kopfhoehe wird als etwas niedriger angesehen als beim Builder
    if (l.get_ac() == LixEn::TUMBLER
     && l.is_solid(-2, -12) && !l.is_solid(-2, -6)
     && (l.get_special_x() > 0 || l.get_special_y() < 0)
         // die vorige Zeile behebt das Langsamwerden beim oo-Fall

     || l.get_ac() == LixEn::JUMPER
     && l.is_solid( 0, -14) && !l.is_solid( 0, -8)) {
        if (l.get_ac() != LixEn::TUMBLER) l.assign(LixEn::TUMBLER);
        // Halb so schnell in x-Richtung wie bisher, aber
        // gerade Geschwindigkeitszahl beibehalten
        if (l.get_special_y() < 0) l.set_special_y(0);
        l.set_special_x(l.get_special_x() / 4);
        l.set_special_x(l.get_special_x() * 2);
        return true;
        // Es kann sein, dass sich der Lemming in eine Wand hineinbewegt, wenn
        // nur oder obere Teil vom Lemming vor der Wand ist, nicht der untere.
        // Das macht hoffentlich nichts, denn einzig der Lauf-Pixel soll
        // wandfrei sein, und sowas wird kontrolliert bei "if (swh > 6 ..."
    }
    // Vor eine Wand springen: Muss vor die Bodenkollision, damit wir somit
    // den Hochteleportier-Bug bekaempfen
    else if (swh > 9  && l.get_ac() == LixEn::JUMPER
     ||      swh > 0  && l.get_ac() == LixEn::TUMBLER) {
        // Suche in horizontaler Richtung nach dem ersten freien Pixel,
        // bewege dorthin, drehe Lemming in die Richtung dieser Bewegung.
        l.turn();
        l.move_ahead();
        // Dies behebt das Zuckeln in der Wand, was auftrat, als wir den
        // Hochteleportations-Bug entfernten: Hochbewegen oder hinunterbewegen
        // nach dem Umdrehen, je nach Y-Geschwindigkeit.
        // Dabei wird untersucht, wo der naechste freie Raum ist, und entweder
        // vor, zurueck oder entgegen der Y-Richtung vertikal bewegt.
        if (l.is_solid(0, 0)) {
            l.assign(LixEn::STUNNER);
            /* DEBUGGING, testing with geoo
            for (int dist = 1; ; ++dist) {
                if (!l.is_solid(dist/2*2, 0)) {
                    l.move_ahead(dist/2*2);
                    break;
                }
                else if (!l.is_solid(-(dist/2*2))) {
                    l.turn();
                    l.move_ahead(dist/2*2);
                    break;
                }
                else if (l.get_special_y() <  0 && !l.is_solid(0,  dist)) {
                    l.move_down(dist);
                    l.set_special_y(0);
                    break;
                }
                else if (l.get_special_y() >= 0 && !l.is_solid(0, -dist + 1)) {
                    // Outdated comm. since discusison with geoo -> stun immed.
                    // // Die + 1 in der Bedingung stehen dort, weil Fuss des
                    // // Lemmings immer noch einen freien Pixel drunter hat.
                    // l.move_up(dist);
                    l.assign(LixEn::STUNNER);
                    break;
                }
            }
            */
        }
        return true;
    }
    // Boden-Kollision
    // Damit sich die Lemminge auch mal mehr festhalten an den Klippen,
    // pruefen wir auf swh <= 2. Hoffentlich gibt das keine Bugs.
    else if (l.get_special_y() >= 0 && swh <= 2
     && (l.is_solid(0, 1) || l.is_solid(0, 2)))
    {
        while (l.is_solid(0, 1)) l.move_up(1);
        if (l.get_special_y() > max_special_y) l.assign(LixEn::SPLATTER);
        else {
            bool short_anim = (l.get_special_y() < 12);
            if (l.get_ac() == LixEn::JUMPER) {
                l.assign(LixEn::LANDER);
                if (short_anim) l.next_frame();
            }
            else l.assign(LixEn::STUNNER);
        }
        return true;
    }

    // Wenn Jumper, nicht Tumbler:
    // Wand nicht ganz so hoch, man kann sich festhalten oben dran.
    else if (swh > 0) {
        if (l.get_special_y() > max_special_y) {
            l.move_up(swh);
            l.assign(LixEn::SPLATTER);
        }
        else {
            l.assign(LixEn::ASCENDER);
            l.set_frame(5-swh/2 < 0 ? 0 : 5-swh/2);
            l.move_down(1 + !(swh%2));
            // Zeile aus lemupd.cpp kopiert, vom Climber:
            // Ist's eine schroffe Klippe? Sonst noch einen nach vorne.
            if (!l.is_solid(2, -18)) l.move_ahead();
        }
        return true;
    }
    // Keine Kollision an diesem Pixel
    return false;
}



// ############################################################################
// ############################################################################
// ############################################################################



void tumbler_frame_selection(Lixxie& l)
{
    // Last 3 args are the same as in update_jumper().
    int y = l.get_special_y();
    int x = l.get_special_x();
    if (x < 0) x *= -1;
    if (x < 2) x =   2;

    int  tan = y * 12 / x;
    int  tf  = 0;          // tf == target frame
    bool anim = false;     // some poses consist of 2 alternating frames

    if      (tan >  18) tf = 13, anim = true;
    else if (tan >   9) tf = 11, anim = true;
    else if (tan >   3) tf =  9, anim = true;
    else if (tan >   1) tf =  8;
    else if (tan >  -1) tf =  7;
    else if (tan >  -4) tf =  6;
    else if (tan > -10) tf =  5;
    else if (tan > -15) tf =  4;
    else if (tan > -30) tf =  3;
    else if (tan > -42) tf =  2;
    else                tf =  0, anim = true;

    if (l.get_frame() > 0
     && tf > l.get_frame() + (anim ? 2 : 1)) {
        tf = l.get_frame() + (anim ? 2 : 1);
    }

    if (tf == l.get_frame() && anim) ++tf;

    l.set_frame(tf);
}
