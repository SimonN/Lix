/*
 * lemming/init.cpp
 *
 * Initialisierungs-Unterklasse
 *
 */

#include "lix.h"
#include "ac_func.h"

#include "../graphic/gra_lib.h"

std::vector <Lixxie::AcFunc>      Lixxie::ac_func(LixEn::AC_MAX);

void Lixxie::initialize_this_gets_called_from_glob_gfx_cpp()
{
    ac_func     [LixEn::FALLER]    .assign = assign_faller;
    ac_func     [LixEn::EXITER]    .assign = assign_exiter;
    ac_func     [LixEn::WALKER]    .assign = assign_walker;
    ac_func     [LixEn::CLIMBER]   .assign = assign_climber;
    ac_func     [LixEn::BUILDER]   .assign = assign_builder;
    ac_func     [LixEn::PLATFORMER].assign = assign_platformer;
    ac_func     [LixEn::MINER]     .assign = assign_miner;
    ac_func     [LixEn::DIGGER]    .assign = assign_digger;
    ac_func     [LixEn::JUMPER]    .assign = assign_jumper;

    ac_func     [LixEn::FALLER]    .update = update_faller;
    ac_func     [LixEn::TUMBLER]   .update = update_tumbler;
    ac_func     [LixEn::STUNNER]   .update = update_stunner;
    ac_func     [LixEn::LANDER]    .update = update_lander;
    ac_func     [LixEn::SPLATTER]  .update = update_splatter;
    ac_func     [LixEn::BURNER]    .update = update_burner;
    ac_func     [LixEn::DROWNER]   .update = update_drowner;
    ac_func     [LixEn::EXITER]    .update = update_exiter;
    ac_func     [LixEn::WALKER]    .update = update_walker;
    ac_func     [LixEn::CLIMBER]   .update = update_climber;
    ac_func     [LixEn::ASCENDER]  .update = update_ascender;
    ac_func     [LixEn::FLOATER]   .update = update_floater;
    ac_func     [LixEn::EXPLODER]  .update = update_exploder;
    ac_func     [LixEn::BLOCKER]   .update = update_blocker;
    ac_func     [LixEn::BUILDER]   .update = update_builder;
    ac_func     [LixEn::SHRUGGER]  .update = update_shrugger;
    ac_func     [LixEn::PLATFORMER].update = update_platformer;
    ac_func     [LixEn::SHRUGGER2] .update = update_shrugger2;
    ac_func     [LixEn::BASHER]    .update = update_basher;
    ac_func     [LixEn::MINER]     .update = update_miner;
    ac_func     [LixEn::DIGGER]    .update = update_digger;
    ac_func     [LixEn::JUMPER]    .update = update_jumper;
    ac_func     [LixEn::ROPER]     .update = update_roper;

    ac_func     [LixEn::FALLER]    .pass_top =
    ac_func     [LixEn::TUMBLER]   .pass_top =
    // ac_func     [LixEn::JUMPER]    .pass_top =
    ac_func     [LixEn::FLOATER]   .pass_top = true;

    ac_func     [LixEn::NOTHING]   .nukable =
    ac_func     [LixEn::SPLATTER]  .nukable =
    ac_func     [LixEn::DROWNER]   .nukable =
    ac_func     [LixEn::EXITER]    .nukable =
    ac_func     [LixEn::EXPLODER]  .nukable = false;

    ac_func     [LixEn::BUILDER]   .returns_x =
    ac_func     [LixEn::PLATFORMER].returns_x = 12;

    ac_func     [LixEn::ROPER]     .aiming    = true;
    ac_func     [LixEn::ROPER]     .aim_sound = Sound::ROPER;
}
