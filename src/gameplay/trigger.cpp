#include "trigger.h"

Triggerable::Triggerable(
    Torbit& t,
    const Object* o,
    const int x,
    const int y
) :
    EdGraphic(t, o, x, y),
    nonperm (false),
    semiperm(false),
    start_anim(false)
{
    switch (o->type) {
        case Object::TRAP:
            nonperm = true;
            break;
        case Object::FLING:
            nonperm = (o->subtype & 2);
            break;
        case Object::TRAMPOLINE:
            semiperm = true;
            break;
        default:
            break;
        }
}



Triggerable::~Triggerable()
{
}



void Triggerable::allow_animation()
{
    start_anim = true;
}


void Triggerable::animate()
{
    if (! (nonperm || semiperm) || get_x_frame() != 0 || start_anim) {
        start_anim = false;
        set_x_frame(get_x_frame() + 1);
        if (get_x_frame() >= get_x_frames()) set_x_frame(0);
    }
    tribes.clear();
}



bool Triggerable::get_open_for(const Tribe& t) const
{
    if (nonperm)
        return get_x_frame() == 0 && tribes.find(&t) == tribes.end();
    else
        return true;
}
