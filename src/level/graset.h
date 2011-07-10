/*
 * level/graset.h
 *
 * A base class from wich L1's and L2's graphic sets get derived.
 *
 */

#pragma once

#include <vector>

#include "object.h"

class GraphicSet {

public:

    const Object* get_terrain   (int)           const;
    const Object* get_special   (int)           const;
          int     get_terrain_id(const Object*) const;
          int     get_special_id(const Object*) const;

    // These are useful while creating a graphics set. ObjLib should
    // provide no or just const access to a graphics set after it's completed.
    inline void push_back_terrain(const Object& ob) { terrain.push_back(ob); }
    inline void push_back_special(const Object& ob) { special.push_back(ob); }

    inline virtual ~GraphicSet() {}

private:

    std::vector <Object> terrain;
    std::vector <Object> special;

};
