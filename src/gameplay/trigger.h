/*
 * gameplay/trigger.h
 *
 * Triggered EdGraphics
 *
 * These come as semi-permanent and nonpermanent.
 *
 * Nonpermanent traps in frame 0 accept up to one lix per tribe, and work
 * on them (kill them, or fling them). They subsequently cycle through their
 * animation once. While they animate, they do not take any more lixes.
 *
 * Semi-permanent traps accept lixes all the time, but cycle through the
 * animation only when they're actually used.
 *
 */

#pragma once

#include <set>

#include "../editor/graph_ed.h"

struct Tribe;

class Triggerable : public EdGraphic {

private:

    bool nonperm;
    bool semiperm;
    bool start_anim;

    std::set <const Tribe*> tribes;

public:

             Triggerable(Torbit&, const Object*, const int = 0, const int = 0);
    virtual ~Triggerable();

    inline  bool get_nonperm()  const { return nonperm;  }
    inline  bool get_semiperm() const { return semiperm; }

            void allow_animation();
    virtual void animate(); // this will also clear tribes

            bool get_open_for(const Tribe&) const;
    inline  void add_tribe(const Tribe& t) { tribes.insert(&t); }

};
