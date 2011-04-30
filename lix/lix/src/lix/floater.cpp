/*
 * lemming/floater.cpp
 *
 */

#include "ac.h"

void update_floater(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // special_y und special_x steuern, wie viel Pixel der Lemming jeweils
    // faellt. Die Vorgaben sind 0|0, sie sollten nach assign vom Faller
    // und vom Jumper geaendert werden.
    switch (l.get_frame()) {
        case 0: l.set_special_y(l.get_special_y() > 10 ? 10 : 6); break;
        case 1: l.set_special_y(6); break;
        case 2: l.set_special_y(2); break;
        case 3:
        case 4: l.set_special_y(0); break;
        case 5: l.set_special_y(2); break;
        case 6: l.set_special_y(4); break; // Das ist das normale Fallen.
        default:                    break;
    }

    // Horizontale Bewegung auf Dauer zum Stillstand kommen lassen,
    // die for-Schleife hier kontrolliert auf sehr schnellen Stillstand
    for (int i = 2; i < l.get_special_x(); ++i)
     if (l.is_solid(i, 0)) {
        l.set_special_x(i-2);
        break;
    }
    if (l.get_special_x() > 0 && l.get_frame() > 0)
     l.set_special_x(l.get_special_x() - 2);

    // Auf Boden prüfen,
    // dabei wird diagonal nach unten geschritten und davon ausgegangen,
    // dass die Geschwindigkeit in y-Richtung > Geschw. in x-Richtung ist
    for (int i = 0; i < l.get_special_y(); ++i) {
        int x = (l.get_special_x() * i) / l.get_special_y();
        if (l.is_solid(x, i+2)) {
            l.move_ahead(x);
            l.move_down(i);
            l.assign(LixEn::LANDER);
            break;
        }
    }

    if (l.get_ac() == LixEn::FLOATER) {
        l.move_ahead(l.get_special_x());
        l.move_down(l.get_special_y());
        if (l.is_last_frame()) l.set_frame(6);
        else l.next_frame();
    }
}
