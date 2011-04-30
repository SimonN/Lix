/*
 * lemming/ac_func.cpp
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
    nukable  (true),
    aiming   (false),
    returns_x(0),
    aim_sound(Sound::NOTHING),
    assign(0),
    update(0)
{
}



Lixxie::AcFunc::~AcFunc()
{
}



void Lixxie::assign(const LixEn::Ac new_ac)
{
    if (new_ac != ac && ac_func[ac].returns_x > 0) {
        tribe->return_skills(ac, special_x / ac_func[ac].returns_x);
    }
    if (ac_func[new_ac].assign) {
        ac_func[new_ac].assign(*this);
    }
    else assign_default(new_ac);
}



void Lixxie::update(const UpdateArgs& ua)
{
    if (ac_func[ac].update) ac_func[ac].update(*this, ua);
}



void Lixxie::assign_default(const LixEn::Ac new_ac)
{
    frame     = 0;
    special_y = 0;
    special_x = 0;
    ac        = new_ac;
}
