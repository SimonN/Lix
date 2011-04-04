/*
 * graphic/torbit.cpp
 *
 */

#include <cmath> // fmod fuers Drehen

#include "torbit.h"
#include "../other/help.h" // Help::mod, Help::hypot

Torbit::Torbit(int xl, int yl, bool tx, bool ty)
:
    torus_x(tx),
    torus_y(ty)
{
    if (xl < 1) xl = 1;
    if (yl < 1) yl = 1;
    bitmap = al_create_bitmap(xl, yl);
}



Torbit::Torbit(const Torbit& orig)
:
    torus_x(orig.torus_x),
    torus_y(orig.torus_y)
{
    bitmap = al_create_bitmap(orig.get_xl(), orig.get_yl());
    al_set_target_bitmap(bitmap);
    al_draw_bitmap(orig.bitmap, 0, 0, 0);
}



Torbit::~Torbit()
{
    if (bitmap) {
        al_destroy_bitmap(bitmap);
        bitmap = 0;
    }
}



Torbit& Torbit::operator = (const Torbit& orig)
{
    if (this == &orig) return *this;

    torus_x = orig.torus_x;
    torus_y = orig.torus_y;
    resize(orig.get_xl(), orig.get_yl());
    al_set_target_bitmap(bitmap);
    al_draw_bitmap(orig.bitmap, 0, 0, 0);
    return *this;
}



int Torbit::get_xl() const {
     return bitmap ? al_get_bitmap_width(bitmap) : 0;
}



int Torbit::get_yl() const {
     return bitmap ? al_get_bitmap_height(bitmap) : 0;
}



void Torbit::resize(int xl, int yl)
{
    if (xl < 1) xl = 1;
    if (yl < 1) yl = 1;
    if (bitmap && (al_get_bitmap_width (bitmap) != xl
                || al_get_bitmap_height(bitmap) != yl)) {
        al_destroy_bitmap(bitmap);
        bitmap = 0;
    }
    if (!bitmap) bitmap = al_create_bitmap(xl, yl);
}



int Torbit::distance_x(const int x1, const int x2) const
{
    if (!bitmap) return 0;
    if (!torus_x) return x2 - x1;
    else {
        int result = x2 - x1;
        int d = x2 - x1 - al_get_bitmap_width(bitmap);
        if (std::abs(d) < std::abs(result)) result = d;
        d     = x2 - x1 + al_get_bitmap_width(bitmap);
        if (std::abs(d) < std::abs(result)) result = d;
        return result;
    }
}



int Torbit::distance_y(const int y1, const int y2) const
{
    if (!bitmap) return 0;
    if (!torus_y) return y2 - y1;
    else {
        int result = y2 - y1;
        int d = y2 - y1 - al_get_bitmap_height(bitmap);
        if (std::abs(d) < std::abs(result)) result = d;
        d     = y2 - y1 + al_get_bitmap_height(bitmap);
        if (std::abs(d) < std::abs(result)) result = d;
        return result;
    }
}



double Torbit::hypot(int x1, int y1, int x2, int y2) const
{
    return Help::hypot(distance_x(x2, x1), distance_y(y2, y1), 0, 0);
}



void Torbit::clear_to_color(const ALLEGRO_COLOR& col)
{
    al_set_target_bitmap(bitmap);
    al_clear_to_color(col);
}




ALLEGRO_COLOR Torbit::get_pixel(int x, int y) const
{
    // Am Rand die Karte mit der letzten Pixelreihe gedanklich wiederholt
    // fortsetzen, es sei denn, es ist ohnehin ein Torus.
    if (!bitmap) return color[COL_BLACK];
    const int xl = al_get_bitmap_width (bitmap);
    const int yl = al_get_bitmap_height(bitmap);
    return al_get_pixel(bitmap,
     torus_x ? Help::mod(x, xl) :
     x < 0   ? 0                :
     x >= xl ? xl - 1           : x,
     torus_y ? Help::mod(y, yl) :
     y < 0   ? 0                :
     y >= yl ? yl - 1           : y);
}



void Torbit::set_pixel(const int x, const int y, const ALLEGRO_COLOR& col)
{
    // Hier allerdings ausserhalb des Randes nicht zeichnen, anders als bei
    // Map::get_pixel(), siehe oben.
    if (bitmap) {
        const int xl = al_get_bitmap_width (bitmap);
        const int yl = al_get_bitmap_height(bitmap);
        if ((torus_x || (x >= 0 && x < xl))
         && (torus_y || (y >= 0 && y < yl))) {
            al_set_target_bitmap(bitmap);
            al_draw_pixel(torus_x ? Help::mod(x, xl) : x,
                          torus_y ? Help::mod(y, yl) : y, col);
        }
    }
}



void Torbit::draw_rectangle_private(
    int x,  int y,
    int xl, int yl,
    const ALLEGRO_COLOR& col,
    void (*func)(float, float, float, float, ALLEGRO_COLOR)
) {
    if (!bitmap) return;
    const int bxl = al_get_bitmap_width (bitmap);
    const int byl = al_get_bitmap_height(bitmap);
    if (torus_x) x = Help::mod(x, bxl);
    if (torus_y) y = Help::mod(y, byl);
    const int   x2 = x - bxl;
    const int   y2 = y - byl;
    al_set_target_bitmap(bitmap);
                            func(x,  y,  x  + xl, y  + yl, col);
    if (torus_x)            func(x2, y,  x2 + xl, y  + yl, col);
    if (torus_x)            func(x , y2, x  + xl, y2 + yl, col);
    if (torus_x && torus_y) func(x2, y2, x2 + xl, y2 + yl, col);
}



void Torbit::draw_filled_rectangle(
    int x, int y, int xl, int yl,
    const ALLEGRO_COLOR& col
) {
    draw_rectangle_private(x, y, xl, yl, col, al_draw_filled_rectangle);
}



void Torbit::replace_color(
    const ALLEGRO_COLOR& c_old,
    const ALLEGRO_COLOR& c_new
) {
    if (!bitmap) return;
    rcir_at(0, 0,
     al_get_bitmap_width(bitmap),
     al_get_bitmap_height(bitmap), c_old, c_new);
}



void Torbit::replace_color_in_rect(
    int x, int y, int xl, int yl,
    const ALLEGRO_COLOR& c_old,
    const ALLEGRO_COLOR& c_new
) {
    if (!bitmap) return;
    const int x2 = x - al_get_bitmap_width (bitmap);
    const int y2 = y - al_get_bitmap_height(bitmap);
                            rcir_at(x,  y,  xl, yl, c_old, c_new);
    if (torus_x)            rcir_at(x2, y,  xl, yl, c_old, c_new);
    if (torus_x)            rcir_at(x,  y2, xl, yl, c_old, c_new);
    if (torus_x && torus_y) rcir_at(x2, y2, xl, yl, c_old, c_new);
}



void Torbit::rcir_at(
    int start_x, int start_y, int xl, int yl,
    const ALLEGRO_COLOR& c_old,
    const ALLEGRO_COLOR& c_new
) {
    const int max_xl = al_get_bitmap_width (bitmap);
    const int max_yl = al_get_bitmap_height(bitmap);
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (start_x + xl > max_xl) xl = max_xl - start_x;
    if (start_y + yl > max_yl) yl = max_yl - start_y;

    al_set_target_bitmap(bitmap);
    for  (int x = start_x; x < start_x + xl; ++x)
     for (int y = start_y; y < start_y + yl; ++y) {
        if (al_get_pixel(bitmap, x, y) == c_old) {
            al_put_pixel(        x, y,    c_new);
        }
    }
}



// Ja, ist etwas hackerisch. Es gibt nur eine Funktion, die aufs Bitmap malt
// und prueft, ob die Quelle torusfoermig ist, sowie eine weitere Funktion,
// die aufs Torbit malt und prueft, ob das Ziel torusfoermig ist. Eigentlich
// sollten beide Dinge von einer Funktion beherrscht werden.
void Torbit::draw(
    ALLEGRO_BITMAP* dest,
    int x,  int y,
    int xl, int yl) const
{
    if (!bitmap || !dest) return;
    const int bxl = al_get_bitmap_width (bitmap);
    const int byl = al_get_bitmap_height(bitmap);
    const int dxl = al_get_bitmap_width (dest);
    const int dyl = al_get_bitmap_height(dest);
    if (!xl)  xl  = bxl < dxl ? bxl : dxl;
    if (!yl)  yl  = byl < dyl ? byl : dyl;
    const int xla = x + xl > bxl ? bxl - x : xl;
    const int yla = y + yl > byl ? byl - y : yl;
    const int tx  = torus_x && xla < xl;
    const int ty  = torus_y && yla < yl;

    ALLEGRO_BITMAP* b = bitmap;
    al_set_target_bitmap(dest);
                  al_draw_bitmap_region(b, x, y, xla,    yla,    0,   0,   0);
    if (tx      ) al_draw_bitmap_region(b, 0, y, xl-xla, yla,    xla, 0,   0);
    if (      ty) al_draw_bitmap_region(b, x, 0, xla,    yl-yla, 0,   yla, 0);
    if (tx && ty) al_draw_bitmap_region(b, 0, 0, xl-xla, yl-yla, xla, yla, 0);
}



void Torbit::draw(Torbit& target, int x, int y) const
{
    if (!bitmap || !target.bitmap) return;
    target.draw_from_at(this->bitmap, x, y);
}



void Torbit::draw_from(
    ALLEGRO_BITMAP* bit, int x, int y,
    bool mirr, double rot, double scal)
{
    if (!bit || !bitmap) return;
    double r = ::fmod(rot, 4);

    int mirrflags = 0;
    if (mirr) mirrflags = ALLEGRO_FLIP_HORIZONTAL;
    if (r == 2 && mirr) {
        r = 0;
        mirrflags = ALLEGRO_FLIP_HORIZONTAL;
    }

    if (r == 0 && !scal) draw_from_at(bit, x, y, 0, 0, 0, mirrflags);
    else {
        const int    xl = al_get_bitmap_width (bit);
        const int    yl = al_get_bitmap_height(bit);
        const double cx = xl/2.0;
        const double cy = yl/2.0;

        if (!scal) {
            draw_from_at(bit, x + cx, y + cy,
             0, &::al_draw_rotated_bitmap, 0,
             mirrflags,
             cx, cy,
             r * ALLEGRO_PI / 2.0);
        }
        else {
            draw_from_at(bit, x + cx, y + cy,
             0, 0, &::al_draw_scaled_rotated_bitmap,
             mirrflags,
             cx, cy,
             r * ALLEGRO_PI / 2.0,
             scal);
        }
    }
}



void Torbit::draw_from_at(
    ALLEGRO_BITMAP* bit,
    double x,
    double y,
    void (*fa)(ALLEGRO_BITMAP*, float,float, int),
    void (*fb)(ALLEGRO_BITMAP*, float,float,float,float,float, int),
    void (*fc)(ALLEGRO_BITMAP*, float,float,float,float,float,float,float,int),
    int    mrrf,
    double cx,
    double cy,
    double rad,
    double s
) {
    // Keine Pruefung auf Zeiger == 0, das haben die aufrufenden Funktionen
    // immer schon am Anfang gemacht. War bei denen teilweise naemlich noetig.
    const bool& tx = torus_x;
    const bool& ty = torus_y;
    ALLEGRO_BITMAP*& tb = bitmap;
    const int   tbxl = al_get_bitmap_width (tb);
    const int   tbyl = al_get_bitmap_height(tb);
    if (tx)     x  = Help::mod(x, tbxl);
    if (ty)     y  = Help::mod(y, tbyl);
    if (!fa && !fb && !fc) fa = ::al_draw_bitmap;

    al_set_target_bitmap(tb);
    if (fa) {
                    fa(bit, x,        y,        mrrf);
        if (tx    ) fa(bit, x - tbxl, y,        mrrf);
        if (    ty) fa(bit, x,        y - tbyl, mrrf);
        if (tx&&ty) fa(bit, x - tbxl, y - tbyl, mrrf);
    }
    else if (fb) {
                    fb(bit, cx,      cy,      x,      y,      rad, mrrf);
        if (tx    ) fb(bit, cx-tbxl, cy,      x-tbxl, y,      rad, mrrf);
        if (    ty) fb(bit, cx,      cy-tbyl, x,      y-tbyl, rad, mrrf);
        if (tx&&ty) fb(bit, cx-tbxl, cy-tbyl, x-tbxl, y-tbyl, rad, mrrf);
    }
    else if (fc) {
                    fc(bit, cx,      cy,      x,      y,      s, s, rad, mrrf);
        if (tx    ) fc(bit, cx-tbxl, cy,      x-tbxl, y,      s, s, rad, mrrf);
        if (    ty) fc(bit, cx,      cy-tbyl, x,      y-tbyl, s, s, rad, mrrf);
        if (tx&&ty) fc(bit, cx-tbxl, cy-tbyl, x-tbxl, y-tbyl, s, s, rad, mrrf);
    }
}
