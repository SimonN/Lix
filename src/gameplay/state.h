/*
 * gameplay/state.h
 *
 * Ein Spielstand, der geladen, gespeichert usw. werden kann, und von dem
 * die Gameplayklasse genau einen besitzt, der immer aktuell ist.
 *
 */

#pragma once

#include "lookup.h"
#include "tribe.h"
#include "trigger.h"

#include "../other/types.h"

struct GameState {

private:

    typedef std::vector <Tribe>       TribeVec;
    typedef std::vector <EdGraphic>   EdGrVec;
    typedef std::vector <Triggerable> TrigVec;

public:

    Ulng     update;
    Ulng     clock;
    bool     clock_running;

    TribeVec tribes;
    TrigVec  trap;
    TrigVec  fling;
    TrigVec  trampoline;

    Torbit   land;
    Lookup   lookup;

    GameState();

    inline operator bool () const { return land.get_xl() > 1; }

};
