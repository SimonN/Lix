/*
 * menu/bitmap.cpp
 *
 */

#include "preview.h"

#include "../graphic/gra_lib.h"
#include "../level/level.h"

namespace Api{

Preview::Preview(int x, int y, unsigned xl, unsigned yl)
:
    Frame(x, y, xl, yl),
    torbit(0),
    icon_status(GraLib::get(gloB->file_bitmap_preview_icon), get_ground()),
    icon_torus (GraLib::get(gloB->file_bitmap_preview_icon), get_ground()),
    status(Level::BAD_EMPTY)
{
    set_undraw_color(color[COL_API_M]);
    icon_torus.set_y_frame(1);
}

Preview::Preview(const Preview& p)
:
    Frame (p),
    torbit(0),
    icon_status(GraLib::get(gloB->file_bitmap_preview_icon), get_ground()),
    icon_torus (GraLib::get(gloB->file_bitmap_preview_icon), get_ground()),
    status(p.status)
{
    set_undraw_color(color[COL_API_M]);
    icon_torus.set_y_frame(1);
}

Preview::~Preview()
{
    if (torbit) delete torbit;
}

Preview& Preview::operator = (const Preview& p)
{
    if (this == &p) return *this;
    Frame::operator = (p);
    return *this;
}



void Preview::set_level(const Level& l)
{
    if (torbit) delete torbit;
    torbit = new Torbit(l.create_preview(get_xl(),
     get_yl(), color[COL_API_M]));

    status = l.get_status();
    icon_status.set_x_frame(status);
    icon_torus .set_x_frame(l.torus_x + 2 * l.torus_y);
    if (status == Level::BAD_EMPTY) icon_status.set_x_frame(0);

    set_draw_required();
}

void Preview::clear()
{
    if (torbit) {
        delete torbit;
        torbit = 0;
        icon_status.set_x_frame(0);
        icon_torus .set_x_frame(0);
    }
    set_draw_required();
}


void Preview::draw_self()
{
    Frame::draw_self();
    if (torbit) blit(torbit->get_al_bitmap(), get_ground().get_al_bitmap(),
                 0, 0, get_x(), get_y(), get_xl(), get_yl());
    else rectfill(get_ground().get_al_bitmap(), get_x(), get_y(),
          get_x() + get_xl() - 1, get_y() + get_yl() - 1,
          get_undraw_color());

    icon_torus.set_xy(get_x(), get_y());
    icon_torus.draw();

    icon_status.set_xy(get_x(), get_y());                       icon_status.draw();
    icon_status.set_y (get_y()+ get_yl()-icon_status.get_yl()); icon_status.draw();
    icon_status.set_x (get_x()+ get_xl()-icon_status.get_xl()); icon_status.draw();
    icon_status.set_y (get_y());                                icon_status.draw();
}

} // Ende Namensraum Api
