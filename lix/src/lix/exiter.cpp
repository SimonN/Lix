/*
 * lemming/exiter.cpp
 *
 * special_x
 *
 *   Wird auf 1 gesetzt, wenn die Animation vollstaendig durchlaufen wurde.
 *   Dann kann die Gameplay-Funktion diesen Lemming entfernen und dem ent-
 *   sprechenden Spieler einen Punkt geben - hier kennen wir ja keine Ziele.
 *
 * special_y
 *
 *   Nimmt die Nummer des Ziels im Ziel-Vektor auf. Allein die Gameplay-Klasse
 *   kann mit dieser Info etwas anfangen. Die in dieser Datei stehenden Funk-
 *   tionen scheren sich nicht um special_x.
 *
 */

#include "ac.h"

void assign_exiter(Lixxie& l)
{
    l.set_ac(LixEn::EXITER);
    l.set_frame(0);
    l.set_special_x(0);
    l.set_special_y(0);

    // Nicht explodieren lassen
    l.set_updates_since_bomb(0);
}



void update_exiter(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (!l.is_last_frame()) l.next_frame();
    else l.set_special_x(1);
}
