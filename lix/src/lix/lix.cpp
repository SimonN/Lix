/*
 * lemming/lemming.cpp
 *
 * Es gibt in der Tat schoene Spruecke in der INI-Datei von GS Lixxies:
 *
 * Und der Autor sprach: Ich forme dich nach meinem Ebenbild, mit gruenem Haar
 * und nichts zu tun, was auch ich nicht taete. Ebenso wie ich wirst du immer
 * in der Welt des Computers gefangen sein.
 *
 *   -- Aus dem Buch ueber lustige Funktionen, Kapitel create_lemming(ax, ay).
 *
 */

#include <algorithm> // swap
#include <cmath> // sin - Wichtig für die Zündschnur

#include "ac.h" // update args
#include "lix.h"

#include "../gameplay/effect.h"
#include "../graphic/gra_lib.h"
#include "../other/globals.h"   // Dateiname des Lixxie-Bilds
#include "../other/help.h"      // Help::mod

Torbit*        Lixxie::land       = 0;
Torbit*        Lixxie::steel_mask = 0;
Map*           Lixxie::ground_map = 0;
EffectManager* Lixxie::effect     = 0;

const int Lixxie::distance_safe_fall   = 126;
const int Lixxie::distance_float       =  60;
const int Lixxie::updates_for_bomb     =  75;
const int Lixxie::updates_roper_before =  75;
const int Lixxie::updates_roper_after  =  15;



// Dies wird hier nur standardkonstruiert, load_all_bitmaps() fuellt es auf.
Lixxie::Matrix Lixxie::countdown;


Lixxie::Lixxie(
    Tribe*     new_tribe,
    int        new_ex,
    int        new_ey
) :
    Graphic           (GraLib::get_lix(new_tribe ? new_tribe->style
                       : LixEn::GARDEN), *ground_map,
                       new_ex / 2 * 2 - LixEn::ex_offset,
                       new_ey         - LixEn::ey_offset),
    tribe             (new_tribe),
    marked            (0),
    dir               (1),
    special_x         (0),
    special_y         (0),

    frame             (0),
    updates_since_bomb(0),
    exploder_knockback(false),
    climber           (false),
    floater           (false),

    style             (tribe ? tribe->style : LixEn::GARDEN),
    ac                (LixEn::NOTHING)
{
    if (tribe) {
        assign(LixEn::FALLER);
        frame = 4;
    }
    // Wichtig fuer den Torus: Rechtzeitig Modulo rechnen
    set_ex(new_ex/2*2);
    set_ey(new_ey);
}

Lixxie::~Lixxie()
{
}






void Lixxie::set_static_maps(Torbit& l, Torbit& s, Map& g)
{
    land       = &l;
    steel_mask = &s;
    ground_map = &g;
}



void Lixxie::set_ex(const int n) {
    ex = n / 2 * 2;
    set_x(n - LixEn::ex_offset);
    if (ground_map->get_torus_x()) ex = Help::mod(ex, land->get_xl());
}

void Lixxie::set_ey(const int n) {
    set_y(n - LixEn::ey_offset);
    ey = get_y() + LixEn::ey_offset;
    if (ground_map->get_torus_y()) ey = Help::mod(ey, land->get_yl());
}

void Lixxie::move_ahead(int plus_x) { set_ex(ex + plus_x * dir);  }
void Lixxie::move_down (int plus_y) { set_ey(ey + plus_y);        }
void Lixxie::move_up  (int minus_y) { set_ey(ey - minus_y);       }
void Lixxie::turn()                 { dir *= -1;                  }



bool Lixxie::get_in_trigger_area(const EdGraphic& gr, const bool twice) const
{
    const int div = twice ? 1 : 2;
    const Object& ob = *gr.get_object();
    int dx = ground_map->distance_x(get_ex(),
              gr.get_x() + ob.trigger_x + ob.trigger_xl/2);
    int dy = ground_map->distance_y(get_ey(),
              gr.get_y() + ob.trigger_y + ob.trigger_yl/2);
    if (dx < 0) dx *= -1;
    if (dy < 0) dy *= -1;
    return dx <= ob.trigger_xl/div && dy <= ob.trigger_yl/2;
}



const bool Lixxie::get_steel(const int px, const int py) {
    return steel_mask->get_pixel(ex + px*dir, ey+py) == color[COL_STEEL_MASK];
}

bool Lixxie::get_steel_absolute(const int x, const int y) {
    return steel_mask->get_pixel(x, y) == color[COL_STEEL_MASK];
}

const int Lixxie::get_pixel(const int px, const int py) {
    return land->get_pixel(ex + px * dir, ey + py);
}

void Lixxie::set_pixel(const int px, const int py, const int col) {
    land->set_pixel(ex + px * dir, ey + py, col);
}

bool Lixxie::is_solid(int px, int py) {
    return get_pixel(px, py) != color[COL_PINK];
}



int Lixxie::solid_wall_height(const int px, const int py)
{
    int solid = 0;
    for (int i = 1; i > -12; --i) {
        if (is_solid(px, py + i)) ++solid;
        else break;
    }
    return solid;
}



int Lixxie::count_solid(int x1, int y1, int x2, int y2)
{
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);
    // Totaler Rückgabewert
    int ret = 0;
    for (int ix = x1; ix <= x2; ++ix) {
        for (int iy = y1; iy <= y2; ++iy) {
            // Variable für jeden gefundenen Nicht-Luftpixel erhöhen
            if (is_solid(ix, iy)) ++ret;
        }
    }
    return ret;
}



int Lixxie::count_steel(int x1, int y1, int x2, int y2)
{
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);
    // Totaler Rückgabewert
    int ret = 0;
    for (int ix = x1; ix <= x2; ++ix) {
        for (int iy = y1; iy <= y2; ++iy) {
            // Variable für jeden gefundenen Nicht-Luftpixel erhöhen
            if (get_steel(ix, iy)) ++ret;
        }
    }
    return ret;
}

// ############################################################################
// ############################################################## Malfunktionen
// ############################################################################



bool Lixxie::remove_pixel(int px, int py)
{
    // Dies nur bei draw_pixel() und remove_pixel()
    if (dir < 0) --px;

    // Abbaubare Landschaft?
    if (get_pixel(px, py) != color[COL_PINK] && !get_steel(px, py)) {
        set_pixel(px, py, color[COL_PINK]);
        return false;
    }
    // Stahl?
    else if (get_steel(px, py)) return true;
    else                        return false;
}



void Lixxie::remove_pixel_absolute(int x, int y)
{
    if (!get_steel_absolute(x, y)
     && land->get_pixel(x, y) != color[COL_PINK]) {
        land->set_pixel(x, y,    color[COL_PINK]);
    }
}



bool Lixxie::remove_rectangle(int x1, int y1, int x2, int y2)
{
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);
    // Totaler Rückgabewert
    bool ret = false;
    for (int ix = x1; ix <= x2; ++ix) {
        for (int iy = y1; iy <= y2; ++iy) {
            // Ab einem Stahl ist der totale Rückgabewert true
            if (remove_pixel(ix, iy)) ret = true;
        }
    }
    return ret;
}



// Ähnlich wie remove_pixel...
void Lixxie::draw_pixel(int px, int py, int col)
{
    // Dies nur bei draw_pixel() und remove_pixel()
    if (dir < 0) --px;

    if (get_pixel(px, py) == color[COL_PINK]) set_pixel(px, py, col);
}



void Lixxie::draw_rectangle(int x1, int y1, int x2, int y2, int col)
{
    if (x2 < x1) std::swap(x1, x2);
    if (y2 < y1) std::swap(y1, y2);

    // Alle Pixel im Rechteck durchgehen
    for (int ix = x1; ix <= x2; ++ix) {
        for (int iy = y1; iy <= y2; ++iy) {
            draw_pixel(ix, iy, col);
        }
    }
}



void Lixxie::draw_brick(int x1, int y1, int x2, int y2)
{
    const int col_l = get_cutbit()->get_pixel(19, LixEn::BUILDER - 1, 0, 0);
    const int col_m = get_cutbit()->get_pixel(20, LixEn::BUILDER - 1, 0, 0);
    const int col_d = get_cutbit()->get_pixel(21, LixEn::BUILDER - 1, 0, 0);

    draw_rectangle(x1 + (dir<0), y1, x2 - (dir>0), y1, col_l);
    draw_rectangle(x1 + (dir>0), y2, x2 - (dir<0), y2, col_d);
    if (dir > 0) {
        draw_pixel(x2, y1, col_m);
        draw_pixel(x1, y2, col_m);
    }
    else {
        draw_pixel(x1, y1, col_m);
        draw_pixel(x2, y2, col_m);
    }
}



void Lixxie::play_sound(const UpdateArgs& ua, Sound::Id sound_id)
{
    if (effect) effect->add_sound(ua.upd, *tribe, ua.id, sound_id);
}



void Lixxie::play_sound_if_trlo(const UpdateArgs& ua, Sound::Id sound_id)
{
    if (effect) effect->add_sound_if_trlo(ua.upd, *tribe, ua.id, sound_id);
}



bool Lixxie::is_last_frame()
{
    const Cutbit& c = *get_cutbit();
    BITMAP* b = c.get_al_bitmap();
    int pixel_col
     = getpixel(b, (frame + 3)*(c.get_xl()+1)+1,  (ac - 1)*(c.get_yl()+1)+2);
    if (frame == c.get_x_frames() - 3 || pixel_col == getpixel(b, b->w - 1, 0))
     return true;
    return false;
}



void Lixxie::next_frame(int loop)
{
    if (is_last_frame() || frame + 3 == loop) frame = 0;
    else frame++;
}







void Lixxie::draw()
{
    if (ac == LixEn::NOTHING) return;

    set_x_frame(frame + 2);
    set_y_frame(ac - 1);

    // Wenn ein Zählwerk erforderlich ist...
    if (updates_since_bomb > 0) {
        int fuse_x = countdown[get_x_frame()][get_y_frame()].x;
        int fuse_y = countdown[get_x_frame()][get_y_frame()].y;
        if (dir < 0) fuse_x = GraLib::get_lix(style).get_xl() - fuse_x;
        fuse_x += get_x();
        fuse_y += get_y();

        // Hierhin zeichnen
        Torbit& tb = *ground_map;

        int x = 0;
        int y = 0;
        for (; -y < (updates_for_bomb - (int) updates_since_bomb)/5+1; --y) {
            const int u = updates_since_bomb;
            x           = (int) (std::sin(u/2.0) * 0.02 * (y-4) * (y-4));
            tb.set_pixel(fuse_x + x-1, fuse_y + y-1, color[COL_GREY_FUSE_L]);
            tb.set_pixel(fuse_x + x-1, fuse_y + y  , color[COL_GREY_FUSE_L]);
            tb.set_pixel(fuse_x + x  , fuse_y + y-1, color[COL_GREY_FUSE_D]);
            tb.set_pixel(fuse_x + x  , fuse_y + y  , color[COL_GREY_FUSE_D]);
        }
        // Flamme zeichnen
        const Cutbit& cb = GraLib::get(gloB->file_bitmap_fuse_flame);
        cb.draw(*ground_map,
         fuse_x + x - cb.get_xl()/2,
         fuse_y + y - cb.get_yl()/2,
         updates_since_bomb % cb.get_x_frames(), 0);
    }
    // Zuendschnur fertig

    // mirror kippt vertikal, also muss bei dir < 0 auch noch um 180 Grad
    // gedreht werden. Allegro-Zeichenfunktionen bieten oft ebenfalls nur
    // vertikale Kippung, ich benutze daher ebenfalls diese Konvention.
    set_mirror  (     dir < 0 );
    set_rotation(2 * (dir < 0));
    Graphic::draw();
}
