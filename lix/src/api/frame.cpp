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
    const int x1 = get_x_here() - 2;
    const int y1 = get_y_here() - 2;
    const int x2 = get_x_here() + 2 + get_xl();
    const int y2 = get_y_here() + 2 + get_yl();

    const ALLEGRO_COLOR& color_1 = color[COL_API_D];
    const ALLEGRO_COLOR& color_2 = color[COL_API_M];
    const ALLEGRO_COLOR& color_3 = color[COL_API_L];

    al_set_target_bitmap    (get_ground().get_al_bitmap());
    al_draw_filled_rectangle(x1,   y1,   x1+2, y2-1, color_1); // left
    al_draw_filled_rectangle(x1,   y1,   x2-1, y1+2, color_1); // top
    al_draw_filled_rectangle(x2-2, y1+1, x2,   y2-1, color_3); // right
    al_draw_filled_rectangle(x1+1, y2-2, x2-1, y2,   color_3); // bottom
    al_put_pixel            (x1,   y2-1,             color_2); // lower left
    al_put_pixel            (x1+1, y2-2,             color_2); // lower left
    al_put_pixel            (x2-2, y1+1,             color_2); // upper right
    al_put_pixel            (x2-1, y1,               color_2); // upper right
}



void Frame::undraw_self()
{
    if (get_undraw_color() == color[COL_TRANSPARENT]) return;

    const int x1 = get_x_here() - 2;
    const int y1 = get_y_here() - 2;
    const int x2 = get_x_here() + 2 + get_xl();
    const int y2 = get_y_here() + 2 + get_yl();

    al_set_target_bitmap(get_ground().get_al_bitmap());
    al_draw_filled_rectangle(x1, y1, x2, y2, get_undraw_color());
}

} // Api
