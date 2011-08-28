/*
 * lix/ac_func.h
 *
 * Achtung: Hier wird nicht die Klasse AcFunc definiert! Das geschieht in
 * lix/lix.h. Hier werden nur die globalen Funktionen deklariert,
 * die in Lixxie::Initializer::Initializer() dem ac_func-Vektor zugewiesen
 * werden.
 *
 */

#pragma once

void assclk_faller    (Lixxie&);
void assclk_tumbler   (Lixxie&);
void assclk_stunner   (Lixxie&);
void assclk_lander    (Lixxie&);
void assclk_splatter  (Lixxie&);
void assclk_burner    (Lixxie&);
void assclk_drowner   (Lixxie&);
void assclk_exiter    (Lixxie&);
void assclk_walker    (Lixxie&);
void assclk_runner    (Lixxie&);
void assclk_climber   (Lixxie&);
void assclk_ascender  (Lixxie&);
void assclk_floater   (Lixxie&);
void assclk_exploder  (Lixxie&);
void assclk_exploder2 (Lixxie&);
void assclk_blocker   (Lixxie&);
void assclk_builder   (Lixxie&);
void assclk_shrugger  (Lixxie&);
void assclk_platformer(Lixxie&);
void assclk_shrugger2 (Lixxie&);
void assclk_basher    (Lixxie&);
void assclk_miner     (Lixxie&);
void assclk_digger    (Lixxie&);
void assclk_jumper    (Lixxie&);
void assclk_roper     (Lixxie&);
void assclk_batter    (Lixxie&);
void assclk_cuber     (Lixxie&);

void become_faller    (Lixxie&);
void become_tumbler   (Lixxie&);
void become_stunner   (Lixxie&);
void become_lander    (Lixxie&);
void become_splatter  (Lixxie&);
void become_burner    (Lixxie&);
void become_drowner   (Lixxie&);
void become_exiter    (Lixxie&);
void become_walker    (Lixxie&);
void become_runner    (Lixxie&);
void become_climber   (Lixxie&);
void become_ascender  (Lixxie&);
void become_floater   (Lixxie&);
void become_exploder  (Lixxie&);
void become_exploder2 (Lixxie&);
void become_blocker   (Lixxie&);
void become_builder   (Lixxie&);
void become_shrugger  (Lixxie&);
void become_platformer(Lixxie&);
void become_shrugger2 (Lixxie&);
void become_basher    (Lixxie&);
void become_miner     (Lixxie&);
void become_digger    (Lixxie&);
void become_jumper    (Lixxie&);
void become_roper     (Lixxie&);
void become_batter    (Lixxie&);
void become_cuber     (Lixxie&);

void update_faller    (Lixxie&, const UpdateArgs&);
void update_tumbler   (Lixxie&, const UpdateArgs&);
void update_stunner   (Lixxie&, const UpdateArgs&);
void update_lander    (Lixxie&, const UpdateArgs&);
void update_splatter  (Lixxie&, const UpdateArgs&);
void update_drowner   (Lixxie&, const UpdateArgs&);
void update_burner    (Lixxie&, const UpdateArgs&);
void update_exiter    (Lixxie&, const UpdateArgs&);
void update_walker    (Lixxie&, const UpdateArgs&);
void update_runner    (Lixxie&, const UpdateArgs&);
void update_climber   (Lixxie&, const UpdateArgs&);
void update_ascender  (Lixxie&, const UpdateArgs&);
void update_floater   (Lixxie&, const UpdateArgs&);
void update_exploder  (Lixxie&, const UpdateArgs&);
void update_exploder2 (Lixxie&, const UpdateArgs&);
void update_blocker   (Lixxie&, const UpdateArgs&);
void update_builder   (Lixxie&, const UpdateArgs&);
void update_shrugger  (Lixxie&, const UpdateArgs&);
void update_platformer(Lixxie&, const UpdateArgs&);
void update_shrugger2 (Lixxie&, const UpdateArgs&);
void update_basher    (Lixxie&, const UpdateArgs&);
void update_miner     (Lixxie&, const UpdateArgs&);
void update_digger    (Lixxie&, const UpdateArgs&);
void update_jumper    (Lixxie&, const UpdateArgs&);
void update_roper     (Lixxie&, const UpdateArgs&);
void update_batter    (Lixxie&, const UpdateArgs&);
void update_cuber     (Lixxie&, const UpdateArgs&);
