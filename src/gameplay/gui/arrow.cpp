/*
 * gameplay/arrow.cpp
 *
 */

#include "arrow.h"

#include "../../graphic/gra_lib.h"
#include "../../level/level.h"     // Mindestmasse
#include "../../other/help.h"      // Help::random_double()
#include "../../other/globals.h"   // Bitmap fuer den Pfeil

const int Arrow::offset_x  = 30;
const int Arrow::offset_y  = 56;
const int Arrow::offset_sx = 12;
const int Arrow::offset_sy =  2;

Arrow::Arrow(Torbit& b, LixEn::Style style,
 const int x, const int y, LixEn::Ac ac, unsigned long upd, const int lid)
:
    Graphic(GraLib::get(gloB->file_bitmap_game_arrow),
            b, x - offset_x,     y - offset_y),
    skill  (GraLib::get_lix(style), b, x - offset_x + offset_sx,
                                       y - offset_y + offset_sy),
    age    (0),
    update (upd),
    lem_id (lid)
{
    skill.set_y_frame(ac != LixEn::NOTHING ? ac - 1 : 0);
}

Arrow::~Arrow()
{
}

bool Arrow::operator == (const Arrow& a)
{
    return get_x()             == a.get_x()
     &&    skill.get_y_frame() == a.skill.get_y_frame() // Skill
     &&    skill.get_cutbit()  == a.skill.get_cutbit()  // Style
     &&    update              == a.update
     &&    lem_id              == a.lem_id;
}

void Arrow::set_x(const int i)
{
    Graphic::set_x(i);
    skill   .set_x(i + offset_sx);
}

void Arrow::set_y(const int i)
{
    Graphic::set_y(i);
    skill   .set_y(i + offset_sy);
}

void Arrow::draw()
{
    Graphic::draw();
    skill   .draw();
}



// ############################################################################
// ##################################################################### Debris
// ############################################################################



const int Debris::ttl_min               =  30;
const int Debris::ttl_max               = 150;
const int Debris::ttl_pickaxe           = 150;
const int Debris::x_speed_max           =   7;
const int Debris::y_speed_min           = -10;
const int Debris::y_speed_max           =   0;

const int Debris::pixel_ttl_min         =  60;
const int Debris::pixel_ttl_max         = 180;
const int Debris::pixel_x_speed_max     =   6;
const int Debris::pixel_y_speed_min     =  -8;
const int Debris::pixel_y_speed_max     =   0;

const int Debris::explosion_y_frame_min =   1;




Debris::Debris(Map& m, const int nx, const int ny)
:
    map    (m),
    cutbit (&GraLib::get(gloB->file_bitmap_debris)),
    color  (rand() % (cutbit->get_y_frames()
            - explosion_y_frame_min) + explosion_y_frame_min),
    ttl    (rand() % (ttl_max - ttl_min) + ttl_min),
    x      (nx - cutbit->get_xl()/2),
    y      (ny - cutbit->get_yl()/2),
    x_speed(Help::random_double(-x_speed_max, x_speed_max)),
    y_speed(Help::random_double( y_speed_min, y_speed_max)),
    rot    (0)
{
    common_constructor();
}



// Pixel
Debris::Debris(Map& m, const int nx, const int ny, const LixEn::Style st)
:
    map    (m),
    cutbit (0),
    color  (GraLib::get_lix(st).get_pixel(2 + rand() % 3,
            LixEn::EXPLODER2 - 1, 0, 0)),
    ttl    (rand() % (pixel_ttl_max - pixel_ttl_min) + pixel_ttl_min),
    x      (nx),
    y      (ny),
    x_speed(Help::random_double(-pixel_x_speed_max, pixel_x_speed_max)),
    y_speed(Help::random_double( pixel_y_speed_min, pixel_y_speed_max)),
    rot    (0)
{
    common_constructor();
}



// Bei der Hacke etc.: Extra anders y-Speed bestimmen, immer etwas nach oben.
Debris::Debris(
    Map& m, const int nx, const int ny, const int nyf, const int nrot
) :
    map    (m),
    cutbit (&GraLib::get(gloB->file_bitmap_debris)),
    color  (nyf),
    ttl    (ttl_pickaxe),
    x      (nx - cutbit->get_xl()/2),
    y      (ny - cutbit->get_yl()/2),
    x_speed(Help::random_double(-x_speed_max/2, x_speed_max/2)),
    y_speed(Help::random_double( y_speed_min,   y_speed_min/2)),
    rot    (nrot)
{
    common_constructor();
}



void Debris::common_constructor()
{
    // On wrapping maps, reduce the time to live
    if (map.get_torus_y() && map.get_yl() < 3 * map.get_screen_yl()) {
        const double ratio = map.get_yl() / map.get_screen_yl();
        // ratio is at most 3 because of the if above.
        ttl = static_cast <int> (ttl * (0.20 + 0.26 * ratio));
    }
}



Debris::~Debris()
{
}



void Debris::calc()
{
    y_speed += (cutbit ? 0.2 : 0.12);
    x       += x_speed;
    y       += y_speed;

    if (ttl > 0) --ttl;
    const bool& tx = map.get_torus_x();
    const bool& ty = map.get_torus_y();
    const int   xl = (cutbit ? cutbit->get_xl() : 2);
    if ((!tx && (x + xl < 0 || x > map.get_xl()) )
     || (!ty &&                y > map.get_yl()  ) )
        ttl = 0;

    if (cutbit) rot += (x_speed / 20);
}



void Debris::draw()
{
    if (cutbit) {
        int x_frame = cutbit->get_x_frames() - 1 - ttl/4;
        if (x_frame < 0) x_frame = 0;
        cutbit->draw(map, (int) x, (int) y, x_frame, color, false, rot);
    }
    else {
        map.set_pixel((int) x,   (int) y,   color);
        map.set_pixel((int) x+1, (int) y,   color);
        map.set_pixel((int) x,   (int) y+1, color);
        map.set_pixel((int) x+1, (int) y+1, color);
    }
}
