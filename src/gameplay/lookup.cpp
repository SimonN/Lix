/*
 * lookup.cpp
 *
 */

#include "lookup.h"

typedef Lookup::LoNr LoNr;

Lookup::Lookup(int nxl, int nyl, bool ntorus_x, bool ntorus_y)
:
    xl     (nxl > 1 ? nxl : 1),
    yl     (nyl > 1 ? nyl : 1),
    torus_x(ntorus_x),
    torus_y(ntorus_y),
    lt     (xl * yl, 0)
{
}



Lookup::~Lookup()
{
}



void Lookup::resize(int nxl, int nyl, bool ntx, bool nty)
{
    *this = Lookup(nxl, nyl, ntx, nty);
}



// These are private functions. They assume the input to be completely
// in valid range for faster execution.
int  Lookup::get_rectangle_at(int x, int y, int xr, int yr, LoNr n) const
{
    int count = 0;
    for  (int ix = x; ix < xr; ++ix)
     for (int iy = y; iy < yr; ++iy) if (get_at(ix, iy) & n) ++count;
    return count;
}



// Also a private function.
void Lookup::add_rectangle_at(int x, int y, int xr, int yr, LoNr n)
{
    for  (int ix = x; ix < xr; ++ix)
     for (int iy = y; iy < yr; ++iy) add_at(ix, iy, n);
}



LoNr Lookup::get(int x, int y) const
{
    amend(x, y);
    return get_at(x, y);
}



bool Lookup::get(int x, int y, LoNr n) const
{
    amend(x, y);
    return get_at(x, y) & n;
}



int Lookup::get_rectangle(int x, int y, int xr, int yr, LoNr n) const
{
    amend(x, y);
    const int xrr = x + xr <= xl ? xr : xl - x;
    const int yrr = y + yr <= yl ? yr : yl - y;
    int count =                       get_rectangle_at(x, y, xrr,    yrr,   n);
    if (torus_x && xr > xrr) count += get_rectangle_at(0, y, xr-xrr, yrr,   n);
    if (torus_y && yr > yrr) count += get_rectangle_at(0, y, xrr,    yr-yrr,n);
    if (torus_x && xr > xrr
     && torus_y && yr > yrr) count += get_rectangle_at(0, 0, xr-xrr, yr-yrr,n);
    return count;
}



bool Lookup::get_solid(int x, int y) const
{
    amend(x, y);
    return get_at(x, y) & bit_terrain;
}



bool Lookup::get_solid_even(int x, int y) const
{
    amend(x, y);
    return ( get_at(x&~1,y) | get_at(x|1,y) ) & bit_terrain;
}



// int Lookup::get_solid_rect_even(int, int, int, int) const;



bool Lookup::get_steel(int x, int y) const
{
    amend(x, y);
    return get_at(x, y) & bit_steel;
}



//    int  get_steel_rectangle(int, int, int, int) const;



void Lookup::rm(int x, int y, LoNr n)
{
    if (! amend_if_inside(x, y)) return;
    rm_at(x, y, n);
}



void Lookup::add(int x, int y, LoNr n)
{
    if (! amend_if_inside(x, y)) return;
    add_at(x, y, n);
}



void Lookup::add_rectangle(int x, int y, int xr, int yr, LoNr n)
{
    for  (int ix = 0; ix < xr; ++ix)
     for (int iy = 0; iy < yr; ++iy) add(x + ix, y + iy, n);
}



void Lookup::set_solid(int x, int y)
{
    if (! amend_if_inside(x, y)) return;
    add_at(x, y, bit_terrain);
}



void Lookup::set_solid_rectangle(int x, int y, int xr, int yr)
{
    add_rectangle(x, y, xr, yr, bit_terrain);
}



void Lookup::set_air(int x, int y)
{
    if (! amend_if_inside(x, y))  return;
    if (get_at(x, y) & bit_steel) return;
    rm_at(x, y, bit_terrain);
}



//    void Lookup::save_as_bitmap()
//    {
//        BITMAP* bmp = ::create_bitmap(xl, yl);
//        ::clear_to_color(bmp, 0);
//        for  (int x = 0; x < xl; ++x)
//         for (int y = 0; y < yl; ++y) {
//            const LoNr n = get_at(x, y);
//            ::putpixel(bmp, x, y, ::makecol(
//                (n & bit_trap)    *  70 + (n & bit_fire)  * 110
//                                        + (n & bit_water) *  35,
//                (n & bit_terrain) * 150 + (n & bit_steel) * 100,
//                (n & bit_goal)    * 100 + (n & bit_fling) * 150
//                ));
//        }
//        ::save_bmp("lookupmap.bmp", bmp, 0);
//    }

