/*
 * goal.cpp
 *
 */

#include "goal.h"

#include "../gameplay/lookup.h"
#include "../graphic/gra_lib.h"
#include "../level/obj_lib.h"

Goal::Goal(Torbit& ground, const Object* ob, const int new_x, const int new_y)
:
    EdGraphic(ground, ob, new_x, new_y)
{
}

Goal::~Goal()
{
}



bool Goal::has_tribe(const Tribe* t) const
{
    for (TribeVec::const_iterator i = tribes.begin(); i != tribes.end(); ++i)
     if (*i == t) return true;
    return false;
}



void Goal::draw(bool draw_owners, bool draw_no_sign)
{
    EdGraphic::draw();
    unsigned offset = 0; // for drawing several players

    if (draw_no_sign) {
        const Cutbit& c = GraLib::get(gloB->file_bitmap_mouse);
        c.draw(get_ground(),
         get_x() + get_object()->get_trigger_x()
                 + get_object()->trigger_xl / 2
                 - c           . get_xl()   / 2,
         get_y() + get_object()->get_trigger_y()
                 + get_object()->trigger_yl / 2
                 - c           . get_yl(),
         2, 2); // (2,2) are the (xf,yf) of the international "no"-sign
    }
    else if (draw_owners)
     for (TribeVec::const_iterator i = tribes.begin(); i != tribes.end(); ++i){
        const Cutbit& c = GraLib::get_lix((*i)->style);
        c.draw(get_ground(),
         get_x() + get_object()->get_trigger_x() + get_object()->trigger_xl / 2
         - c.get_xl() / 2
         + (20 * offset++)
         - 10 * (tribes.size() - 1), // tribes vector only contains owners
         get_yl() > 70 ? get_y() : get_y() + get_yl() - 70, // y-pos
         0, LixEn::WALKER - 1);
    }
}
