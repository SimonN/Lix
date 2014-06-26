/*
 * lix/ac_func.cpp
 *
 * Grundlage die Funktionen, die pro Faehigkeit aufgerufen werden,
 * Festlegung der Standardfunktionen
 *
 */

#include "lix.h"

#include "../gameplay/tribe.h"

Lixxie::AcFunc::AcFunc()
:
    pass_top (false),
    leaving  (false),
    blockable(true),
    aiming   (false),
    sound_assign(Sound::ASSIGN),
    sound_become(Sound::NOTHING),
    sound_aim   (Sound::NOTHING),
    assclk(0),
    become(0),
    update(0)
{
}



Lixxie::AcFunc::~AcFunc()
{
}



void Lixxie::assclk(const LixEn::Ac new_ac)
{
    const LixEn::Ac old_ac = ac;
    if (ac_func[new_ac].assclk) ac_func[new_ac].assclk(*this);
    else                        become(new_ac);
    if (old_ac != ac) --frame;
}



void Lixxie::become(const LixEn::Ac new_ac)
{
    if (new_ac != ac && queue > 0) {
        tribe->return_skills(ac, queue);
        queue = 0; // in case other skill_become() redirect again to become()
    }
    // Reset sprite placement like climber's offset in x-direction by 1,
    // or the digger sprite displacement in one frame. This is the same code
    // as the sprite placement in set_ex/ey().
    set_x(ex - LixEn::ex_offset);
    set_y(ey - LixEn::ey_offset);

    if (ac_func[new_ac].become) ac_func[new_ac].become(*this);
    else                        become_default(new_ac);
}



void Lixxie::become_default(const LixEn::Ac new_ac)
{
    frame     = 0;
    special_y = 0;
    special_x = 0;
    queue     = 0;
    ac        = new_ac;
}



void Lixxie::update(const UpdateArgs& ua)
{
    if (ac_func[ac].update) ac_func[ac].update(*this, ua);
}
