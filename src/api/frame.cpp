#include "frame.h"

namespace Api {

Frame::Frame(int nx, int ny, int xl, int yl)
:
    Element(nx, ny, xl, yl)
{
}

Frame::~Frame()
{
}

void Frame::draw_self()
{
    // Diese hier machen das Zeichnen übersichtlicher.
    BITMAP* g    = get_ground().get_al_bitmap();
    const int x1 = get_x_here() - 2;
    const int y1 = get_y_here() - 2;
    const int x2 = get_x_here() + 1 + get_xl();
    const int y2 = get_y_here() + 1 + get_yl();

    // Farben wählen
    int col_1 = color[COL_API_D];
    int col_3 = color[COL_API_L];

    // Acquiren, malen, freilassen
    acquire_bitmap(g);
    vline   (g, x1,   y1,         y2-1, col_1); // Links  außen
    vline   (g, x1+1, y1,         y2-2, col_1); // Links  innen
    hline   (g, x1+2, y1,   x2-1,       col_1); // Oben   außen
    hline   (g, x1+2, y1+1, x2-2,       col_1); // Oben   innen
    vline   (g, x2,   y1+1,       y2,   col_3); // Rechts außen
    vline   (g, x2-1, y1+2,       y2,   col_3); // Rechts innen
    hline   (g, x1+1, y2,   x2-2,       col_3); // Unten  außen
    hline   (g, x1+2, y2-1, x2-2,       col_3); // Unten  innen
    release_bitmap(g);
}



void Frame::undraw_self()
{
    if (!get_undraw_color()) return;

    const int x1 = get_x_here() - 2;
    const int y1 = get_y_here() - 2;
    const int x2 = get_x_here() + 1 + get_xl();
    const int y2 = get_y_here() + 1 + get_yl();

    rectfill(get_ground().get_al_bitmap(), x1, y1, x2, y2, get_undraw_color());
}

} // Api
