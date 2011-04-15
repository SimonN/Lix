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

    const unsigned digits;
    const bool     six_buttons;

    int      number;

    int      minimum;
    int      maximum;
    unsigned step_sml;
    unsigned step_med;
    unsigned step_big;
    bool     white_zero;
    bool     format_time;
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

           void set_minimum (const int);
           void set_maximum (const int);
    inline int  get_minimum ()                 { return minimum;  }
    inline int  get_maximum ()                 { return maximum;  }
    inline void set_step_sml(const unsigned i) { step_sml = i;    }
    inline void set_step_med(const unsigned i) { step_med = i;    }
    inline void set_step_big(const unsigned i) { step_big = i;    }

    inline void set_show_sign(bool b = true)   { show_sign = b;   }
    inline void set_minus_one_char(char c)     { minus_one_char = c; }
    inline bool get_show_sign()                { return show_sign; }
    inline char get_minus_one_char()           { return minus_one_char; }

    inline int  get_number()                   { return number;   }
           void set_number(const int);
    inline void set_white_zero (bool b = true) { white_zero  = b; }
    inline void set_format_time(bool b = true) { format_time = b; }

protected:

    virtual void calc_self();
    virtual void draw_self();

}; // Ende der Klasse Number
}  // Ende Namensraum Api
