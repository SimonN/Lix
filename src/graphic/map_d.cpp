/*
 * gameplay/map_d.cpp
 *
 * Drawing routines of the Map class
 *
 */

#include "map.h"
#include "../graphic/glob_gfx.h"
#include "../other/user.h"
#include "../other/help.h" // torus modulo

void Map::draw(Torbit& target)
{
    const bool z = zoom;
    int less_x = 0;
    int less_y = 0; // how much is the screen larger than the map?
    if (!get_torus_x() && get_xl() * (z+1) < screen_xl)
     less_x = screen_xl - get_xl() * (z+1);
    if (!get_torus_y() && get_yl() * (z+1) < screen_yl)
     less_y = screen_yl - get_yl() * (z+1);

    for     (int x = less_x/2; x < screen_xl-less_x/2; x += get_xl()*(z+1)) {
        for (int y = less_y;   y < screen_yl;          y += get_yl()*(z+1)) {
            draw_at(target, x, y);
            if (less_y != 0) break;
        }
        if (less_x != 0) break;
    }

    // Draw the screen border
    const int c = useR->screen_border_colored
                ? color[COL_SCREEN_BORDER] : 0;
    if (less_x) {
        rectfill(target.get_al_bitmap(), 0, 0, less_x/2 - 1,
         screen_yl - 1, c);
        rectfill(target.get_al_bitmap(), screen_xl - less_x/2, 0,
         screen_xl - 1, screen_yl - 1, c);
    }
    if (less_y)
     rectfill(target.get_al_bitmap(), less_x/2, 0,
     screen_xl - less_x/2 - 1, less_y - 1, c);
}



static inline void blit_double(
    BITMAP* src,
    BITMAP* dest,
    int     sx, // source x, y
    int     sy,
    int     sxl, // length on the source
    int     syl,
    int     tx, // start of the target
    int     ty
) {
    // The safety things are commented out for speed. Since I fixed the torus
    // drawing on 2011-09-13, we're called with safe parameters already.
    //    // The +1 or -1 are just to round towards safety when dividing.
    //    if (!src || !dest) return;
    //    if (sx < 0) { sxl += sx; tx -= 2 * sx; sx = 0; }
    //    if (sy < 0) { syl += sy; ty -= 2 * sy; sy = 0; }
    //    if (tx < 0) { sxl += (tx - 1)/2; sx -= (tx - 1)/2; tx = 0; }
    //    if (ty < 0) { syl += (ty - 1)/2; sy -= (ty - 1)/2; ty = 0; }
    //    if (sx >= src->w  || sy >= src->h ) return;
    //    if (tx >= dest->w || sy >= dest->h) return;
    //    if (sx + sxl > src->w) sxl = src->w - sx;
    //    if (sy + syl > src->h) syl = src->h - sy;
    //    if (tx + 2 * sxl > dest->w) sxl = (dest->w - tx + 1) / 2;
    //    if (ty + 2 * syl > dest->w) syl = (dest->h - ty + 1) / 2;
    //    if (sxl <= 0 || syl <= 0) return;
    masked_stretch_blit(src, dest, sx, sy, sxl, syl, tx, ty, 2*sxl, 2*syl);
}



void Map::draw_at(
    Torbit&   target,
    const int offx, // start this much away from the screen's upper left edge
    const int offy
) {
    const int& mapxl = get_xl();
    const int& mapyl = get_yl();
    const int  scrx = !zoom ? screen_x: Help::mod(screen_x+screen_xl/4, mapxl);
    const int  scry = !zoom ? screen_y: Help::mod(screen_y+screen_yl/4, mapyl);
    const int  lx = std::min(!zoom ? screen_xl : screen_xl / 2, mapxl);
    const int  ly = std::min(!zoom ? screen_yl : screen_yl / 2, mapyl);
    const bool short_x = scrx + lx > mapxl;
    const bool short_y = scry + ly > mapyl;
    const int  shoby_x = short_x ? scrx + lx - mapxl : 0;
    const int  shoby_y = short_y ? scry + ly - mapyl : 0;

    if (! zoom) {
        masked_blit(get_al_bitmap(), target.get_al_bitmap(),
         scrx, scry, // source x/y start
         offx, offy, // target x/y start
         short_x ? mapxl - scrx : lx, // x-length and y-length of area to copy
         short_y ? mapyl - scry : ly);
        if (short_x && get_torus_x()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, scry,
             offx + mapxl - scrx,
             offy,
             scrx + lx - mapxl,
             short_y ? mapyl - scry : ly);
        }
        if (short_y && get_torus_y()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             scrx, 0,
             offx,
             offy + mapyl - scry,
             short_x ? mapxl - scrx : lx,
             scry + ly - mapyl);
        }
        if (short_x && short_y && get_torus_x() && get_torus_y()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, 0,
             offx + mapxl - scrx,
             offy + mapyl - scry,
             scrx + lx - mapxl,
             scry + ly - mapyl);
        }
    }
    else {
        // This is the above code copied, but with zoom things hacked in
        blit_double(get_al_bitmap(), target.get_al_bitmap(),
         scrx, scry, // source x/y start
         short_x ? mapxl - scrx : lx, // x-length and y-length of the source
         short_y ? mapyl - scry : ly,
         offx, // target x/y start
         offy);
        if (short_x && get_torus_x()) {
            blit_double(get_al_bitmap(), target.get_al_bitmap(),
             0, scry,
             shoby_x,
             short_y ? mapyl - scry : ly,
             offx + 2 * (mapxl - scrx),
             offy);
        }
        if (short_y && get_torus_y()) {
            blit_double(get_al_bitmap(), target.get_al_bitmap(),
             scrx, 0,
             short_x ? mapxl - scrx : lx,
             shoby_y,
             offx,
             offy + 2 * (mapyl - scry));
        }
        if (short_x && short_y && get_torus_x() && get_torus_y()) {
            blit_double(get_al_bitmap(), target.get_al_bitmap(),
             0, 0,
             shoby_x,
             shoby_y,
             offx + 2 * (mapxl - scrx),
             offy + 2 * (mapyl - scry));
        }
    }
    // end if/else zoom
}



void Map::load_masked_screen_rectangle(Torbit& src)
{
    if (get_xl() != src.get_xl() || get_yl() != src.get_yl()) return;
    const int&  x  = screen_x;
    const int&  y  = screen_y;
    const int&  xl = get_xl();
    const int&  yl = get_yl();
    const bool& tx = get_torus_x();
    const bool& ty = get_torus_y();
    const int min_w = screen_xl;
    const int min_h = screen_yl;
    BITMAP* s = src.get_al_bitmap();
    BITMAP* b =     get_al_bitmap();
                masked_blit(s, b, x,    y,    x,    y,    min_w, min_h);
    if (tx)     masked_blit(s, b, x-xl, y,    x-xl, y,    min_w, min_h);
    if (ty)     masked_blit(s, b, x,    y-yl, x,    y-yl, min_w, min_h);
    if (tx&&ty) masked_blit(s, b, x-xl, y-yl, x-xl, y-yl, min_w, min_h);
}



void Map::clear_screen_rectangle(int col)
{
    const int&  x1 = screen_x;
    const int&  y1 = screen_y;
    const int&  x2 = get_xl();
    const int&  y2 = get_yl();
    const bool& tx = get_torus_x();
    const bool& ty = get_torus_y();
    BITMAP* b = get_al_bitmap();
                rectfill(b, x1, y1, x2, y2, col);
    if (tx)     rectfill(b, 0,  y1, x1, y2, col);
    if (ty)     rectfill(b, x1, 0,  x2, y1, col);
    if (tx&&ty) rectfill(b, 0,  0,  x1, y1, col);
}
