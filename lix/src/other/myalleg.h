/*
 * other/myalleg.h
 *
 * If something requires Allegro functions, always include myalleg.h, and
 * always make this the first file to be included. If you include several
 * headers with none of them being myalleg.h, but one of them includes
 * myalleg.h itself, include that header first.
 *
 */

#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
