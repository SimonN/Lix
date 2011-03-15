/*
 * gameplay/player.cpp
 *
 */

#include "tribe.h"

Tribe::Tribe()
:
    lix_hatch   (0),
    lix_saved   (0),
    lix_out     (0),
    rate_min    (1),
    rate        (1),
    nuke        (false),
    update_hatch(0),
    hatch_next  (0),
    skill       (gloB->skill_max),
    skills_used (0),
    style       (LixEn::GARDEN)
{
}



Tribe::~Tribe()
{
    lixvec.clear();
}



void Tribe::return_skills(const LixEn::Ac ac, const int amount)
{
    for (int i = 0; i < gloB->skill_max; ++i)
     if (skill[i].ac == ac && skill[i].nr != LixEn::infinity) {
        skill[i].nr += amount;
        skills_used -= amount;
        break;
    }
}



std::string Tribe::get_name() const
{
    std::string ret;
    for (std::list <Master> ::const_iterator
     itr = masters.begin(); itr != masters.end(); ++itr) {
        ret += itr->name;
        if (itr != --masters.end()) ret += ", ";
    }
    return ret;
}
