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
    bg = create_bitmap(get_xl(), get_yl());
}

GraphicBG::GraphicBG(const GraphicBG& o)
:
    Graphic(o),
    drawn  (o.drawn),
    x_drawn(o.x_drawn),
    y_drawn(o.y_drawn)
{
    bg = create_bitmap(get_xl(), get_yl());
    blit(o.bg, bg, 0, 0, 0, 0, get_xl(), get_yl());
}

GraphicBG& GraphicBG::operator = (const GraphicBG& o)
{
    Graphic::operator = (o);
    // Umweg ueber temporaeren Zeiger macht Selbstzuweisung absturzfrei
    BITMAP* temp = create_bitmap(get_xl(), get_yl());
    blit(o.bg, temp, 0, 0, 0, 0, get_xl(), get_yl());
    destroy_bitmap(bg);
    bg = temp;
    return *this;
}

GraphicBG::~GraphicBG()
{
    if (drawn) undraw();
    destroy_bitmap(bg);
}









void GraphicBG::undraw()
{
    if (drawn) {
        drawn = false;
        blit(bg, get_ground().get_al_bitmap(), 0, 0,
         x_drawn, y_drawn, bg->w, bg->h);
    }
}



void GraphicBG::draw()
{
    drawn   = true;
    x_drawn = get_x();
    y_drawn = get_y();
    // Hintergrund sichern
    blit(get_ground().get_al_bitmap(), bg,
     get_x(), get_y(), 0, 0, bg->w, bg->h);
    Graphic::draw();
}
