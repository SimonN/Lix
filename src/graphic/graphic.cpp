/*
 * graphic/graphic.cpp
 *
 */

#include <cmath> // fmod(): Modular arithmetic for floats

#include "graphic.h"

#include "../other/help.h"

Graphic::Graphic(
    const Cutbit& c,
    Torbit&       gr,
    const int     new_x,
    const int     new_y)
:
    cutbit  (&c),
    ground  (gr),

    x       (new_x),
    y       (new_y),
    mirror  (false),
    rotation(0),
    mode    (Cutbit::NORMAL),

    x_frame (0),
    y_frame (0)
{
}



Graphic::~Graphic()
{
}



Graphic& Graphic::operator = (const Graphic& g)
{
    if (this == &g || &ground != &g.ground) return *this;
    cutbit   = g.cutbit,
    x        = g.x;
    y        = g.y;
    mirror   = g.mirror;
    rotation = g.rotation;
    mode     = g.mode;

    x_frame  = g.x_frame;
    y_frame  = g.y_frame;

    return *this;
}



void Graphic::set_x(const int i)
{
    x = i;
    if (ground.get_torus_x()) x = Help::mod(x, ground.get_xl());
}
void Graphic::set_y(const int i)
{
    y = i;
    if (ground.get_torus_y()) y = Help::mod(y, ground.get_yl());
}



void Graphic::set_rotation(const double dbl)
{
    rotation = fmod(dbl, 4);
}



int Graphic::get_xl() const
{
    return (rotation == 0 || rotation == 2)
     ? cutbit->get_xl() : cutbit->get_yl();
}
int Graphic::get_yl() const
{
    return (rotation == 0 || rotation == 2)
     ? cutbit->get_yl() : cutbit->get_xl();
}



bool Graphic::is_last_frame() const
{
    return x_frame == get_x_frames() - 1
     || cutbit->get_pixel(x_frame + 1, y_frame, 0, 0)
     == _getpixel16(cutbit->get_al_bitmap(), 0, 0);
}



bool Graphic::get_frame_exists(const int xf, const int yf) const
{
    return xf < get_x_frames() && yf < get_y_frames()
     && cutbit->get_pixel(xf, yf, 0, 0)
     != _getpixel16(cutbit->get_al_bitmap(), 0, 0);
}



int Graphic::get_pixel(int x, int y) const
{
    const int xl = cutbit->get_xl();
    const int yl = cutbit->get_yl();
    int    use_x = x;
    int    use_y = y;

    // If the rotation is a multiple of a quarter turn, rotate the values
    // with the Graphic object. If the rotation is a fraction, return
    // the value from the original bitmap (treated as unrotated).
    // Lix terrain can only be rotated in quarter turns.
    int rotation_integer = (int) rotation;
    if (rotation_integer - rotation != 0) rotation_integer = 0;

    switch (rotation_integer) {
        case 0: use_x = x;      use_y = !mirror ? y      : yl-y-1; break;
        case 1: use_x = y;      use_y = !mirror ? yl-x-1 : x;      break;
        case 2: use_x = xl-x-1; use_y = !mirror ? yl-y-1 : y;      break;
        case 3: use_x = xl-y-1; use_y = !mirror ? x      : yl-x-1; break;
    }
    return cutbit->get_pixel(x_frame, y_frame, use_x, use_y);
}



void Graphic::draw()
{
    cutbit->draw(ground, x, y, x_frame, y_frame, mirror, rotation, 0, mode);
}
