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

#include "../editor/graph_ed.h"
#include "../other/types.h"

struct GameState {

private:

    typedef std::vector <Tribe>     TribeVec;
    typedef std::vector <EdGraphic> EdGrVec;

public:

    Ulng     update;
    Ulng     clock;
    bool     clock_running;

    TribeVec tribes;
    EdGrVec  trap;
    EdGrVec  fling;

    Torbit   land;
    Lookup   lookup;

    GameState();

    inline operator bool () const { return land.get_xl() > 1; }

};
