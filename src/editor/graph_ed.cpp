/*
 * editor/graph_ed.cpp
 *
 * Hatches are always drawn with a rotation of 0. Instead, rotation means
 * if lixes should walk to the left (!= 0) instead of the right (== 0).
 * Thus, some functions must override those in Graphic:: to implement this.
 *
 */

#include "graph_ed.h"

#include "../gameplay/lookup.h"
#include "../graphic/gra_lib.h"
#include "../other/globals.h"
#include "../other/help.h" // proper modulo via Help::mod

EdGraphic::EdGraphic(Torbit& tb, const Object* o, int x, int y)
:
    Graphic  (o->cb, tb, x, y),
    object   (o),
    draw_info(false)
{
}



EdGraphic::~EdGraphic()
{
}



void EdGraphic::animate()
{
    if (is_last_frame()) {
        if (object->type != Object::HATCH) set_x_frame(0);
        // do nothing for hatches here.
    }
    else set_x_frame(get_x_frame() + 1);
    // Traps won't be animated all the time by Gameplay, so we don't have
    // to check for anything here.
}



int EdGraphic::get_xl() const
{
    // see top comment
    if (object->type == Object::HATCH) return get_cutbit()->get_xl();
    else                               return Graphic::     get_xl();
}
int EdGraphic::get_yl() const
{
    // see top comment
    if (object->type == Object::HATCH) return get_cutbit()->get_yl();
    else                               return Graphic::     get_yl();
}



int EdGraphic::get_selbox_x() const
{
    if (object->type == Object::HATCH) return object->selbox_x;
    int edge = (int) get_rotation();
    if (get_mirror()) edge = edge == 1 ? 3 : edge == 3 ? 1 : edge;
    switch (edge) {
    case 0: return object->selbox_x; // rotation is clockwise
    case 1: return object->cb.get_yl() - object->selbox_y - object->selbox_yl;
    case 2: return object->cb.get_xl() - object->selbox_x - object->selbox_xl;
    case 3: return object->selbox_y;
    }
    return object->selbox_x; // against compiler warning
}

int EdGraphic::get_selbox_y() const
{
    if (object->type == Object::HATCH) return object->selbox_y;
    int edge = (int) get_rotation();
    if (get_mirror()) edge = edge == 0 ? 2 : edge == 2 ? 0 : edge;
    switch (edge) {
    case 0: return object->selbox_y;
    case 1: return object->selbox_x;
    case 2: return object->cb.get_yl() - object->selbox_y - object->selbox_yl;
    case 3: return object->cb.get_xl() - object->selbox_x - object->selbox_xl;
    }
    return object->selbox_y; // against compiler warning
}

int EdGraphic::get_selbox_xl() const
{
    if (object->type == Object::HATCH) return object->selbox_xl;
    if ((int) get_rotation() % 2 == 1) return object->selbox_yl;
    else                               return object->selbox_xl;
}

int EdGraphic::get_selbox_yl() const
{
    if (object->type == Object::HATCH) return object->selbox_yl;
    if ((int) get_rotation() % 2 == 1) return object->selbox_xl;
    else                               return object->selbox_yl;
}



int EdGraphic::get_pixel(int x, int y) const
{
    // see top comment
    if (object->type == Object::HATCH) {
        const int use_y = !get_mirror() ? y : get_yl() - y;
        return get_cutbit()->get_pixel(get_x_frame(), get_y_frame(), x, use_y);
    }
    else return Graphic::get_pixel(x, y);
}



void EdGraphic::draw()
{
    // see top comment
    if (object->type == Object::HATCH && get_rotation()) {
        set_rotation(false);
        Graphic::draw();
        set_rotation(true);
    }
    else Graphic::draw();

    if (draw_info) {
        if (object->type == Object::HATCH) {
            // draw arrow pointing into the hatch's direction
            static const Cutbit& cb
             = GraLib::get(gloB->file_bitmap_edit_hatch);
            cb.draw(get_ground(), get_x() + get_xl()/2 - cb.get_xl()/2,
                                  get_y() + ::text_height(font_med),
                                  get_rotation() ? 1 : 0, 0);
        }
    }
}



void EdGraphic::draw_with_trigger_area()
{
    // do the regular drawing
    draw();
    // now draw trigger area on top
    if (object->type == Object::GOAL
     || object->type == Object::HATCH
     || object->type == Object::TRAP
     || object->type == Object::WATER
     || object->type == Object::FLING) {
        get_ground().draw_rectangle(
            get_x() + object->get_trigger_x(),
            get_y() + object->get_trigger_y(),
            object->trigger_xl,
            object->trigger_yl,
            makecol(0x40, 0xFF, 0xFF)
        );
    }
}



void EdGraphic::draw_lookup(Lookup& lk)
{
    if (!object) return;
    Lookup::LoNr nr = 0;
    switch (object->type) {
        case Object::GOAL:  nr = Lookup::bit_goal; break;
        case Object::TRAP:  nr = Lookup::bit_trap; break;
        case Object::WATER: nr = object->subtype == 0
                               ? Lookup::bit_water
                               : Lookup::bit_fire; break;
        case Object::FLING: nr = Lookup::bit_fling; break;
        default: break;
    }
    lk.add_rectangle(get_x() + object->get_trigger_x(),
                     get_y() + object->get_trigger_y(),
                     object->trigger_xl,
                     object->trigger_yl, nr);
}

