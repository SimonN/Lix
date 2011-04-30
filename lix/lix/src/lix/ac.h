/*
 * lemming/ac.h
 *
 * Diese Datei bindet die kompletten Klassen ein, die von den einzelnen
 * Faehigkeitsdateien gebraucht werden. Wir tun das nicht in lix.h und
 * auch nicht in ac_func.h, um haeufiges Neukompilieren zu vermeiden.
 *
 */

#pragma once

#include "../gameplay/effect.h"

struct UpdateArgs {
    unsigned long upd; // the update
    unsigned      id;  // the lemming's id, to pass to the effect manager

    // Zielen
    int  aim_x; // Wo ist die Maus auf dem Land?
    int  aim_y;
    bool aim_c; // Geklickt?

    // Gegen Kompiliererwarnungen in einigen Funktionen, die ueberhaupt nix
    // brauchen hieraus, sondern nur den ebenfalls uebergebenen Lemming
    inline void suppress_unused_variable_warning() const {}

};
