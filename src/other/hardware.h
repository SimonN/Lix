/*
 * hardware.h
 *
 * Es wird ganz unten per "extern Hardware hardware" auf ein neues Objekt
 * verwiesen. Dieses wird in der Datei "hardware.cpp" erschaffen.
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

//////////
// Maus //
//////////

private:

    int      mouse_own_x;  // Hier wird gespeichert, was bei get_mouse_x
    int      mouse_own_y;  // bzw. -_y zurueckgegeben wird
    int      mickey_x;     // Wie weit veraendert seit dem letzten
    int      mickey_y;     // main_loop? Bei ruhender Maus z.B. null.

    bool     mouse_click  [3]; // gerade angeklickt
    bool     mouse_double [3]; // gerade doppelt geklickt
    unsigned mouse_hold   [3]; // wird gehalten seit... (ungehalten: 0)
    unsigned mouse_release[3]; // gerade losgelassen - wie lange war gedrueckt?

    bool     mouse_buffer [3]; // Klicks zwischen den Ticks? (intern)
    unsigned mouse_since  [3]; // Wie lange seit letztem Klick (intern)

public:

    // Fuer Polling-Komponenten
    static const unsigned doubleclick_speed;
    static const unsigned doubleclick_for60;
    inline int  get_mickey_x()        { return mickey_x;         }
    inline int  get_mickey_y()        { return mickey_y;         }
    inline int  get_mx()              { return mouse_own_x;      }
    inline int  get_my()              { return mouse_own_y;      }
    inline bool get_ml()              { return mouse_click  [0]; }
    inline bool get_mr()              { return mouse_click  [1]; }
    inline bool get_mm()              { return mouse_click  [2]; }
    inline bool get_mld()             { return mouse_double [0]; }
    inline bool get_mrd()             { return mouse_double [1]; }
    inline bool get_mmd()             { return mouse_double [2]; }
    inline unsigned get_mlh()         { return mouse_hold   [0]; }
    inline unsigned get_mrh()         { return mouse_hold   [1]; }
    inline unsigned get_mmh()         { return mouse_hold   [2]; }
    inline unsigned get_mlr()         { return mouse_release[0]; }
    inline unsigned get_mrr()         { return mouse_release[1]; }
    inline unsigned get_mmr()         { return mouse_release[2]; }



//////////////
// Tastatur //
//////////////

private:

    bool ctrl;  // Eine der beiden CTRL-Tasten  gehalten?
    bool shift; // Eine der beiden Shift-Tasten gehalten?
    bool alt;   // Eine der beiden Alt-Tasten gehalten?
    bool last_tick_ctrl;  // Hardware-internal variable, necessary to enable
    bool last_tick_shift; // get_key() to return modifiers as well
    bool last_tick_alt;

    bool key_buffer[KEY_MAX]; // fuer key_release

    int  key_from_buffer;
    int  key_from_buffer_ascii;

public:

    inline int  get_key()       { return key_from_buffer;       }
    inline int  get_key_ascii() { return key_from_buffer_ascii; }

    inline bool key_once   (int i)   { return key_from_buffer == i;  }
           bool key_release(int); // Important: key_release must be called in
                                  // every logic update to work as expected!

           bool key_enter_once();    // Special because Alt+Enter shall not
           bool key_enter_release(); // trigger it, and both Enters shall work
    inline bool key_hold   (int scancode) { return key[scancode]; }
    inline bool get_ctrl   ()             { return ctrl;          }
    inline bool get_shift  ()             { return shift;         }
    inline bool get_alt    ()             { return alt;           }

/////////////////////////////
// Und alles kontrollieren //
/////////////////////////////

public:

    Hardware();
    ~Hardware();

    void main_loop();
    void poll_mouse(const bool = false);

    void freeze_mouse_x();
    void freeze_mouse_y();

};

// Objekt referenzieren, welches erst in main() erschaffen wird
extern Hardware hardware;
