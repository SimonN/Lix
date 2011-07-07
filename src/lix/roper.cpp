/*
 * lemming/jumper.cpp
 *
 * special_x
 *
 *   0, falls der Haken noch nicht abgeschossen wurde.
 *   1, falls doch bereits.
 *
 *   Diese Konvention ist fuer alle zielenden Faehigkeiten aehnlich, siehe
 *   lix.h fuer Details.
 *
 * special_y
 *
 *   Hat zwei Bedeutungen, die jeweils in einem Absatz erklaert werden:
 *
 *   Wird bei der Zuweisung des Ropers von der Standardzuweisung auf 0 gesetzt.
 *   In jedem Update wird es erhoeht. Wenn es zu hoch ist und der Roper noch
 *   nicht den Haken abgeschossen hat, so wird er wieder zum Walker, aber nur,
 *   falls special_x noch nicht 1 ist.
 *
 *   Wird beim Abfeuern des Ropers auf 0 gesetzt. Der Lemming verharrt naemlich
 *   eine gewisse Zeit nach dem Abfeuern in seiner letzten Blickrichtung vor
 *   dem Abfeuern, bevor er zum Walker wird. Diese Zeit wird hiermit gemessen.
 *
 */

#include <cmath>

#include "ac.h"

const int frame_aiming_first = 8;

// Hilfsfunktion
void set_frame_after_aiming(Lixxie& l, const UpdateArgs& ua)
{
    const double pi = std::acos(-1.0);
    const int    x  = (ua.aim_x - l.get_ex()) * l.get_dir();
    const int    y  =  ua.aim_y - l.get_ey();
    // Fuer (0, 0) einfach nix machen, Lemming soll wie vorher schauen
    if (x != 0 || y != 0) {
        // Den Winkel so ausrechnen, als sei (0, 1) der Nullwinkel,
        // nicht (1, 0), wie es normal waere. Alles gedreht.
        // atan2 nimmt die Dinger in der Reihenfolge 1. y, 2. x.
        double angle = std::atan2(-x, -y) * 180 / pi;
        if      (angle >  99) l.set_frame(frame_aiming_first +  0);
        else if (angle >  80) l.set_frame(frame_aiming_first +  1);
        else if (angle >  60) l.set_frame(frame_aiming_first +  2);
        else if (angle >  30) l.set_frame(frame_aiming_first +  3);
        else if (angle >  10) l.set_frame(frame_aiming_first +  4);
        else if (angle > -10) l.set_frame(frame_aiming_first +  5);
        else if (angle > -30) l.set_frame(frame_aiming_first +  6);
        else if (angle > -60) l.set_frame(frame_aiming_first +  7);
        else if (angle > -80) l.set_frame(frame_aiming_first +  8);
        else if (angle > -99) l.set_frame(frame_aiming_first +  9);
        else                  l.set_frame(frame_aiming_first + 10);
    }
}



void update_roper(Lixxie& l, const UpdateArgs& ua)
{
    // Normales Update, einmal pro Tick
    if (!ua.aim_c) {
        if (l.get_frame() < frame_aiming_first) {
            l.next_frame();
        }
        // Nagel fertig in der Erde, aber vor Haken abschiessen
        else if (l.get_special_x() == 0) {
            set_frame_after_aiming(l, ua);

            // Wenn man zu lange wartet, weiterlaufen, einfach
            l.set_special_y(l.get_special_y() + 1);
            if (l.get_special_y() > Lixxie::updates_roper_before)
             l.assign(LixEn::WALKER);
        }
        // Haken wurde schon abgeschossen, warten, ehe man weiterlaeuft
        else {
            l.set_special_y(l.get_special_y() + 1);
            if (l.get_special_y() > Lixxie::updates_roper_after)
             l.assign(LixEn::WALKER);
        }
    }

    // Zusaetzliches Update pro Tick, wenn zum Zielen per Klick aufgefordert
    // wurde: Dies wird vor dem normalen Update ausgefuehrt.
    else {
        // Nagel in den Boden hauen: Zwar Maus in Beschlag nehmen durch
        // special_x == 0, aber nix machen. Weiter die Klicks aufsaugen.
        if (l.get_frame() < frame_aiming_first);

        // Zielen, aber noch nicht abgeschossen: Nun abschiessen
        else if (l.get_special_x() == 0) {
            l.set_special_x(1);
            l.set_special_y(0);
            set_frame_after_aiming(l, ua);
        }
    }
}
