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



void Goal::draw(bool owners)
{
    EdGraphic::draw();
    unsigned offset = 0; // for drawing several players
    if (owners)
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



void Goal::draw_lookup(Lookup& lk)
{
    const Object* ob = get_object();
    if (! ob) return;
    // Draw the area in which blockers aren't allowed
    lk.add_rectangle(get_x() + ob->get_trigger_x() - ob->trigger_xl,
                     get_y() + ob->get_trigger_y() - ob->trigger_yl,
                     3 * ob->trigger_xl,
                     3 * ob->trigger_yl, Lookup::bit_goal_prox);
    // Also draw the actual
    EdGraphic::draw_lookup(lk);
}
