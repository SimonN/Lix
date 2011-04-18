/*
 * other/myalleg.h
 *
 * It appears that we have to include winalleg.h since I upgraded to Allegro
 * 4.4. The problem is that both the MinGW windows API and Allegro define
 * the type BITMAP, and winalleg.h does define magic to get around this.
 *
 * If something requires Allegro functions, always include myalleg.h, and
 * always make this the first file to be included. If you include several
 * headers with none of them being myalleg.h, but one of them includes
 * myalleg.h itself, include that header first.
 *
 */

#pragma once

#include <allegro.h>

#ifdef _WIN32
    #include <winalleg.h>
#endif
