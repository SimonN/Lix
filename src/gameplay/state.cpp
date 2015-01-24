/*
 * gameplay/state.cpp
 *
 */

#include "state.h"

GameState::GameState()
:
    update(0),
    goals_locked(false),
    land(1, 1),
    lookup(1, 1)
{
}
