/*
 * lemming/ac_func.h
 *
 * Achtung: Hier wird nicht die Klasse AcFunc definiert! Das geschieht in
 * lemming/lix.h. Hier werden nur die globalen Funktionen deklariert,
 * die in Lemming::Initializer::Initializer() dem ac_func-Vektor zugewiesen
 * werden.
 *
 */

#pragma once

void assign_faller    (Lixxie&);
void assign_tumbler   (Lixxie&);
void assign_stunner   (Lixxie&);
void assign_lander    (Lixxie&);
void assign_splatter  (Lixxie&);
void assign_burner    (Lixxie&);
void assign_drowner   (Lixxie&);
void assign_exiter    (Lixxie&);
void assign_walker    (Lixxie&);
void assign_climber   (Lixxie&);
void assign_ascender  (Lixxie&);
void assign_floater   (Lixxie&);
void assign_exploder  (Lixxie&);
void assign_blocker   (Lixxie&);
void assign_builder   (Lixxie&);
void assign_shrugger  (Lixxie&);
void assign_platformer(Lixxie&);
void assign_shrugger2 (Lixxie&);
void assign_basher    (Lixxie&);
void assign_miner     (Lixxie&);
void assign_digger    (Lixxie&);
void assign_jumper    (Lixxie&);
void assign_roper     (Lixxie&);

void update_faller    (Lixxie&, const UpdateArgs&);
void update_tumbler   (Lixxie&, const UpdateArgs&);
void update_stunner   (Lixxie&, const UpdateArgs&);
void update_lander    (Lixxie&, const UpdateArgs&);
void update_splatter  (Lixxie&, const UpdateArgs&);
void update_drowner   (Lixxie&, const UpdateArgs&);
void update_burner    (Lixxie&, const UpdateArgs&);
void update_exiter    (Lixxie&, const UpdateArgs&);
void update_walker    (Lixxie&, const UpdateArgs&);
void update_climber   (Lixxie&, const UpdateArgs&);
void update_ascender  (Lixxie&, const UpdateArgs&);
void update_floater   (Lixxie&, const UpdateArgs&);
void update_exploder  (Lixxie&, const UpdateArgs&);
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
