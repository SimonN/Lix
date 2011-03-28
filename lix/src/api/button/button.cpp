#include "button.h"

#include "../../other/hardware.h"
#include "../../other/log.h"

namespace Api {

Button::Button(int nx, int ny, int xl, int yl)
:
    Element(nx, ny, xl, yl),
    warm  (false),
    hot   (false),
    hotkey(    0),
    clicked_last_calc(false),
    down  (false),
    on    (false)
{
    // Farben werden ohnehin beim Zeichnen gewaehlt
}



// Liefert nur true, wenn der Button gerade fertig angeklickt wurde
void Button::calc_self()
{
    bool mouse_here = is_mouse_here();

    if (get_hidden()) {
        clicked_last_calc = false;
    }
    else {
        // Eingedrueckt erscheinen? Nur im kalten Modus oder bei ausgeschalt.
        // Buttons auch im warmen Modus kontrollieren, nie im heissen Modus.
        if (!hot) {
            if (mouse_here && hardware.get_mlh() && (!warm || !on)) {
                if (!down) set_draw_required();
                down = true;
            }
            else {
                if (down) set_draw_required();
                down = false;
            }
        }
        // Aktives Anklicken pruefen
        // ALLEGRO_KEY_ENTER als Hotkey laesst auch ALLEGRO_KEY_ENTER_PAD zu!
        bool b =
            (!warm && !hot && mouse_here && hardware.get_mlr())
         || ( warm && !hot && mouse_here && hardware.get_ml ())
         || (          hot && mouse_here && hardware.get_mlh());
        // See hardware.h/cpp for why Enter is separated
        if (hotkey == ALLEGRO_KEY_ENTER) b = b || hardware.key_enter_once();
        else                     b = b || hardware.key_once(hotkey);
        clicked_last_calc = b;
    }
}



void Button::draw_self() {
    // Diese hier machen das Zeichnen übersichtlicher.
    ALLEGRO_BITMAP* g    = get_ground().get_al_bitmap();
    const int x1 = get_x_here();
    const int y1 = get_y_here();
    const int x2 = get_x_here() + get_xl();
    const int y2 = get_y_here() + get_yl();

    if (down) {
        color_1    = color[COL_API_DOWN_D];
        color_2    = color[COL_API_DOWN_M];
        color_3    = color[COL_API_DOWN_L];
        color_text = color[COL_TEXT  ];
    } else if (on) {
        color_1    = color[COL_API_ON_D];
        color_2    = color[COL_API_ON_M];
        color_3    = color[COL_API_ON_L];
        color_text = color[COL_TEXT_ON   ];
    } else {
        color_1    = color[COL_API_L   ];
        color_2    = color[COL_API_M   ];
        color_3    = color[COL_API_D   ];
        color_text = color[COL_TEXT];
    }

    // Jetzt wird's ernst
    al_set_target_bitmap(g);
    al_draw_filled_rectangle(x1+2, y1+2, x2-2, y2-2, color_2); // Mittelfläche
    al_draw_filled_rectangle(x1,   y1,   x1+2, y2-1, color_1); // left
    al_draw_filled_rectangle(x1,   y1,   x2-1, y1+2, color_1); // top
    al_draw_filled_rectangle(x2-2, y1+1, x2,   y2-1, color_3); // right
    al_draw_filled_rectangle(x1+1, y2-2, x2-1, y2,   color_3); // bottom
    al_put_pixel            (x1,   y2-1,             color_2); // lower left
    al_put_pixel            (x1+1, y2-2,             color_2); // lower left
    al_put_pixel            (x2-2, y1+1,             color_2); // upper right
    al_put_pixel            (x2-1, y1,               color_2); // upper right
}

} // Api
