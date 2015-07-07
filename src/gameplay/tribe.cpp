/*
 * gameplay/player.cpp
 *
 */

#include "tribe.h"
#include "../other/user.h"

Tribe::Tribe()
:
    lix_hatch     (0),
    lix_saved     (0),
    lix_saved_late(0),
    lix_out       (0),
    lix_exiting   (0),
    spawnint_slow (32),
    spawnint_fast ( 4),
    spawnint      (32),
    nuke          (false),
    update_hatch  (0),
    update_saved  (0),
    hatch_next    (0),
    skills_used   (0),
    style         (LixEn::GARDEN)
{
}



Tribe::~Tribe()
{
    lixvec.clear();
}



void Tribe::return_skills(const LixEn::Ac ac, const int amount)
{
    if (skills.find(ac) == skills.end())
        return;
    skills[ac]  += amount;
    skills_used -= amount;
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
