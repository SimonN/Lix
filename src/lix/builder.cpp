/*
 * lix/builder.cpp
 *
 * special_x
 *
 *   Amount of bricks left.
 *
 * special_y
 *
 *   Normally 0. It's 1 when the lix is fully inside terrain, it should
 *   not move up then. Because the sprite assumes that it does move up,
 *   it's moved down again after stopping due to hitting terrain.
 *
 */

#include "ac.h"

void assclk_builder(Lixxie& l)
{
    if (l.get_ac() == LixEn::BUILDER) {
        l.set_special_x(l.get_special_x() + 12);
    }
    else l.become(LixEn::BUILDER);
}



void become_builder(Lixxie& l)
{
    l.become_default(LixEn::BUILDER);
    l.set_special_x(12);
    l.set_frame(6);
}



void update_builder(Lixxie& l, const UpdateArgs& ua)
{
    switch (l.get_frame()) {

    // Bau-Frame
    case 7:
        // don't glitch up through steel, but still get killed by top of
        // screen: first see whether trapped, then make brick. For the magic
        // number 13, see walker.cpp, it's because they can ascend 12 pixels.
        l.set_special_y(l.solid_wall_height(2) >= 13);

        l.set_special_x(l.get_special_x() - 1);
        l.draw_brick   (-2, 0, 9, 1);
        if (l.get_special_x() < 3) l.play_sound_if_trlo(ua, Sound::BRICK);

        l.move_up();
        break;

    // Zwei Fortbewegungs-Frames
    case 11:
        // Kollision mit Landschaft? Dann umdrehen!
        // Kopf und Fuß werden geprüft hierfür.
        // +6|-18 statt +2|-16: Die Lix soll wissen, wo's hin geht.
        // +6|-1 ist die Koordinate über dem Stein, nicht -2, da sonst
        // in seltenen Fällen (Lix in dünner Horizontalen) nicht
        // bis zu dieser Ebene hochgebaut wird.

        // Die zweite Abfrage in der Und-Klammer dient dazu, Treppen
        // auch auf anderen Treppen bauen zu können.
        // Last line is Horus bug prevention.

        // Note that the lix has already moved up and the image has its
        // feet below the regular position, inside the newly placed brick.
        if (((l.is_solid(6, 0) && l.is_solid(6, -2)) || l.is_solid(6, -16))
         || ((l.is_solid(4, 0) && l.is_solid(4, -2)) || l.is_solid(4, -16))
         ||  (l.is_solid(2, 0) && l.is_solid(2, -2))) {
            // Ueberfluessige Faehigkeitsbenutzngen an den Spieler
            // zurueckgeben, passiert nur bei Option "multiple Builders".
            // Wird von l.assign() erledigt.
            // See top comment for the check of special_y.
            l.turn();
            if (l.get_special_y() == 1) l.move_down();
            l.become(LixEn::WALKER);
        }
        break;

    case 12:
    case 13:
        l.move_ahead();
        break;

    // Klötze zählen und ggf. zum Shrugger werden.
    case 15:
        if (l.get_special_x() <= 0) l.become(LixEn::SHRUGGER);
        break;
    }

    if (l.get_ac() == LixEn::BUILDER) l.next_frame();
}



void update_shrugger(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) l.become(LixEn::WALKER);
    else l.next_frame();
}
