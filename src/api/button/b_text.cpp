#include "b_text.h"

#include "../../graphic/gra_lib.h"
#include "../../other/help.h"

namespace Api {

TextButton::TextButton(int x, int y, int xl, int yl, const std::string& str)
:
    Button(x, y, xl, yl),
    text  (str),
    left  (false),

    check_cutbit(&GraLib::get(gloB->file_bitmap_checkbox)),
    check_frame (0)
{
}

TextButton::TextButton(const TextButton& b)
:
    Button(b),
    text  (b.text),
    left  (b.left),

    check_cutbit(b.check_cutbit),
    check_frame (b.check_frame)
{
}

TextButton::~TextButton() {
}











void TextButton::draw_self()
{
    Button::draw_self();
    const int xh = get_x_here();
    const int yh = get_y_here();

    if (!text.empty()) {
        // Warum machen wir das hier in jedem Zeichnungstick und nicht nur beim
        // Textsetzen? Verzeichnis-Suchroutinen fragen den Text naemlich ab,
        // und die Aenderung des Hakens kann die Anzeigelaenge aendern.
        // Die -5 machen, dass es gut aussieht und nicht uebersteht.
        // Bei sichtbarem Haekchen zusaetzlich schnibbeln.
        std ::string s = text;
        Help::string_shorten(s, font_med, get_xl() - 5
                             - (check_frame != 0) * check_cutbit->get_xl());
        if (left) {
            Help::draw_shadow_text(get_ground(), font_med,
             s.c_str(), xh + 3, yh + get_yl()/2 - 10,
             get_color_text(), color[COL_API_SHADOW]);
        } else {
            Help::draw_shadow_centered_text(get_ground(),
             font_med, s.c_str(), xh + get_xl()/2,
                                  yh + get_yl()/2 - 10,
             get_color_text(), color[COL_API_SHADOW]);
        }

        // Haken malen
        if (check_frame) check_cutbit->draw(get_ground(),
         xh + get_xl() - check_cutbit->get_xl(),
         yh, check_frame, 2*get_on());
    }

}
// Ende TextButton::draw_self()

}
// Ende Namensraum
