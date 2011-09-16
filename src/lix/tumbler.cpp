/*
 * lix/tumbler.cpp
 *
 */

#include "ac.h"

static const int tumbler_max_special_y = 18; // Ab hier wird er zum Splatter



void update_jumper (Lixxie& l, const UpdateArgs& ua);
void update_tumbler(Lixxie& l, const UpdateArgs& ua)
{
    update_jumper(l, ua);
}



void tumbler_unglitch_out_of_wall(Lixxie& l)
{
    // We can be either a stunner or still a jumper/tumbler here.
    // In the current implementation, we're always a stunner.
    if (! l.is_solid(0, 1)) return;

    // If we are in some stupid wall, do this loop.
    for (int dist = 1; ; ++dist) {
        if (!l.is_solid(dist/2*2, 1)) {
            l.move_ahead(dist/2*2);
            break;
        }
        else if (!l.is_solid(-(dist/2*2), 1)) {
            l.turn();
            l.move_ahead(dist/2*2);
            break;
        }
        else if (l.get_special_y() <= 0 && !l.is_solid(0,  dist + 1)) {
            l.move_down(dist);
            l.set_special_y(-l.get_special_y());
            break;
        }
        else if (l.get_special_y() >= 0 && !l.is_solid(0, -dist)) {
            // Die + 1 in der Bedingung stehen dort, weil Fuss der
            // Lix immer noch einen freien Pixel drunter hat.
            l.move_up(dist);
            l.become(LixEn::STUNNER);
            break;
        }
    }
}



static void tumbler_land(Lixxie& l)
{
    if (l.get_special_y() > tumbler_max_special_y) {
        const int frame_to_get_second_splatter = 9;
        const int second_splatter_frame = 10;

        bool second_splatter = (l.get_ac() == LixEn::TUMBLER
                             && l.get_frame() >= frame_to_get_second_splatter);
        l.become(LixEn::SPLATTER);
        if (second_splatter) l.set_frame(second_splatter_frame);
    }
    // become a lander or stunner
    else {
        bool short_anim = (l.get_special_y() < 12);
        if (l.get_ac() == LixEn::JUMPER) {
            l.become(LixEn::LANDER);
            if (short_anim) l.next_frame();
        }
        else {
            l.become(LixEn::STUNNER);
            tumbler_unglitch_out_of_wall(l);
        }
    }
}



// Prueft Boden, Decken und Waende nach jedem einzelnen vorgerueckten Pixel.
// Liefert genau dann wahr, wenn es eine Kollision gab, damit das Vorruecken
// update_jumper/update_tumbler abgebrochen werden kann.
bool jumper_and_tumbler_collision(Lixxie& l)
{
    const int swh           = l.solid_wall_height();

    // Das Vorruecken zum jeweiligen Pixel ist bereits gemacht.

    // Kopfhoehe wird als etwas niedriger angesehen als beim Builder
    if ((l.get_ac() == LixEn::TUMBLER
        && l.is_solid(-2, -12) && !l.is_solid(-2, -6)
        && l.get_special_y() < 4)
         // die vorige Zeile behebt das Langsamwerden beim oo-Fall
     || (l.get_ac() == LixEn::JUMPER
        && l.is_solid( 0, -14) && !l.is_solid( 0, -8))
    ) {
        if (l.get_ac() != LixEn::TUMBLER) l.become(LixEn::TUMBLER);
        // Halb so schnell in x-Richtung wie bisher, aber
        // gerade Geschwindigkeitszahl beibehalten
        if (l.get_special_y() < 4) l.set_special_y(4);
        l.set_special_x(l.get_special_x() / 4);
        l.set_special_x(l.get_special_x() * 2);

        return true;
        // Es kann sein, dass sich die Lix in eine Wand hineinbewegt, wenn
        // nur oder obere Teil von der Lix vor der Wand ist, nicht der untere.
        // Das macht hoffentlich nichts, denn einzig der Lauf-Pixel soll
        // wandfrei sein, und sowas wird kontrolliert bei "if (swh > 6 ..."
    }
    // Vor eine Wand springen: Muss vor die Bodenkollision, damit wir somit
    // den Hochteleportier-Bug bekaempfen
    else if ((swh > 9  && l.get_ac() == LixEn::JUMPER)
     ||      (swh > 0  && l.get_ac() == LixEn::TUMBLER)) {
        // Stick to the surface of the wall which we're inside right now
        if (l.get_ac() == LixEn::JUMPER && l.get_climber()) {
            l.move_ahead(-2);
            l.become(LixEn::CLIMBER);
            l.set_frame(0);

            return true; // Do not tumbler_unglitch_out_of_wall near "Zuckeln"
        }
        else if (l.is_solid(0, 2) && ! l.is_solid(0, 0)) {
            // Don't turn if batted onto a 45 degree slope.
            while (l.is_solid(0, 1)) l.move_up(1);
            tumbler_land(l);
        }
        else if (l.is_solid(-2, 2) && ! l.is_solid(-2, 0)) {
            // Don't turn if batted onto a 45 degree slope, part 2
            l.move_ahead(-2);
            while (l.is_solid(0, 1)) l.move_up(1);
            tumbler_land(l);
        }
        else {
            // Move out of wall we seem to be in.
            l.turn();
            l.move_ahead();
        }
        // Dies behebt das Zuckeln in der Wand, was auftrat, als wir den
        // Hochteleportations-Bug entfernten: Hochbewegen oder hinunterbewegen
        // nach dem Umdrehen, je nach Y-Geschwindigkeit.
        // Dabei wird untersucht, wo der naechste freie Raum ist, und entweder
        // vor, zurueck oder entgegen der Y-Richtung vertikal bewegt.
        if (l.is_solid(0, 1)) {
            tumbler_unglitch_out_of_wall(l);
        }
        return true;
    }

    // Boden-Kollision
    // Damit sich die Lixen auch mal mehr festhalten an den Klippen,
    // pruefen wir auf swh <= 2. Hoffentlich gibt das keine Bugs.
    else if (l.get_special_y() >= 0 && swh <= 2
     && (l.is_solid(0, 1) || l.is_solid(0, 2)))
    {
        while (l.is_solid(0, 1)) l.move_up(1);
        tumbler_land(l);
        return true;
    }

    // Wenn Jumper, nicht Tumbler:
    // Wand nicht ganz so hoch, man kann sich festhalten oben dran.
    else if (swh > 0) {
        if (l.get_special_y() > tumbler_max_special_y) {
            // this shouldn't ever get used, a jumper shouldn't fall with
            // deadly speed, she should always turn into a tumbler
            l.move_up(swh);
            tumbler_land(l); // this will always kill her since spec_y is high
        }
        else {
            l.become(LixEn::ASCENDER);
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
