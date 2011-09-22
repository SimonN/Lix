/*
 * api/number.h
 *
 * Feld mit einer Ganzzahl und Buttons, welche die Zahl aendern koennen.
 *
 */

#pragma once

#include "element.h"
#include "frame.h"
#include "button/b_bitmap.h"

namespace Api {
class Number : public Element {

private:

    int      digits;
    bool     six_buttons;

    int      number;

    int      minimum;
    int      maximum;
    unsigned step_sml;
    unsigned step_med;
    unsigned step_big;
    bool     white_zero;
    bool     format_time;
    bool     format_hex;
    bool     show_sign;
    char     minus_one_char;

    BitmapButton down_big;
    BitmapButton down_med;
    BitmapButton down_sml;
    BitmapButton   up_sml;
    BitmapButton   up_med;
    BitmapButton   up_big;

public:

    //     Grund,   x,   y,    X-Laenge, Ziffern, Minimum, Maximum, Anfangsw.
    Number(int, int,  const unsigned = 110, const unsigned = 2,
                               const int = 0, const int = 99,  const int = 0,
                               const bool = false); // bool: Sechs Buttons
    virtual ~Number();

           bool get_clicked () const;

           void set_minimum (const int);
           void set_maximum (const int);
    inline int  get_minimum () const           { return minimum;  }
    inline int  get_maximum () const           { return maximum;  }
    inline void set_step_sml(const unsigned i) { step_sml = i;    }
    inline void set_step_med(const unsigned i) { step_med = i;    }
    inline void set_step_big(const unsigned i) { step_big = i;    }
           void set_six_buttons(bool);

    inline int  get_number() const             { return number;   }
           void set_number(const int);

    inline void set_show_sign(bool b = true)   { show_sign = b;   }
    inline void set_minus_one_char(char c)     { minus_one_char = c; }
    inline void set_white_zero (bool b = true) { white_zero  = b; }
    inline void set_format_time(bool b = true) { format_time = b; }
    inline void set_format_hex (bool b = true) { format_hex  = b; }

           void set_macro_color(int = 0); // initial value

protected:

    virtual void calc_self();
    virtual void draw_self();

}; // Ende der Klasse Number
}  // Ende Namensraum Api
