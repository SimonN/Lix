/*
 * api/button_e.h
 *
 * class EnumButton
 *
 *   Ein Textbutton, der beim Draufklicken zur naechsten der paar Optionen
 *   wechselt. Von n Optionen kann er die Nummern 0 bis (n-1) annhemen, nach
 *   (n-1) folgt wieder 0.
 *
 * operator int
 * int get_number()
 *
 *   Beides liefert die Nummer der derzeit gewaehlten Option.
 *
 * virtual void draw_self()
 *
 *   Existiert in dieser Klasse gar nicht, weil sie mit
 *   virtual void TextButton::draw_self() identisch waere.
 *
 */

#pragma once

#include "b_text.h"

namespace Api {
class EnumButton : public TextButton {

private:

    int                       number;
    std::vector <std::string> text_vector;

public:

    //         Wohin           X        Y        X-Laenge   Y-Laenge
    EnumButton(int = 0, int = 0, int = 100, int = 20);
    virtual ~EnumButton();

    inline  int  get_number() { return number; }
    inline  operator int   () { return number; }

            void set_number(const int);
            void add_text  (const std::string&);

protected:

    virtual void calc_self();

};
// Ende Klasse EnumButton
}
// Ende Namensraum
