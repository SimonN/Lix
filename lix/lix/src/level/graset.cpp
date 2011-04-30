/*
 * level/graset.cpp
 *
 */

#include "graset.h"

const Object* GraphicSet::get_terrain(int ter_id) const
{
    if (ter_id < 0 || ter_id >= (int) terrain.size()) return 0;
    else return &terrain[ter_id];
}



const Object* GraphicSet::get_special(int spe_id) const
{
    if (spe_id < 0 || spe_id >= (int) special.size()) return 0;
    else return &special[spe_id];
}



const int GraphicSet::get_terrain_id(const Object* ob) const
{
    if (!ob) return -1;
    for (int id = 0; id < (int) terrain.size(); ++id)
     if (ob == &terrain[id]) return id;
    return -1;
}



const int GraphicSet::get_special_id(const Object* ob) const
{
    if (!ob) return -1;
    for (int id = 0; id < (int) special.size(); ++id)
     if (ob == &special[id]) return id;
    return -1;
}
