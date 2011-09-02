/*
 * bitmap/sound.h
 *
 * Die Sound-Singleton-Klasse uebernimmt das Abspielen von Sound (und spaeter
 * vielleicht der Musik). Die hier beschriebenen Methoden sind alle statisch.
 *
 * static void initialize()
 * static void deinitialize()
 *
 *   Dies sollte von Main aufgerufen werden. Es werden der Treiber und Sounds
 *   geladen bzw. wieder zerstoert.
 *
 * Geladene Sounds koennen jederzeit zum Abspielen vorgemerkt werden mit
 * der play()-Funktion. Das geschieht ueblicherweise in den Calculate-Funk-
 * tionen der einzelnen Programm-Unterklassen.

 * Wenn LMain die Zeichenfunktionen aufruft, dann sollte dort auch
 * Sound::draw() aufgerufen werden. Alle abzuspielenden Sounds werden dann
 * gespielt. Wurde ein Sound mehrfach vorgemerkt, wird er hingegen nur einmal
 * gespielt. Also:
 *
 * static void play_loud(Id)
 *
 *   Merkt den Sound mit der entsprechenden ID vor, damit er beim naechsten
 *   Aufruf von draw() laut abgespielt wird.
 *
 * static void play_quiet(Id)
 *
 *   Wie play_loud(), spielt den Ton allerdings leise ab, falls in den Optionen
 *   das Abspielen von Gegner-Klaengen aktiviert ist. Wurde sowohl lautes als
 *   auch leises Abspielen vorgemerkt, wird bei draw() nur laut abgespielt.
 *
 * static void play_loud_if(Id, bool)
 *
 *   Wenn der boolsche Ausdruck wahr ist, dann ist dies das Gleiche wie
 *   play_loud(), andernfalls das Gleiche wie play_quiet().
 *
 * static void draw()
 *
 *   Spielt alle vorgemerkten Sounds ab, mehrfach vorgemerkte nur einmal.
 *   Wurden beide Lautstaerken vorgemerkt, wird lediglich laut abgespielt.
 *
 */

#pragma once

#include "../other/myalleg.h"

#include <string>
#include <vector>

class Sound {

public:

    enum Id {
        NOTHING,
        SAVE,        // File gets saved. Not used right now.

        PANEL,       // Choice of skill on the panel
        ASSIGN,      // Assignment of a skill to a lix
        CLOCK,       // Once per second played when the clock is low on time

        LETS_GO,     // Lets-go sound played after starting a level
        HATCH_OPEN,  // Entrance hatches open
        HATCH_CLOSE, // Entrance hatches close
        OBLIVION,    // Feep: lix walks/falls out of the level area
        FIRE,        // Lix catches on fire and burns to death
        WATER,       // Glug-glug: lix falls into water
        GOAL,        // Any lix enters the player's own goal
        GOAL_BAD,    // Own lix enters an opponent's goal
        YIPPIE,      // Single player: Enough lixes saved
        OVERTIME,    // Beginning of overtime. DING!

        OUCH,        // Tumbler hits the ground and becomes a stunner
        SPLAT,       // Lix splats because of high drop distance
        OHNO,        // Nuke, or L1-style exploder begins oh-no animation
        POP,         // L1-/L2-Exploder explodes
        BRICK,       // Builder/Platformer lays down his last three bricks
        STEEL,       // Ground remover hits steel and stops
        CLIMBER,     // Jumper sticks against the wall and starts to climb
        ROPER,       // Roper shoots
        ROPER_HIT,   // Roper hook hits terrain
        BATTER_MISS, // Batter doesn't hit anything
        BATTER_HIT,  // Batter hits something

        AWARD_1,     // Player is first in multiplayer
        AWARD_2,     // Player is second or ties for first place, no super tie.
        AWARD_3,     // Player is not last, but no award_1/2, or super tie.
        AWARD_4,     // Player is last or among last, no super tie.

        MAX          // Kein Sound, nur die Anzahl Vektor-Elemente
    };

private:

    class Sample {
    private:
        std::string filename;
        SAMPLE*     sample;
        bool        unique;        // vor Neuabspiel: den Alten ggf. stoppen
        bool        loud;          // vorgemerkt zum Abspiel, normale Lautst.
        bool        quiet;         // vorgemerkt zum Abspiel recht leise
        bool        last_was_loud; // Letzes Abspielen war laut statt leise
    public:
        Sample();
        Sample(const std::string&); // Dateiname
        Sample(const Sample&);
        ~Sample();
        Sample& operator = (const Sample&);

        inline const std::string& get_filename() const { return filename; }
        inline       bool         get_unique()   const { return unique;   }
        inline       void         set_unique(bool b = true) { unique = b; }
        inline       void         set_loud  (bool b = true) { loud   = b; }
        inline       void         set_quiet (bool b = true) { quiet  = b; }

                     void         draw();
    };

    static Sound* singleton;

    std::vector <Sample> sample;

    // Machen die Arbeit von (de)initialize()
    Sound();
    ~Sound();

    // Kopierverbot
    Sound(const Sound&);
    void operator = (const Sound&);

public:

    static void initialize();
    static void deinitialize();

    static void play_loud   (const Id&);
    static void play_quiet  (const Id&);
    static void play_loud_if(const Id&, const bool);

    static void draw();

};
