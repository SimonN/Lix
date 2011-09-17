/*
 * gameplay/state.cpp
 *
 */

#include "state_ma.h"

const unsigned StateManager::updates_sml( 10);
const unsigned StateManager::updates_med( 50);
const unsigned StateManager::updates_big(200);

const GameState& StateManager::get_auto(Ulng u)
{
    GameState*                          s = &zero;
    if      (sml_1 && sml_1.update < u) s = &sml_1;
    else if (sml_2 && sml_2.update < u) s = &sml_2;
    else if (med_1 && med_1.update < u) s = &med_1;
    else if (med_2 && med_2.update < u) s = &med_2;
    else if (big_1 && big_1.update < u) s = &big_1;
    else if (big_2 && big_2.update < u) s = &big_2;

    if (sml_1 && sml_1.update > s->update) sml_1 = GameState();
    if (sml_2 && sml_2.update > s->update) sml_2 = GameState();
    if (med_1 && med_1.update > s->update) med_1 = GameState();
    if (med_2 && med_2.update > s->update) med_2 = GameState();
    if (big_1 && big_1.update > s->update) big_1 = GameState();
    if (big_2 && big_2.update > s->update) big_2 = GameState();

    return *s;
}



void StateManager::calc_save_auto(const GameState& s)
{
    const bool save_sml  =   ( s.update % updates_sml == 0);
    const bool which_sml = ! ((s.update / updates_sml) % 2);
    const bool save_med  =   ( s.update % updates_med == 0);
    const bool which_med = ! ((s.update / updates_med) % 2);
    const bool save_big  =   ( s.update % updates_big == 0);
    const bool which_big = ! ((s.update / updates_big) % 2);

    if (save_big) {
        if (which_big) big_1 = (med_1 ? med_1 : big_1);
        else           big_2 = (med_1 ? med_1 : big_2);
    }
    if (save_med) {
        if (which_med) med_1 = (sml_1 ? sml_1 : med_1);
        else           med_2 = (sml_1 ? sml_1 : med_2);
    }
    if (save_sml) {
        if (which_sml) sml_1 = s;
        else           sml_2 = s;
    }
}
