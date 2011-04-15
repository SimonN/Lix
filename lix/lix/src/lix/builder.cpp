/*
 * lemming/builder.cpp
 *
 * special_x
 *
 *   Amount of bricks left.
 *
 */

#include "ac.h"

void assign_builder(Lixxie& l)
{
    l.set_special_y(0);
    if (l.get_ac() == LixEn::BUILDER) {
        l.set_special_x(l.get_special_x() + 12);
    }
    else {
        l.set_special_x(12);
        l.set_frame(6);
        l.set_ac(LixEn::BUILDER);
    }
}



void update_builder(Lixxie& l, const UpdateArgs& ua)
{
    switch (l.get_frame()) {

    // Bau-Frame
    case 7:
        l.set_special_x(l.get_special_x() - 1);
        l.draw_brick   (-2, 0, 9, 1);
        if (l.get_special_x() < 3) l.play_sound_if_trlo(ua, Sound::BRICK);
        break;

    // Zwei Fortbewegungs-Frames
    case 11:
        // Kollision mit Landschaft? Dann umdrehen!
        // Kopf und Fuß werden geprüft hierfür.
        // +6|-18 statt +2|-16: Der Lemming soll wissen, wo's hin geht.
        // +6|-1 ist die Koordinate über dem Stein, nicht -2, da sonst
        // in seltenen Fällen (Lemming in dünner Horizontalen) nicht
        // bis zu dieser Ebene hochgebaut wird.

        // Die zweite Abfrage in der Und-Klammer dient dazu, Treppen
        // auch auf anderen Treppen bauen zu können.
        // Last line is Horus bug prevention.
        if ((l.is_solid(6, -2) && l.is_solid(6, -4) || l.is_solid(6, -18))
         || (l.is_solid(4, -2) && l.is_solid(4, -4) || l.is_solid(4, -18))
         || (l.is_solid(2, -2) && l.is_solid(2, -4))) {
            // Ueberfluessige Faehigkeitsbenutzngen an den Spieler
            // zurueckgeben, passiert nur bei Option "multiple Builders".
            // Wird von l.assign() erledigt.
            l.turn();
            l.assign(LixEn::WALKER);
            l.set_frame(1); // sofort weiterlaufen
        }
        break;

    case 12:
        l.move_ahead();
        l.move_up();
        break;

    case 13:
        l.move_ahead();
        break;

    // Klötze zählen und ggf. zum Shrugger werden.
    case 15:
        if (l.get_special_x() <= 0) l.assign(LixEn::SHRUGGER);
        break;
    }

    // Wenn der Lemming noch ein Builder ist, nächstes Frame wählen!
    if (l.get_ac() == LixEn::BUILDER) l.next_frame();
}



void update_shrugger(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) {
        // bool was_builder = l.get_ac() == LixEn::SHRUGGER;
        l.assign(LixEn::WALKER);
        // Dieser Code wird auch von update_shrugger2() genutzt, daher ?:
        // l.set_frame(was_builder ? 0 : 8);
    }
    else l.next_frame();
}
