/*
 * api/label.h
 *
 * A text label. Can be regular text color or white, and left-handed,
 * centered, or blocky. Blocky text is drawn from the right towards the left.
 *
 * x and y are the start of the text on the parent; the start can be at the
 * left, the center, or the right. Undrawing/redrawing observers this.
 *
 * void set_number(const int)
 *
 *   This switches to align = BLOCKY and sets the text to a parse of the
 *   number including maybe the minus sign at the front.
 *
 */

#pragma once

#include "element.h"

namespace Api {

class Label : public Element {

public:

    static const int element_yl;

    enum Align {
        LEFT,
        CENTERED,
        BLOCKY
    };

    Label(int = 0, int = 0, const std::string& = "", const Align& = LEFT);
    Label(int, int, int);
    virtual ~Label() {}

    const std::string& get_text ()  const { return text;  }
    const Align&       get_align()  const { return align; }
          int          get_color()  const { return color; }
          int          get_number() const; // only useful if the text is an int

    void set_font      (FONT* f) { font = f; }
    void set_text      (const std::string& = "");
    void set_number    (const int);
    void set_align     (const Align&);
    void set_color     (const int);

private:

    FONT*       font;
    std::string text;
    Align       align;
    int         color;

    int         drawn_x_here; // Like Element::xl, but last drawn, not current
    int         drawn_xl;
    Align       drawn_align;  // drawn_length is put into Element::xl.

protected:

    virtual void draw_self();
    virtual void undraw_self();

}; // end class Label
}  // end namespace Api
