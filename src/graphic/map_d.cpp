/*
 * gameplay/map_d.cpp
 *
 * Drawing routines of the Map class
 *
 */

#include "map.h"
#include "../other/user.h"
#include "../graphic/glob_gfx.h"

void Map::draw(Torbit& target)
{
    const bool z = zoom;
    int less_x = 0;
    int less_y = 0; // how much is the screen larger than the map?
    if (!get_torus_x() && get_xl() * (z+1) < screen_xl)
     less_x = screen_xl - get_xl() * (z+1);
    if (!get_torus_y() && get_yl() * (z+1) < screen_yl)
     less_y = screen_yl - get_yl() * (z+1);

    for     (int x = less_x/2; x < screen_xl; x += get_xl()) {
        for (int y = less_y;   y < screen_yl; y += get_yl()) {
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



void Map::draw_at(
    Torbit&   target,
    const int offx, // start this much away from the screen's upper left edge
    const int offy
) {
    const bool z = zoom;
    const int& mapx = get_xl();
    const int& mapy = get_yl();
    const int  scrx = !z ? screen_x : screen_x + screen_xl/4;
    const int  scry = !z ? screen_y : screen_y + screen_yl/4;
    const int  lx = !z ? screen_xl : screen_xl / 2;
    const int  ly = !z ? screen_yl : screen_yl / 2;
    const bool short_x = scrx + lx > mapx;
    const bool short_y = scry + ly > mapy;

    if (! zoom) {
        masked_blit(get_al_bitmap(), target.get_al_bitmap(),
         scrx, scry, // source x/y start
         offx, offy, // target x/y start
         short_x ? mapx - scrx : lx, // x-length and y-length of area to copy
         short_y ? mapy - scry : ly);
        if (short_x && get_torus_x()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, scry,
             offx + mapx - scrx,
             offy,
             scrx + lx - mapx,
             short_y ? mapy - scry : ly);
        }
        if (short_y && get_torus_y()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             scrx, 0,
             offx,
             offy + mapy - scry,
             short_x ? mapx - scrx : lx,
             scry + ly - mapy);
        }
        if (short_x && short_y && get_torus_x() && get_torus_y()) {
            masked_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, 0,
             offx + mapx - scrx,
             offy + mapy - scry,
             scrx + lx - mapx,
             scry + ly - mapy);
        }
    }
    else {
        // This is the above code copied, but with zoom things hacked in
        masked_stretch_blit(get_al_bitmap(), target.get_al_bitmap(),
         scrx, scry, // source x/y start
         short_x ? mapx - scrx : lx, // x-length and y-length of the source
         short_y ? mapy - scry : ly,
         offx, // target x/y start
         offy,
         short_x ? 2 * (mapx - scrx) : 2 * lx, // x/y-length of the target,
         short_y ? 2 * (mapy - scry) : 2 * ly); // exactly 2 * source
        if (short_x && get_torus_x()) {
            masked_stretch_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, scry,
             scrx + lx - mapx,
             short_y ? mapy - scry : ly,
             offx + 2 * (mapx - scrx),
             offy,
             2 * (scrx + lx - mapx),
             short_y ? 2 * (mapy - scry) : 2 * ly);
        }
        if (short_y && get_torus_y()) {
            masked_stretch_blit(get_al_bitmap(), target.get_al_bitmap(),
             scrx, 0,
             short_x ? mapx - scrx : lx,
             scry + ly - mapy,
             offx,
             offy + 2 * (mapy - scry),
             short_x ? 2 * (mapx - scrx) : 2 * lx,
             2 * (scry + ly - mapy));
        }
        if (short_x && short_y && get_torus_x() && get_torus_y()) {
            masked_stretch_blit(get_al_bitmap(), target.get_al_bitmap(),
             0, 0,
             scrx + lx - mapx,
             scry + ly - mapy,
             offx + 2 * (mapx - scrx),
             offy + 2 * (mapy - scry),
             2 * (scrx + lx - mapx),
             2 * (scry + ly - mapy));
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
