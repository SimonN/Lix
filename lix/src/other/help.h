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
#include "../network/net_t.h"    // Uint32

#include "../graphic/glob_gfx.h" // Standardfarbe fuer Schattentext
#include "../graphic/torbit.h"   // Textzeichnen auf Torus statt ALLEGRO_BITMAP*

namespace Help {

    // Timer-Funktionen
    extern ALLEGRO_TIMER* timer; // treat this as private if possible
    extern const int      timer_ticks_per_second;

    void timer_initialize();
    void timer_deinitialize();
    int  get_timer_ticks();

    double hypot        (const int,    const int,    const int, const int);
    double random_double(const double, const double); // zw. 1. und 2. Argument
    int    mod          (const int,    const int); // besser als % im Negativen

    struct MainArgs {
        int  scr_f;
        int  scr_x;
        int  scr_y;
        bool sound_load_driver;
    };
    MainArgs parse_main_arguments(int, char*[]);

    std::string version_to_string   (const unsigned long);

    void string_to_nice_case    (std::string&); // Alle ausser 1. Bch.
    void string_remove_extension(std::string&);
    void string_remove_dir      (std::string&);
    void string_cut_to_dir      (std::string&);
    void string_shorten         (std::string&, const ALLEGRO_FONT*, const int);
    std::string
         string_get_extension    (const std::string&);
    char string_get_pre_extension(const std::string&); // 0, wenn keine
    bool string_ends_with        (const std::string&, const std::string&);

    void draw_shaded_text         (Torbit&, ALLEGRO_FONT*, const char*,
                                   int, int, int, int, int);
    void draw_shadow_text         (Torbit&, ALLEGRO_FONT*, const char*,
                                   int, int, ALLEGRO_COLOR);

    void draw_shaded_centered_text(Torbit&, ALLEGRO_FONT*, const char*,
                                   int, int, int, int, int);
    void draw_shadow_centered_text(Torbit&, ALLEGRO_FONT*, const char*,
                                   int, int, ALLEGRO_COLOR);

    void draw_shadow_fixed_number (Torbit&, ALLEGRO_FONT*,
                                   int, int, int, ALLEGRO_COLOR,
                                   bool = false);
    void draw_shadow_fixed_text   (Torbit&, ALLEGRO_FONT*, const std::string&,
                                   int, int, ALLEGRO_COLOR, bool = false);
    void draw_shadow_fixed_updates_used(Torbit&, ALLEGRO_FONT*,
                                   int, int, int, ALLEGRO_COLOR,
                                   bool = false);



    // Funktionszeigertypendefinition f�r die kommenden Suchfunktionen
    typedef void (*DoStr)(std::string&, void*);

    void find_files(const std::string&, const std::string&, DoStr,void*);
    // Die Funktion durchsucht das mit dem ersten Argument angegebene Verzeich-
    // nis nach Dateien, die dem zweiten Argument "*.abc" entsprechen. Es
    // wird ein Pfad/Dateiname relativ zur Lemmings-Executable gefunden.
    // DoStr ist ein Funktionszeiger. Die entsprechende Funktion wird f�r jeden
    // String aufgerufen, der gefunden wird.

    void find_dirs(std::string, DoStr, void*);
    // Wie find_files, allerdings wird hier im angegebenen Ordner nach Unter-
    // verzeichnissen gesucht mit Ausnahme von "." und "..". Diese Funktion
    // ben�tigt keine Suchmaske und somit kein drittes Argument.

    void find_tree(std::string, const std::string&, DoStr, void*);
    // Wie find_files, schlie�t allerdings auch alle Unterverzeichnisse in der
    // Suche nach passenden Dateien ein.

    bool dir_exists (const std::string&);
    bool file_exists(const std::string&);
}
