#include "label.h"
#include "../other/help.h"

namespace Api {

const int Label::element_yl(20);



Label::Label(
    int                x,
    int                y,
    const std::string& tx,
    const Align&       al
) :
    Element     (x, y, 0, element_yl),
    font        (font_med),
    text        (tx),
    align       (al),
    color       (::color[COL_TEXT]),
    drawn_x_here(0),
    drawn_xl    (0),
    drawn_align (LEFT)
{
    // Choose this as the standard undraw color
    set_undraw_color(::color[COL_API_M]);
}



Label::Label(
    int     x,
    int     y,
    int     nr
) :
    Element     (x, y, 0, element_yl),
    font        (font_med),
    align       (BLOCKY),
    color       (::color[COL_TEXT]),
    drawn_x_here(0),
    drawn_xl    (0),
    drawn_align (BLOCKY)
{
    set_undraw_color(::color[COL_API_M]);
    set_number(nr);
}




void Label::set_text(const std::string& s)
{
    text = s;
    if (align == BLOCKY) set_xl(10 * text.size());
    else                 set_xl(::text_length(font, text.c_str()));
    set_draw_required();
}



void Label::set_number(int nr)
{
    text.clear();
    std::vector <char> reversed;
    if (nr < 0) {
        nr *= -1;
        text += '-';
    }
    if (nr == 0) text += '0';
    for ( ; nr > 0; nr /= 10) reversed.push_back(nr % 10 + '0');
    for (std::vector <char>     ::const_iterator itr = --reversed.end();
     itr != --reversed.begin(); --itr) text += (char) *itr;

    set_xl(10 * text.size());
    set_draw_required();
}



// Parse the number back out of the string. This is rarely useful, but
// the gameplay panel does this with the rate.
int Label::get_number() const
{
    bool minus = false;
    int  ret   = 0;
    for (std::string::const_iterator
     itr = text.begin(); itr != text.end(); ++itr) {
        if (*itr >= '0' && *itr <= '9') {
            ret *= 10;
            ret += *itr - '0';
        }
        else if (*itr == '-') minus = true;
    }
    if (minus) ret *= -1;
    return ret;
}



void Label::set_align(const Align& a)
{
    align = a;
    if (align == BLOCKY) set_xl(10 * text.size());
    else                 set_xl(::text_length(font, text.c_str()));
    set_draw_required();
}



void Label::set_color(const int c)
{
    color = c;
    set_draw_required();
}



void Label::draw_self()
{
    // Removes old versions of the text
    undraw_self();
    // As said in the .h file: x is left for LEFT as usual, the center for
    // CENTERED, and the right end of the text for BLOCKY.
    if (align == LEFT || align == CENTERED) {
        drawn_xl     = text_length(font, text.c_str());
        drawn_x_here = get_x_here();
        if (align == CENTERED) drawn_x_here -= drawn_xl / 2;
        Help::draw_shadow_text(get_ground(), font, text.c_str(),
         drawn_x_here, get_y_here(), color, ::color[COL_API_SHADOW]);
    }
    else {
        drawn_xl     = text.length() * 10;
        drawn_x_here = get_x_here() - drawn_xl;
        char one_digit[2] = " ";
        for (int i = 0; i < (int) text.size(); ++i) {
            one_digit[0] = text[text.size() - 1 - i];
            Help::draw_shadow_text(get_ground(), font, one_digit,
             get_x_here() - 10*(i+1), get_y_here(),
             color, ::color[COL_API_SHADOW]);
        }
    }
}



void Label::undraw_self()
{
    if (drawn_xl > 0 && get_undraw_color()) {
        ::rectfill(get_ground().get_al_bitmap(),
         drawn_x_here, get_y_here() + 2, drawn_x_here + drawn_xl - 1,
         get_y_here() + get_yl() - 3, get_undraw_color());
    }
}



}
// end of namespace Api
