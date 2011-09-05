/*
 * A lookup table for quick land/steel detection and backwards correspondence
 * of playfield positions to interactive objects.
 *
 * Right now, this doesn't store pointers to objects. Thus, it can be checked
 * whether there is fire or water or steel at a given position, but it cannot
 * tell what exact interactive object instance sits there. If this behavior
 * is desired, the class here should either be extended, or the object in
 * question should just be looked up manually in the list of objects of its
 * type.
 *
 */

#pragma once

#include <vector>

#include "../other/help.h"

class Lookup {

public:

    typedef short LoNr; // this is at least 2 bytes long

    static const LoNr bit_terrain    = 0x0001;
    static const LoNr bit_steel      = 0x0002;
    static const LoNr bit_ow_left    = 0x0004; // non-implem. one-way walls
    static const LoNr bit_ow_right   = 0x0008; // same

    static const LoNr bit_goal       = 0x0010;
    static const LoNr bit_goal_prox  = 0x0020; // Near goal, can't block here
    static const LoNr bit_fire       = 0x0040;
    static const LoNr bit_water      = 0x0080;

    static const LoNr bit_trap       = 0x0100;
    static const LoNr bit_fling      = 0x0200;
    static const LoNr bit_trampoline = 0x0400;

    //     xl,  yl,  tor_x tor_y
    Lookup(int = 1, int = 1, bool = false, bool = false);
    ~Lookup();

    inline int  get_xl()      const { return xl; }
    inline int  get_yl()      const { return yl; }

    inline bool get_torus_x() const { return torus_x; }
    inline bool get_torus_y() const { return torus_y; }

    void resize             (int, int, bool, bool);

    LoNr get                (int, int)                 const;
    bool get                (int, int, LoNr)           const;
    int  get_rectangle      (int, int, int, int, LoNr) const;
    bool get_solid          (int, int)                 const;
    bool get_solid_even     (int, int)                 const;
    int  get_solid_rect_even(int, int, int, int)       const;
    bool get_steel          (int, int)                 const;
    int  get_steel_rectangle(int, int, int, int)       const;

    //                       x,   y,   xl,  yl,  bit &
    void rm                 (int, int,           LoNr);
    void add                (int, int,           LoNr);
    void add_rectangle      (int, int, int, int, LoNr);
    void set_solid          (int, int);
    void set_solid_rectangle(int, int, int, int);
    void set_air            (int, int);
    void set_air_rectangle  (int, int, int, int);

private:

    int xl;
    int yl;

    bool torus_x;
    bool torus_y;

    std::vector <LoNr> lt; // lookup table, row, row, row, row, ...

    inline const LoNr& get_at(int x, int y) const   { return lt[y * xl + x]; }
    inline void        add_at(int x, int y, LoNr n) { lt[y * xl + x] |= n;   }
    inline void        rm_at (int x, int y, LoNr n) { lt[y * xl + x] &= ~n;  }

    int  get_rectangle_at(int, int, int, int, LoNr) const;
    void add_rectangle_at(int, int, int, int, LoNr);

    // move coordinates onto nontorus, or modulo them on torus.
    inline void amend(int& x, int& y) const
    {
        x = torus_x ? Help::mod(x, xl)
          : x >= xl ? xl - 1
          : x <  0  ? 0 : x;
        y = torus_y ? Help::mod(y, yl)
          : y >= yl ? yl - 1
          : y <  0  ? 0 : y;
    }

    // Is the given point on the map?
    // This shall only be used in set/add functions, not in readers.
    // On tori, this is the same as amend, and it returns true.
    // On non-tori, if pixel outide, returns false, otherwise true.
    inline bool amend_if_inside(int& x, int& y) const
    {
        if (! torus_x && (x < 0 || x >= xl)) return false;
        if (! torus_y && (y < 0 || y >= yl)) return false;
        amend(x, y);
        return true;
    }

};
