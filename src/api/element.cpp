#include "element.h"
#include "manager.h"

namespace Api {

Element::Element(int nx, int ny, int nxl, int nyl)
:
    ground(&Manager::get_torbit()),
    x     (nx),
    y     (ny),
    xl    (nxl),
    yl    (nyl),
    hidden(false),
    drawn (false),
    draw_required(true),
    undraw_color(0),
    parent(0)
{
}



Element::~Element()
{
    if (parent) parent->children.erase(this);
    Manager::remove_focus(this);
    Manager::remove_elder(this);
}



int Element::get_x_here() const
{
    int here_x = get_x();
    for (Element* p = parent; p != 0; p = p->parent) here_x += p->get_x();
    return here_x;
}

int Element::get_y_here() const
{
    int here_y = get_y();
    for (Element* p = parent; p != 0; p = p->parent) here_y += p->get_y();
    return here_y;
}



void Element::set_x(const int i)
{
    x = i;
    draw_required = true;
}

void Element::set_y(const int i)
{
    y = i;
    draw_required = true;
}



void Element::set_xl(const unsigned i)
{
    xl = i;
    draw_required = true;
}
void Element::set_yl(const unsigned i)
{
    yl = i;
    draw_required = true;
}



void Element::set_hidden(const bool b)
{
    if (b) Manager::remove_focus(this);
    if (!b && hidden) set_draw_required();
    hidden = b;
}



void Element::set_draw_required(const bool b)
{
    draw_required = b;
    for (ElIt itr = children.begin(); itr != children.end(); ++itr)
     (**itr).set_draw_required(b);
}



bool Element::is_mouse_here() const
{
    if (hardware.get_mx() >= get_x_here()
     && hardware.get_mx() <  get_x_here() + xl
     && hardware.get_my() >= get_y_here()
     && hardware.get_my() <  get_y_here() + yl) return true;
    else return false;
}



void Element::add_child(Element& e)
{
    // No multiple parents
    if (e.parent) return;
    e.parent = this;
    children.insert(&e);
}



void Element::remove_child(Element& e)
{
    // Don't orphanize other people's children
    if (e.parent != this) return;
    e.parent = 0;
    children.erase(&e);
}



void Element::clear_children()
{
    for (ElIt itr = children.begin(); itr != children.end(); ++itr)
     remove_child(**itr);
}



void Element::hide_all_children(const bool b)
{
    for (ElIt itr = children.begin(); itr != children.end(); ++itr)
     (**itr).set_hidden(b);
}



void Element::calc()
{
    if (!hidden)
     for (ElIt itr = children.begin(); itr != children.end(); ++itr) {
        (**itr).calc();
    }
    calc_self();
}



void Element::work()
{
    if (!hidden)
     for (ElIt itr = children.begin(); itr != children.end(); ++itr) {
        (**itr).work();
    }
    work_self();
}



void Element::draw() {
    if (!hidden) {
        if (draw_required) {
            draw_required = false;
            draw_self();
            drawn = true;
        }
        // In the options menu, all stuff has to be undrawn first, then
        // drawn, so that rectangles don't overwrite proper things.
        for (ElIt itr = children.begin(); itr != children.end(); ++itr)
         if ((**itr).get_hidden()) (**itr).draw();
        for (ElIt itr = children.begin(); itr != children.end(); ++itr)
         if (!(**itr).get_hidden()) (**itr).draw();
    }
    // hidden
    else if (drawn) {
        draw_required = false;
        undraw_self();
        drawn = false;
    }
}



void Element::undraw_self() {
    if (drawn && undraw_color) {
        rectfill(ground->get_al_bitmap(),
         get_x_here(), get_y_here(),
         get_x_here() + xl - 1, get_y_here() + yl - 1, undraw_color);
    }
}

} // Api
