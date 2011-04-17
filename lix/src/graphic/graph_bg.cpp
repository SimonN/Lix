// Klasse fuer Grafikobjekte aller Art

#include "graph_bg.h"

#include "../other/help.h"

GraphicBG::GraphicBG(const Cutbit& c, Torbit& gr, int new_x, int new_y)
:
    Graphic(c, gr, new_x, new_y),
    drawn  (false),
    x_drawn(0),
    y_drawn(0)
{
    bg = al_create_bitmap(get_xl(), get_yl());
}

GraphicBG::GraphicBG(const GraphicBG& o)
:
    Graphic(o),
    drawn  (o.drawn),
    x_drawn(o.x_drawn),
    y_drawn(o.y_drawn)
{
    bg = al_create_bitmap(get_xl(), get_yl());
    al_set_target_bitmap(bg);
    al_draw_bitmap(o.bg, 0, 0, 0);
}

GraphicBG& GraphicBG::operator = (const GraphicBG& o)
{
    if (this == &o || &bg == &o.bg) return *this;
    Graphic::operator = (o);
    al_destroy_bitmap(bg);
    bg = al_clone_bitmap(o.bg);
    drawn   = o.drawn;
    x_drawn = o.x_drawn;
    y_drawn = o.y_drawn;
    return *this;
}

GraphicBG::~GraphicBG()
{
    if (drawn) undraw();
    al_destroy_bitmap(bg);
}









void GraphicBG::undraw()
{
    if (drawn) {
        drawn = false;
        // restore the bg
        al_set_target_bitmap(get_ground().get_al_bitmap());
        Help::set_al_transparency_off();
        al_draw_bitmap(bg, x_drawn, y_drawn, 0);
        Help::set_al_transparency_on();
    }
}



void GraphicBG::draw()
{
    drawn   = true;
    x_drawn = get_x();
    y_drawn = get_y();
    // save the bg
    al_set_target_bitmap(bg);
    Help::set_al_transparency_off();
    al_draw_bitmap_region(get_ground().get_al_bitmap(),
     get_x(), get_y(), get_xl(), get_yl(), 0, 0, 0);
    Help::set_al_transparency_on();
    Graphic::draw();
}
