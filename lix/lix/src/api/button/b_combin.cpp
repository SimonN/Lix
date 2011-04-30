/*
 * button_c.cpp
 *
 */

#include "b_combin.h"
#include "../../other/help.h"

namespace Api {

CombinedButton::CombinedButton(int x, int y, int xl, int yl)
:
    Button(x, y, xl, yl)
{
    object = 0;
}

CombinedButton::CombinedButton(const CombinedButton& b)
:
    Button(b)
{
    text   = b.text;
    object = b.object;
}

CombinedButton::~CombinedButton()
{
}





void CombinedButton::set_object(const Object* ob)
{
    if (get_xl() < 5 || get_yl() < 17) {
        object = 0;
    } else {
        object = ob;
        set_draw_required();
    }
}





void CombinedButton::draw_self()
{
    Button::draw_self();
    const int xh = get_x_here();
    const int yh = get_y_here();

    // Bitmap und Text zusaetzlich auf den Button zeichnen
    if (object && object->cb) {
        const Cutbit* cutbit = &object->cb;
        const int    th = 11;                     // reserv. Texthoehe
        const double bx = get_xl() - 4;
        const double by = get_yl() - 4 - th;
        const double cx = cutbit->get_xl(); // Vermeidet in Fall 2
        const double cy = cutbit->get_yl(); // Teilung durch Ints

        // Fall 1: Das Bild passt in das Fenster. Ungestr. blitten, zentriert.
        if (bx >= cx && by >= cy) {
            cutbit->draw(get_ground(),
             (int)(xh + (get_xl() - cx)/2),
             (int)(yh + (get_yl() - cy - th)/2));
        }

        // Fall 2: Das Bild muss erst kleinskaliert werden, bevor es zentriert
        //         aehnlich wie im ersten Fall geblittet wird.
        else {
            // Eine Seite soll genau passen, wie viel Platz bei der anderen
            // verschwendet wird, ist egal.
            double factor              = bx/cx;
            if (factor > by/cy) factor = by/cy;

            cutbit->draw(get_ground(),
             (int)(xh + (1 + get_xl() - cx*factor     )/2),
             (int)(yh + (    get_yl() - cy*factor - th)/2),
             0,     0, // Frame-Wahl
             false, 0, // Spiegeln, drehen
             factor);
        }
    }
    // Ende Blitten des ersten Cutbit-Frames
    // Jetzt nur noch den Text drunter schreiben
    if (!text.empty()) {
        std::string text_to_write = text;
        Help::string_shorten(text_to_write, font_sml, get_xl() - 4);
        Help::draw_shadow_centered_text(get_ground(), font_sml,
         text_to_write.c_str(), xh+get_xl()/2, yh+get_yl()-15,
         get_color_text(), color[COL_API_SHADOW]);
    }
}
// Ende von draw()

}
// Ende namespace Api
