/*
 * lix/init.cpp
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
    ac_func     [LixEn::WALKER]    .assclk = assclk_walker;
    ac_func     [LixEn::RUNNER]    .assclk = assclk_runner;
    ac_func     [LixEn::CLIMBER]   .assclk = assclk_climber;
    ac_func     [LixEn::FLOATER]   .assclk = assclk_floater;
    ac_func     [LixEn::EXPLODER]  .assclk = assclk_exploder;
    ac_func     [LixEn::EXPLODER2] .assclk = assclk_exploder2;
    ac_func     [LixEn::BUILDER]   .assclk = assclk_builder;
    ac_func     [LixEn::PLATFORMER].assclk = assclk_platformer;

    ac_func     [LixEn::FALLER]    .become = become_faller;
    ac_func     [LixEn::TUMBLER]   .become = become_tumbler;
    ac_func     [LixEn::DROWNER]   .become = become_drowner;
    ac_func     [LixEn::EXITER]    .become = become_exiter;
    ac_func     [LixEn::WALKER]    .become = become_walker;
    ac_func     [LixEn::CLIMBER]   .become = become_climber;
    ac_func     [LixEn::BUILDER]   .become = become_builder;
    ac_func     [LixEn::CUBER]     .become = become_cuber;
    ac_func     [LixEn::PLATFORMER].become = become_platformer;
    ac_func     [LixEn::MINER]     .become = become_miner;
    ac_func     [LixEn::DIGGER]    .become = become_digger;
    ac_func     [LixEn::JUMPER]    .become = become_jumper;

    ac_func     [LixEn::FALLER]    .update = update_faller;
    ac_func     [LixEn::TUMBLER]   .update = update_tumbler;
    ac_func     [LixEn::STUNNER]   .update = update_stunner;
    ac_func     [LixEn::LANDER]    .update = update_lander;
    ac_func     [LixEn::SPLATTER]  .update = update_splatter;
    ac_func     [LixEn::BURNER]    .update = update_burner;
    ac_func     [LixEn::DROWNER]   .update = update_drowner;
    ac_func     [LixEn::EXITER]    .update = update_exiter;
    ac_func     [LixEn::WALKER]    .update = update_walker;
    ac_func     [LixEn::RUNNER]    .update = update_runner;
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
    ac_func     [LixEn::BATTER]    .update = update_batter;
    ac_func     [LixEn::CUBER]     .update = update_cuber;

    ac_func     [LixEn::FALLER]    .pass_top =
    ac_func     [LixEn::TUMBLER]   .pass_top =
    ac_func     [LixEn::FLOATER]   .pass_top = true;

    ac_func     [LixEn::CLIMBER]   .blockable =
    ac_func     [LixEn::ASCENDER]  .blockable =
    ac_func     [LixEn::BLOCKER]   .blockable =
    ac_func     [LixEn::EXPLODER]  .blockable =
    ac_func     [LixEn::BATTER]    .blockable =
    ac_func     [LixEn::CUBER]     .blockable = false;

    ac_func     [LixEn::NOTHING]   .leaving =
    ac_func     [LixEn::SPLATTER]  .leaving =
    ac_func     [LixEn::BURNER]    .leaving =
    ac_func     [LixEn::DROWNER]   .leaving =
    ac_func     [LixEn::EXITER]    .leaving =
    ac_func     [LixEn::EXPLODER]  .leaving =
    ac_func     [LixEn::CUBER]     .leaving = true;

    ac_func     [LixEn::BUILDER]   .returns_x =
    ac_func     [LixEn::PLATFORMER].returns_x = 12;

    ac_func     [LixEn::ROPER]     .aiming    = true;
    ac_func     [LixEn::ROPER]     .sound_aim = Sound::ROPER;

    ac_func     [LixEn::SPLATTER]  .sound_become = Sound::SPLAT;
    ac_func     [LixEn::BURNER]    .sound_become = Sound::FIRE;
    ac_func     [LixEn::DROWNER]   .sound_become = Sound::WATER;
}

