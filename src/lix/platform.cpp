/*
 * lix/platform.cpp
 *
 */

#include "ac.h"

void assclk_platformer(Lixxie& l)
{
    if (l.get_ac() == LixEn::PLATFORMER) {
        l.set_special_x(l.get_special_x() + 12);
    }
    else l.become(LixEn::PLATFORMER);
}



static const int platformer_standing_up_frame = 9;

void become_platformer(Lixxie& l)
{
    const bool continue_on_same_height
        = (l.get_ac() == LixEn::SHRUGGER2
        && l.get_frame() < platformer_standing_up_frame);
    l.become_default(LixEn::PLATFORMER);
    l.set_special_x(12);
    l.set_frame(continue_on_same_height ? 16 : 0);
}



static bool platformer_is_solid(Lixxie& l, int x, int y)
{
    // If the pixel is solid, return false nontheless if there is free air
    // over the pixel
    bool b = l.is_solid(x, y);
    if (b && l.is_solid(x + 2, y) && l.is_solid(x + 4, y)) return true;
    for (int i = -1; i > -4; --i)
        b = b && (l.is_solid(x+2, y-2)
               || l.is_solid(x, y-2)
               || l.is_solid(x-2, y-2));
    return b;
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
        // Kompletten naechsen Stein vorausplanen, bei Kollision NICHT drehen.
        if (platformer_is_solid(l, 6, -2)
         && platformer_is_solid(l, 8, -2) && platformer_is_solid(l, 10, -2)) {
            l.become(LixEn::WALKER);
        }
        break;

    case  6:
        if (! l.is_solid(0, -2)) l.move_up();
        else {
            l.become(LixEn::SHRUGGER2);
            l.set_frame(platformer_standing_up_frame);
        }
        // faellt durch

    case  7:
    case 21:
    case 22:
    case 23:
        // 2, 1 statt 2, 0, weil wir direkt ueber dem Boden pruefen wollen,
        // ob da ein Pixel ist. Sonst laufen die Walker durch.
        if (! platformer_is_solid(l, 2, 1)) l.move_ahead();
        else {
            l.become(LixEn::SHRUGGER2);
            l.set_frame(platformer_standing_up_frame);
        }
        break;

    case 25: // durchaus auch das letzte Frame
        // Kann noch ein Stein verlegt werden?
        if (l.get_special_x() == 0) {
            l.become(LixEn::SHRUGGER2);
            l.set_special_y(2); // Bei Klick 2 tiefer anfng. = weiterbauen
        }
        else if (platformer_is_solid(l, 2, 0)
         &&      platformer_is_solid(l, 4, 0)
         &&      platformer_is_solid(l, 6, 0)) {
            l.become(LixEn::SHRUGGER2);
            l.set_frame(platformer_standing_up_frame);
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
