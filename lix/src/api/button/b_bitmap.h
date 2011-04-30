/*
 * button_b.h
 *
 * class BitmapButton
 *
 *   Wenn ein solches Objekt gezeichnet wird, dann wird zunaechst
 *   Button::draw() aufgerufen. Anschliessend wird mit masked_blit das ge-
 *   waehlte X-Frame des Cutbits darueber gezeichnet. Das Y-Frame ist immer
 *   abhaengig vom Button-Status: 0, wenn aus oder gedrueckt, und 1, wenn
 *   eingeschaltet.
 *
 * class Checkbox
 *
 *   Dies ist einfach nur ein Bitmapbutton mit fester Groesse und festem
 *   Bitmap, naemlich dem Kontroll-Haekchen. X-Frame 0 ist leer, X-Frame 1
 *   zeigt den gesetzten Haken. Zum schnellen Umschalten dient toggle(),
 *   zur schnellen Abfrage der Operator bool.
 *
 */

#pragma once

#include "button.h"

#include "../../graphic/cutbit.h"

namespace Api {
class BitmapButton : public Button {

private:

    const Cutbit* cutbit;
    int           x_frame;

public:

    // Konstruktoren, Destruktor
             //           Grund,   Bild,          x,       y
             BitmapButton(const Cutbit&, int = 0, int = 0);
    virtual ~BitmapButton();

    inline  int  get_x_frame() const      { return x_frame;                }
    inline  void set_x_frame(const int i) {        x_frame = i;            }
    inline  int  get_x_frames() const     { return cutbit->get_x_frames(); }

protected:

    virtual void draw_self();

};



class Checkbox : public BitmapButton {
public:              Checkbox(int = 0, int = 0);
    virtual          ~Checkbox();
            void     toggle();
            void     set_checked(const bool b = true);
    inline  bool     get_checked() const { return get_x_frame() == 1; }
    inline  operator bool()        const { return get_x_frame() == 1; }
protected:
    virtual void     calc_self();
};

} // Ende Namensraum Api
