/*
 * lix/faller.cpp
 *
 * special_x
 *
 *   Zaehlt, wie viele Pixel die Lix bereits nach unten gefallen ist.
 *   Wird eine Lix zum Faller, so sollten die anderen Faehigkeiten, wenn
 *   sie diese Aktivitaet zuweisen, schon einige Pixel vorgeben mit
 *   set_special_x(), falls sie die Lix bereits etwas bewegt haben.
 *
 *   Der Faller nutzt die Standardzuweisung assign_default(), damit beginnt
 *   er ansonsten bei null gefallenen Pixeln.
 *
 * special_y
 *
 *   Speed towards the bottom. Is 4 at the beginning, goes up by 1 after each
 *   update. This is not the same acceleration as in jumper.cpp.
 *
 */

#include "ac.h"

void become_faller(Lixxie& l)
{
    l.become_default(LixEn::FALLER);
    l.set_special_y(4);
}



void update_faller(Lixxie& l, const UpdateArgs& ua)
{
    for (int i = 0; i <= l.get_special_y() && l.get_ac() == LixEn::FALLER;++i){
        if (l.is_solid(0, i+2)) {
            l.move_down(i);
            l.set_special_x(l.get_special_x() + i);

            // Schirm in letzter Sekunde?
            if (l.get_special_x() > Lixxie::distance_safe_fall
             && !l.get_floater()) {
                l.become(LixEn::SPLATTER);
                l.play_sound(ua, Sound::SPLAT);
                // Nicht explodieren lassen, das täte er bei 76 :-)
                if (l.get_updates_since_bomb() == 75)
                 l.set_updates_since_bomb(0);
            }
            else if ((l.get_special_x() <= 9 && l.get_frame() < 3)
             ||       l.get_special_x() == 0) {
                l.become(LixEn::WALKER);
                if (l.get_runner()) l.set_frame(6);
                else                l.set_frame(8);
            }
            else if (l.get_frame() < 3) {
                l.become(LixEn::LANDER);
                l.set_frame(1);
            }
            else {
                l.become(LixEn::LANDER);
                // use the regular frame 0
            }
        }
    }

    if (l.get_ac() == LixEn::FALLER) {
        l.set_special_x(l.get_special_x() + l.get_special_y());
        l.move_down(l.get_special_y());

        if (l.get_special_y() < 8) l.set_special_y(l.get_special_y() + 1);

        // The last two frames alternate, the first frames are just the
        // initial frames of falling.
        if (l.is_last_frame()) l.set_frame(l.get_frame() - 1);
        else l.next_frame();

        if (l.get_floater()
         && l.get_special_x() >= Lixxie::distance_float) {
            const int sy = l.get_special_y();
            l.become(LixEn::FLOATER);
            l.set_special_y(sy);
        }
    }
}
