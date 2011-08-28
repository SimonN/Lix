/*
 * lix/ac.h
 *
 * Diese Datei bindet die kompletten Klassen ein, die von den einzelnen
 * Faehigkeitsdateien gebraucht werden. Wir tun das nicht in lix.h und
 * auch nicht in ac_func.h, um haeufiges Neukompilieren zu vermeiden.
 *
 */

#pragma once

#include "../gameplay/effect.h"
#include "../gameplay/state.h"

struct UpdateArgs {
    GameState& st;
    unsigned   id;  // the lix's id, to pass to the effect manager

    // Zielen
    int  aim_x; // Wo ist die Maus auf dem Land?
    int  aim_y;
    bool aim_c; // Geklickt?

    inline UpdateArgs(GameState& s) : st(s) { }

    // Gegen Kompiliererwarnungen in einigen Funktionen, die ueberhaupt nix
    // brauchen hieraus, sondern nur die uebergebene Lix
    inline void suppress_unused_variable_warning() const {}
};
