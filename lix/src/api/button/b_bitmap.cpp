/*
 * button_b.cpp
 *
 */

#include "b_bitmap.h"

#include "../../graphic/gra_lib.h"
#include "../../other/globals.h"

namespace Api {

BitmapButton::BitmapButton(const Cutbit& c, int x, int y)
:
    Button (x, y, c.get_xl(), c.get_yl()),
    cutbit (&c),
    x_frame(0)
{
}

BitmapButton::~BitmapButton()
{
}



void BitmapButton::draw_self()
{
    int y_frame = get_on() && !get_down() ? 1 : 0; // Button::draw setzt down
    Button::draw_self();
    cutbit->draw(get_ground(), get_x_here(), get_y_here(), x_frame, y_frame);
}



Checkbox::Checkbox(const int x, const int y)
:
    BitmapButton(GraLib::get(gloB->file_bitmap_checkbox), x, y)
{
}

Checkbox::~Checkbox()
{
}

void Checkbox::set_checked(const bool b)
{
    set_x_frame(b);
    set_draw_required();
}

void Checkbox::toggle()
{
    set_x_frame(1 - get_x_frame());
    set_draw_required();
}

void Checkbox::calc_self()
{
    Button::calc_self();
    if (get_clicked()) {
        toggle();
    }
}

}
// Ende Namespace Api
