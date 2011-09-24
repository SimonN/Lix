/*
 * help.h
 *
 * Hilfsfunktionen fuer L++
 *
 * "Was will ich mit std, zwei Doppelpunkten, "cout", und dann dem
 * bitweisen Shift-Operator? printf kann ALLES!" -- Malte Dehling
 *
 */

#pragma once

#include "myalleg.h"

#include <cstdlib> // RAND_MAX
#include <string>
#include <vector>

#include "globals.h"
#include "filename.h"
#include "../network/net_t.h"    // Uint32

#include "../graphic/glob_gfx.h" // Standardfarbe fuer Schattentext
#include "../graphic/torbit.h"   // Textzeichnen auf Torus statt BITMAP*

namespace Help {

    // Timer-Funktionen
    extern volatile Uint32 timer_ticks;
    extern const    int    timer_ticks_per_second;

    void     timer_start();
    unsigned get_timer_ticks_per_draw();

    double hypot        (const int,    const int,    const int, const int);
    double random_double(const double, const double); // zw. 1. und 2. Argument

    // inlined Modulo and /2*2 that works for negatives as expected
    inline int mod(int base, int modulo)
    {
        if (modulo < 0) modulo *= -1;
        const int intermediate = base % modulo;
        return intermediate < 0 ? intermediate + modulo : intermediate;
    }
    inline int even(const int x) {
        return (int) (((unsigned int) x) / 2 * 2);
    }

    std::string version_to_string   (const unsigned long);

    void string_to_nice_case    (std::string&); // Alle ausser 1. Bch.
    void string_shorten         (std::string&, const FONT*, const int);
    bool string_ends_with       (const std::string&, const std::string&);

    void draw_shaded_text         (Torbit&, FONT*, const char*,
                                   int, int, int, int, int);
    void draw_shadow_text         (Torbit&, FONT*, const char*,
                                // x    y    Farbe, Schattenfarbe
                                   int, int, int = color[COL_TEXT],
                                   int = color[COL_API_SHADOW]);

    void draw_shaded_centered_text(Torbit&, FONT*, const char*,
                                   int, int, int, int, int);
    void draw_shadow_centered_text(Torbit&, FONT*, const char*,
                                // x    y    Farbe, Schattenfarbe
                                   int, int, int = color[COL_TEXT],
                                   int = color[COL_API_SHADOW]);

    void draw_shadow_fixed_number (Torbit&, FONT*,
                                   int, int, int, int=color[COL_TEXT],
                                   bool = false, int=color[COL_API_SHADOW]);
    void draw_shadow_fixed_text   (Torbit&, FONT*, const std::string&,
                                   int, int, int=color[COL_TEXT], bool = false,
                                   int=color[COL_API_SHADOW]);
    void draw_shadow_fixed_updates_used(Torbit&, FONT*,
                                   int, int, int, int=color[COL_TEXT],
                                   bool = false,  int=color[COL_API_SHADOW]);



    // Funktionszeigertypendefinition für die kommenden Suchfunktionen
    typedef void (*DoStr)(const Filename&, void*);

    void find_files(const Filename&, const std::string&, DoStr, void*);
    // Die Funktion durchsucht das mit dem ersten Argument angegebene Verzeich-
    // nis nach Dateien, die dem zweiten Argument "*.abc" entsprechen. Es
    // wird ein Pfad/Dateiname relativ zur Lix-Executable gefunden.
    // DoStr ist ein Funktionszeiger. Die entsprechende Funktion wird für jeden
    // String aufgerufen, der gefunden wird.

    void find_dirs(const Filename&, DoStr, void*);
    // Wie find_files, allerdings wird hier im angegebenen Ordner nach Unter-
    // verzeichnissen gesucht mit Ausnahme von "." und "..". Diese Funktion
    // benötigt keine Suchmaske und somit kein drittes Argument.

    void find_tree(const Filename&, const std::string&, DoStr, void*);
    // Wie find_files, schließt allerdings auch alle Unterverzeichnisse in der
    // Suche nach passenden Dateien ein.

    bool dir_exists(const Filename&);
}
