/*
 * Hardware::h
 *
 * Es wird ganz unten per "extern Hardware hardware" auf ein neues Objekt
 * verwiesen. Dieses wird in der Datei "Hardware::cpp" erschaffen.
 *
 *   ml = button no. 0 = left button
 *   mr = button no. 1 = right button
 *   mm = button no. 2 = middle button
 *
 * void main_loop()
 *
 *   Fragt die Hardware ab und setzt abfragbare Variablen.
 *
 * void poll_mouse(const bool increase_ticks = false)
 *
 *   Dies soll den Yassin-Bug umgehen: Manchmal werden kurze Klicks nicht
 *   registriert. Sinn der Uebung ist es, die Maus auch dann abzufragen,
 *   waehrend wir auf das Zeichnen warten. In der Warte-While-Schleife kann
 *   etwa sein: { poll_mouse(); rest(1); }
 *
 *   Nur vom Hardware-Main-Loop sollte true uebergeben werden, sonst werden
 *   viel zu viele Ticks hochgezaehlt.
 *
 * void freeze_mouse_x(), -_y()
 *
 *   Bringt die Maus auf die Position vor dem letzten main_loop(), indem
 *   einfach die Mickeys wieder von der Position abgezogen werden.
 *
 *   Das macht Sinn, wenn im Spiel mit rechter Maustaste gescrollt wird.
 *   Die Hardware kann ja von alleine nicht erkennen, wann dies passiert.
 *
 * Maus generell
 *
 *   Die Allegro-Maus wird in jedem main_loop() wieder in die Bildschirmmitte
 *   zurueckgesetzt, um die Bewegung fuer das Siedler-II-Scrollen (rechte
 *   Maustaste gedrueckt halten) auch zu ermoeglichen, wenn die Maus schon am
 *   Rand ist.
 *
 */

#pragma once

#include "../graphic/glob_gfx.h" // LEMSCR_Y

class Hardware {

private:

    Hardware();
    ~Hardware();

    static Hardware* hw;

    ALLEGRO_EVENT_QUEUE* event_queue;

////////////////////
// Display events //
////////////////////

private:

    bool display_is_active;  // if so, centralize mouse
    bool display_was_closed;

public:

    static inline bool get_display_was_closed()
                       { return hw->display_was_closed; }
//////////
// Maus //
//////////

private:

    int  mouse_own_x;  // Hier wird gespeichert, was bei get_mouse_x
    int  mouse_own_y;  // bzw. -_y zurueckgegeben wird
    int  mickey_x;     // Wie weit veraendert seit dem letzten
    int  mickey_y;     // main_loop? Bei ruhender Maus z.B. null.

    bool mouse_click  [3]; // gerade angeklickt
    bool mouse_double [3]; // gerade doppelt geklickt
    int  mouse_hold   [3]; // wird gehalten seit... (ungehalten: 0)
    int  mouse_release[3]; // gerade losgelassen - wie lange war gedrueckt?
    int  mouse_since  [3]; // Wie lange seit letztem Klick (intern)

public:

    // Fuer Polling-Komponenten
    static const  int  doubleclick_speed;
    static const  int  doubleclick_for60;
    static inline int  get_mickey_x()        { return hw->mickey_x;         }
    static inline int  get_mickey_y()        { return hw->mickey_y;         }
    static inline int  get_mx()              { return hw->mouse_own_x;      }
    static inline int  get_my()              { return hw->mouse_own_y;      }
    static inline bool get_ml()              { return hw->mouse_click  [0]; }
    static inline bool get_mr()              { return hw->mouse_click  [1]; }
    static inline bool get_mm()              { return hw->mouse_click  [2]; }
    static inline bool get_mld()             { return hw->mouse_double [0]; }
    static inline bool get_mrd()             { return hw->mouse_double [1]; }
    static inline bool get_mmd()             { return hw->mouse_double [2]; }
    static inline unsigned get_mlh()         { return hw->mouse_hold   [0]; }
    static inline unsigned get_mrh()         { return hw->mouse_hold   [1]; }
    static inline unsigned get_mmh()         { return hw->mouse_hold   [2]; }
    static inline unsigned get_mlr()         { return hw->mouse_release[0]; }
    static inline unsigned get_mrr()         { return hw->mouse_release[1]; }
    static inline unsigned get_mmr()         { return hw->mouse_release[2]; }

//////////////
// Tastatur //
//////////////

private:

    std::vector <bool> key_hold;
    std::vector <bool> key_once;
    std::vector <bool> key_just_up;

    std::string        typed_characters;

public:

    static int                get_typed_key(); // returns -1 if none
    static const std::string& get_typed_characters();

    static bool get_key_once   (int i);
    static bool get_key_release(int);
    static bool get_key_hold   (int);

    static bool get_key_enter_once();    // Special because Alt+Enter shall not
    static bool get_key_enter_release(); // trigger it, both Enters shall work
    static bool get_ctrl   ();
    static bool get_shift  ();
    static bool get_alt    ();

/////////////////////////////
// Und alles kontrollieren //
/////////////////////////////

public:

    static void initialize();
    static void deinitialize();

    static void main_loop();
    static void poll_mouse(const bool = false);

    static void freeze_mouse_x();
    static void freeze_mouse_y();

};
