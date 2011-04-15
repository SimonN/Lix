/*
 * button.h
 *
 * class Button
 *
 *   Ein rechteckiger Button zum Anklicken. Er hat die beim Element beschrie-
 *   bene Struktur "Status" (ausgeschaltet, gedrückt, eingeschaltet,
 *   permanent eingeschaltet). Letztere Eigenschaft bewirkt, dass nach dem
 *   Zeichnen die Eigenschaft nicht wieder auf "OFF" gesetzt wird, wie es bei
 *   den anderen Eigenschaften der Fall ist.
 *
 * bool warm
 *
 *   Wenn gesetzt, so wird der Button niemals eingedrueckt (down) erscheinen,
 *   sondern wird schon aktiviert, wenn die Maustaste gedrueckt wird.
 *   Normalerweise geschieht das ja erst beim Maus-Loslassen.
 *
 * bool hot
 *
 *   Eine weitere Verschaerfung von warm. Der Button springt bereits an,
 *   wenn die Maus gedrueckt gehalten wird ueber ihm. Dies wird etwa von den
 *   Faehigkeits-Panels im Spiel verwendet.
 *
 * int color_xyz
 *
 *   Diese vier Farbvariablen werden automatisch von der draw()-Funktion bei
 *   jedem Zeichnen gesetzt. Sie koennen von aussen nur gelesen werden, mit
 *   get_color_xyz().
 *
 * bool clicked(int offset_x, int offset_y)
 *
 *   Prueft, ob der Button aktiv angeklickt worden ist. Insgesamt werden diese
 *   Aktionen durchgefuehrt:
 *   1. is_mouse_here(b) wird mit dem Offset ausgefuehrt.
 *      Wenn nein, return false. Sonst geht's weiter:
 *   2. Bei gedrueckter linker Maustaste wird b::Element::status = DOWN gesetzt.
 *      Das wird automatisch entsprechend rueckgesetzt in Button::draw().
 *   3. Bei losgelassener linker Maustaste wird true zurueck gegeben.
 *   Der Sinn der Funktion ist, dass fuer jeden Button folgendes Konstrukt
 *   geschrieben werden kann:
 *   if (meinbutton.clicked(fenster_x, fenster_y)) {
 *       mach_wozu_der_button_gut_ist();
 *   }
 *
 * get_on(), set_on() und set_up() sind virtuell, weil texttype sie ueberlaed.
 *
 */

#pragma once

#include "../element.h"

namespace Api {
class Button : public Element {

private:

    bool warm;       // Direkt beim aktiven Klicken anspringen?
    bool hot;        // Sogar schon beim Gedrueckthalten ganz anspringen?
    int  hotkey;     // Standardmaessig = 0, kann gesetzt werden
                     // Klicks und Hotkeys werden vom Fenster abgefragt
    bool clicked_last_calc;
    bool down;
    bool on;

    int  color_1;    // Buttonfarbe oben links
    int  color_2;    // Buttonfarbe in der Mitte
    int  color_3;    // Buttonfarbe unten rechts
    int  color_text; // Button-Text-Farbe fuer abgeleitete Klassen

public:

    // Konstruktoren und Destruktor
             //     wohin           x        y        x_len      y_len
             Button(int = 0, int = 0, int = 100, int = 20);
    virtual ~Button() {}

            inline bool get_down() const              { return down;                       }
    virtual inline bool get_on  () const              { return on;                         }
            inline void set_down(const bool b = true) { down = b;     set_draw_required(); }
    virtual inline void set_on  (const bool b = true) { on   = b;     set_draw_required(); }
    virtual inline void set_off ()                    { on   = false; set_draw_required(); }

            inline bool get_warm()                    { return warm;  }
            inline void set_warm(const bool b = true) { warm = b;     }
            inline bool get_hot ()                    { return hot;   }
            inline void set_hot (const bool b = true) { hot = b;      }

            inline int  get_hotkey() const            { return hotkey;     }
            inline void set_hotkey(const int i = 0)   { hotkey = i;        }

            inline int  get_color_1()    const        { return color_1;    }
            inline int  get_color_2()    const        { return color_2;    }
            inline int  get_color_3()    const        { return color_3;    }
            inline int  get_color_text() const        { return color_text; }

    virtual inline bool get_clicked()    const { return clicked_last_calc; }

protected:

    virtual void calc_self();
    virtual void draw_self();

};

} // Api
