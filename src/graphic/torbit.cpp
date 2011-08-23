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
    bitmap = create_bitmap(xl, yl);
}



Torbit::Torbit(const Torbit& orig)
:
    torus_x(orig.torus_x),
    torus_y(orig.torus_y)
{
    bitmap = create_bitmap(orig.get_xl(), orig.get_yl());
    blit(orig.bitmap, bitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
}



Torbit::~Torbit()
{
    if (bitmap) {
        destroy_bitmap(bitmap);
        bitmap = 0;
    }
}



Torbit& Torbit::operator = (const Torbit& orig)
{
    if (this == &orig) return *this;

    torus_x = orig.torus_x;
    torus_y = orig.torus_y;
    resize(orig.get_xl(), orig.get_yl());
    blit(orig.bitmap, bitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
    return *this;
}



void Torbit::resize(int xl, int yl)
{
    if (xl < 1) xl = 1;
    if (yl < 1) yl = 1;
    if (bitmap && (bitmap->w != xl || bitmap->h != yl)) {
        destroy_bitmap(bitmap);
        bitmap = 0;
    }
    if (!bitmap) bitmap = create_bitmap(xl, yl);
}



int Torbit::distance_x(const int x1, const int x2) const
{
    if (!bitmap) return 0;
    if (!torus_x) return x2 - x1;
    else {
        int result = x2 - x1;
        int d = x2 - x1 - bitmap->w;
        if (std::abs(d) < std::abs(result)) result = d;
        d     = x2 - x1 + bitmap->w;
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
        int d = y2 - y1 - bitmap->h;
        if (std::abs(d) < std::abs(result)) result = d;
        d     = y2 - y1 + bitmap->h;
        if (std::abs(d) < std::abs(result)) result = d;
        return result;
    }
}



double Torbit::hypot(int x1, int y1, int x2, int y2) const
{
    return Help::hypot(distance_x(x2, x1), distance_y(y2, y1), 0, 0);
}



void Torbit::clear_to_color(int col)
{
    ::clear_to_color(bitmap, col);
}




int Torbit::get_pixel(int x, int y) const
{
    // Am Rand die Karte mit der letzten Pixelreihe gedanklich wiederholt
    // fortsetzen, es sei denn, es ist ohnehin ein Torus.
    if (!bitmap) return 0;
    return _getpixel16(bitmap,
     torus_x        ? Help::mod(x, bitmap->w) :
     x < 0          ? 0                       :
     x >= bitmap->w ? bitmap->w - 1           : x,
     torus_y        ? Help::mod(y, bitmap->h) :
     y < 0          ? 0                       :
     y >= bitmap->h ? bitmap->h - 1           : y);
}



void Torbit::set_pixel(const int x, const int y, const int col)
{
    // Hier allerdings ausserhalb des Randes nicht zeichnen, anders als bei
    // Map::get_pixel(), siehe oben.
    if (bitmap
     && (torus_x || (x >= 0 && x < bitmap->w))
     && (torus_y || (y >= 0 && y < bitmap->h)) )
     _putpixel16(bitmap, torus_x ? Help::mod(x, bitmap->w) : x,
                         torus_y ? Help::mod(y, bitmap->h) : y, col);
}



void Torbit::draw_rectangle_private(
    int x,  int y,
    int xl, int yl,
    const int col,
    void (*func)(BITMAP*, int, int, int, int, int)
) {
    if (!bitmap) return;
    if (torus_x) x = Help::mod(x, bitmap->w);
    if (torus_y) y = Help::mod(y, bitmap->h);
    const int   x2 = x - bitmap->w;
    const int   y2 = y - bitmap->h;
                            func(bitmap, x,  y,  x  + xl-1, y  + yl-1, col);
    if (torus_x)            func(bitmap, x2, y,  x2 + xl-1, y  + yl-1, col);
    if (torus_x)            func(bitmap, x , y2, x  + xl-1, y2 + yl-1, col);
    if (torus_x && torus_y) func(bitmap, x2, y2, x2 + xl-1, y2 + yl-1, col);
}



void Torbit::draw_rectangle(int x, int y, int xl, int yl, int col)
{
    draw_rectangle_private(x, y, xl, yl, col, ::rect);
}



void Torbit::draw_filled_rectangle(int x, int y, int xl, int yl, int col)
{
    draw_rectangle_private(x, y, xl, yl, col, ::rectfill);
}



void Torbit::replace_color(int c_old, int c_new)
{
    if (!bitmap) return;
    rcir_at(0, 0, bitmap->w, bitmap->h, c_old, c_new);
}



void Torbit::replace_color_in_rect(
    int x, int y, int xl, int yl, int c_old, int c_new
) {
    if (!bitmap) return;
    const int x2 = x - bitmap->w;
    const int y2 = y - bitmap->h;
                            rcir_at(x,  y,  xl, yl, c_old, c_new);
    if (torus_x)            rcir_at(x2, y,  xl, yl, c_old, c_new);
    if (torus_x)            rcir_at(x,  y2, xl, yl, c_old, c_new);
    if (torus_x && torus_y) rcir_at(x2, y2, xl, yl, c_old, c_new);
}



void Torbit::rcir_at(
    int start_x, int start_y, int xl, int yl, int c_old, int c_new
) {
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (start_x + xl > bitmap->w) xl = bitmap->w - start_x;
    if (start_y + yl > bitmap->h) yl = bitmap->h - start_y;

    for  (int x = start_x; x < start_x + xl; ++x)
     for (int y = start_y; y < start_y + yl; ++y) {
        if (_getpixel16(bitmap, x, y) == c_old) {
            _putpixel16(bitmap, x, y,    c_new);
        }
    }
}



// Ja, ist etwas hackerisch. Es gibt nur eine Funktion, die aufs Bitmap malt
// und prueft, ob die Quelle torusfoermig ist, sowie eine weitere Funktion,
// die aufs Torbit malt und prueft, ob das Ziel torusfoermig ist. Eigentlich
// sollten beide Dinge von einer Funktion beherrscht werden.
void Torbit::draw(
    BITMAP* dest,
    int x,  int y,
    int xl, int yl) const
{
    if (!bitmap || !dest) return;
    if (!xl)  xl  = bitmap->w < dest->w ? bitmap->w : dest->w;
    if (!yl)  yl  = bitmap->h < dest->h ? bitmap->h : dest->h;
    const int xla = x + xl > bitmap->w ? bitmap->w - x : xl;
    const int yla = y + yl > bitmap->h ? bitmap->h - y : yl;
    const int tx  = torus_x && xla < xl;
    const int ty  = torus_y && yla < yl;
                  masked_blit(bitmap, dest, x, y, 0,   0,   xla,    yla);
    if (tx      ) masked_blit(bitmap, dest, 0, y, xla, 0,   xl-xla, yla);
    if (      ty) masked_blit(bitmap, dest, x, 0, 0,   yla, xla,    yl-yla);
    if (tx && ty) masked_blit(bitmap, dest, 0, 0, xla, yla, xl-xla, yl-yla);
}



void Torbit::draw(Torbit& target, int x, int y) const
{
    if (!bitmap || !target.bitmap) return;
    target.draw_from_at(this->bitmap, x, y);
}



void Torbit::draw_from(
    BITMAP* bit, int x, int y,
    bool mirr, double rot, double scal)
{
    if (!bit || !bitmap) return;
    double r = ::fmod(rot, 4);
    if      (r == 0 && !scal && !mirr) draw_from_at(bit, x, y);
    else if (r == 2 && !scal &&  mirr) draw_from_at(bit, x, y,
                                                    ::draw_sprite_h_flip);
    else {
        bool b = (r == 0 || r == 2);
        const int xl = bit->w;
        const int yl = bit->h;
        // rotate_sprite dreht leider um das Zentrum und hat obendrein
        // wuselige Angewohnheiten, wann bei ungeraden Laengen nach vorn
        // oder hinten gerundet wird. Mein Algorithmus faengt das auf.

        // Wir koennen Truemmer trotzdem mit dieser Methode zeichnen!
        // Fuer xl == yl liefert dies gerade wieder das Urspruengliche!

        //  Malposition     Zentrum versch.   Rundungskorrektur
        int xdr = b ? x  :  x - xl/2 + yl/2 - ((xl + yl)%2 && xl < yl);
        int ydr = b ? y  :  y - yl/2 + xl/2 - ((xl + yl)%2 && yl < xl);
        // Weitere, ganz bescheuerter Rundungsfehler
        if (!b && xl%2 == 1 && yl%2 == 0 && yl < xl) {
            --xdr;
            ++ydr;
        }
        else if (!b && xl%2 == 0 && yl%2 == 1 && xl < yl) {
            ++xdr;
            --ydr;
        }
        // fixed sind die Allegro-Typen, die die Sprite-Funktionen wollen
        if (!mirr) draw_from_at(bit, xdr, ydr, 0,::rotate_scaled_sprite,
                    rot, scal);
        else       draw_from_at(bit, xdr, ydr, 0,::rotate_scaled_sprite_v_flip,
                    rot, scal);
    }
}



void Torbit::draw_from_at(
    BITMAP* bit,
    int x,
    int y,
    void (*arg4)(BITMAP*, BITMAP*, int, int),
    void (*arg6)(BITMAP*, BITMAP*, int, int, fixed, fixed),
    double rot,
    double scal
) {
    // Keine Pruefung auf Zeiger == 0, das haben die aufrufenden Funktionen
    // immer schon am Anfang gemacht. War bei denen teilweise naemlich noetig.
    const bool& tx = torus_x;
    const bool& ty = torus_y;
    BITMAP*&    tb = bitmap;
    if (tx)     x  = Help::mod(x, tb->w);
    if (ty)     y  = Help::mod(y, tb->h);

    if (!arg4 && !arg6) arg4 = ::draw_sprite;
    if (arg4) {
                      arg4(tb, bit, x,         y        );
        if (tx      ) arg4(tb, bit, x - tb->w, y        );
        if (      ty) arg4(tb, bit, x,         y - tb->h);
        if (tx && ty) arg4(tb, bit, x - tb->w, y - tb->h);
    }
    else if (arg6) {
        if (!scal) scal = 1;
        fixed      rr   = ftofix(rot * 64);
        fixed      ff   = ftofix(scal ? scal : 1);
                      arg6(tb, bit, x,         y,         rr, ff);
        if (tx      ) arg6(tb, bit, x - tb->w, y,         rr, ff);
        if (      ty) arg6(tb, bit, x,         y - tb->h, rr, ff);
        if (tx && ty) arg6(tb, bit, x - tb->w, y - tb->h, rr, ff);
    }
}
