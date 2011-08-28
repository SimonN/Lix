/*
 * lix/exploder.cpp
 *
 */

#include "ac.h"

void assclk_exploder2(Lixxie& l)
{
    l.set_exploder_knockback();
    l.inc_updates_since_bomb();
}



void assclk_exploder(Lixxie& l)
{
    l.inc_updates_since_bomb();
}



void update_exploder(Lixxie& l, const UpdateArgs& ua)
{
    // Bis zu sechs Pixel fallen
    for (int i = 0; i < 6; ++i) if (!l.is_solid()) l.move_down(1);

    // Nach Ende der Animation den Krater erstellen
    // und die Lix entfernen.
    if (l.is_last_frame()) {
        l.remove_rectangle( -4, -26,   3, -26);
        l.remove_rectangle( -6, -25,   5, -25);
        l.remove_rectangle( -8, -24,   7, -24);
        l.remove_rectangle( -9, -23,   8, -23);

        l.remove_rectangle(-10, -22,   9, -21);
        l.remove_rectangle(-11, -20,  10, -19);
        l.remove_rectangle(-12, -18,  11, -17);
        l.remove_rectangle(-13, -16,  12, -15);

        l.remove_rectangle(-14, -14,  13, -12);
        l.remove_rectangle(-15, -11,  14,  -7);

        l.remove_rectangle(-16,  -6,  15,   5);

        l.remove_rectangle(-15,   6,  14,   8);

        l.remove_rectangle(-14,   9,  13,  10);
        l.remove_rectangle(-13,  11,  12,  12);

        l.remove_rectangle(-12,  13,  11,  13);
        l.remove_rectangle(-11,  14,  10,  14);

        l.remove_rectangle( -9,  15,   8,  15);
        l.remove_rectangle( -7,  16,   6,  16);
        l.remove_rectangle( -5,  17,   4,  17);

        // Explosionsanimation starten
        l.play_sound(ua, Sound::POP);
        l.get_ef()->add_explosion(
         ua.st.update, l.get_tribe(), ua.id, l.get_ex(), l.get_ey());

        l.become(LixEn::NOTHING); // kill it
    }

    else l.next_frame();
}
