/*
 * level/level_dr.cpp
 *
 * Zeichnen von Levels
 *
 */

#include "level.h"
#include "obj_lib.h"

#include "../gameplay/lookup.h"
#include "../graphic/graphic.h"    // to access rotated pixel functions

#include "../api/button/b_skill.h" // to export it as an image
#include "../api/labeltwo.h"       // to export it as an image
#include "../api/manager.h"        // getting access to where buttons are drawn
#include "../other/language.h"     // export
#include "../graphic/gra_lib.h"

#include "../other/help.h"

void Level::draw_to(Torbit& ground, Lookup* lookup) const
{
    ground.clear_to_color(color[COL_PINK]);
    ground.set_torus_xy(torus_x, torus_y);
    if (lookup) {
        lookup->resize(size_x, size_y, torus_x, torus_y);
    }
    // Durch die Terrain-Liste iterieren, Wichtiges zuletzt blitten (obenauf)
    for (PosIt i =  pos[Object::TERRAIN].begin();
               i != pos[Object::TERRAIN].end(); ++i) {
        drit(i, ground, lookup);
    }
}



void Level::drit(PosIt itr, Torbit& ground, Lookup* lookup) const
{
    if (!itr->ob) return;
    if (!itr->ob->cb) return;
    const Cutbit& bit = itr->ob->cb;

    bit.draw(ground, itr->x, itr->y, 0, 0, itr->mirr,
     itr->ob->type == Object::HATCH ? 0 : itr->rot, 0,
       itr->noow ? Cutbit::NOOW
     : itr->dark ? Cutbit::DARK
     :             Cutbit::NORMAL);
    // Ist's Stahl? Nochmal blitten, auf die Stahlmaske!
    // Dies wird jedoch nur ausgeführt, wenn ein Zeiger dafür übergeben wurde.
    // Da alles immer mit masked_blit auch auf die Stahlmaske geblittet wird
    // und diese zu Beginn schwarz ist, enthaelt sie niemals pinke Pixel --
    // bis auf die, die wir bei Stahl selbst einzeichnen!
    if (lookup && itr->ob->type == Object::TERRAIN) {
        // If this gets ever ported to A5, then the bitmap should get locked
        // read-only into memory here, then read pixels, then release.
        // This won't draw to ground, it's just to access rotated pixels.
        Graphic tempgra(bit, ground);
        tempgra.set_rotation(itr->rot);
        tempgra.set_mirror  (itr->mirr);
        for  (int x = itr->x; x < itr->x + tempgra.get_xl(); ++x)
         for (int y = itr->y; y < itr->y + tempgra.get_yl(); ++y)
         if (tempgra.get_pixel(x - itr->x, y - itr->y) != color[COL_PINK]) {
            if (itr->noow) {
                if (! lookup->get(x, y, Lookup::bit_terrain))
                    lookup->add(x, y, itr->ob->subtype == 1 ?
                    Lookup::bit_steel | Lookup::bit_terrain :
                    Lookup::bit_terrain);
            }
            else if (itr->dark)
                lookup->rm(x, y, Lookup::bit_terrain | Lookup::bit_steel);
            else if (itr->ob->subtype == 1)
                lookup->add(x, y, Lookup::bit_terrain | Lookup::bit_steel);
            else {
                lookup->add(x, y, Lookup::bit_terrain);
                lookup->rm (x, y, Lookup::bit_steel);
            }
        }
        // end of single pixel
    }
    // end of preparing lookup
}



Torbit Level::create_preview(int w, int h, int c) const
{
    Torbit b(w, h);
    b.clear_to_color(c);
    if (status == BAD_FILE_NOT_FOUND
     || status == BAD_EMPTY) return b;

    // Sonst hier mit dem Malen beginnen
    Torbit temp_ter(size_x, size_y);
    Torbit temp_obj(size_x, size_y); // may be returned later
    temp_ter.clear_to_color(color[COL_PINK]);
    temp_obj.clear_to_color(color[COL_PINK]);
    temp_ter.set_torus_xy(torus_x, torus_y);
    temp_obj.set_torus_xy(torus_x, torus_y);

    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (PosIt i =  pos[Object::perm(type)].begin();
                i != pos[Object::perm(type)].end(); ++i)
     drit(i, Object::perm(type) == Object::TERRAIN ? temp_ter : temp_obj);

    temp_obj.replace_color(color[COL_PINK], makecol(bg_red,bg_green,bg_blue));
    temp_ter.draw(temp_obj);

    // Etwas aus ../api/button_c.cpp kopierter Code:
    // Eine Seite soll genau passen, wie viel Platz bei der anderen
    // verschwendet wird, ist egal.
    double                          factor = (double) w/size_x;
    if (factor > (double) h/size_y) factor = (double) h/size_y;

    if (w != size_x || h != size_y) {
        stretch_blit(temp_obj.get_al_bitmap(),
            b   .get_al_bitmap(), 0, 0,
            temp_obj.get_xl(), temp_obj.get_yl(),
            (int) (w/2 - size_x*factor/2),
            (int) (h/2 - size_y*factor/2),
            // Jetzt wieder Rundung, aber dass es dann in seltenen Faellen
            // immer noch nicht passt bzw. um einen Pixel nicht, habe
            // ich immer noch nicht verstanden.
            (int) (size_x*factor + 0.5),
            (int) (size_y*factor + 0.5));
        return b;
    }
    else return temp_obj;
}



void Level::export_image(const Filename& filename) const
{
    const int min_export_xl = LEMSCR_X;

    Torbit smallcanvas = create_preview(
        size_x > min_export_xl ? size_x : min_export_xl,
        size_y, color[COL_API_M]);

    Torbit canvas(smallcanvas.get_xl(), smallcanvas.get_yl() + 60);
    canvas.clear_to_color(color[COL_API_M]);
    smallcanvas.draw(canvas);

    Api::SkillButton skill_button(0, 0);
    Torbit& osd = Api::Manager::get_torbit();
    osd.clear_to_color(color[COL_PINK]);

    for (int i = 0; i < gloB->skill_max; ++i) {
       skill_button.set_skill (skill[i].ac);
       skill_button.set_number(skill[i].nr);
       skill_button.draw();
       osd.draw(canvas, skill_button.get_xl() * i, size_y);
    }

    osd.clear_to_color(color[COL_PINK]);

    Api::LabelTwo info_initial (10,  3, 140);
    Api::LabelTwo info_spawnint(10, 20, 140);
    Api::LabelTwo info_clock   (10, 37, 140);
    info_initial .set_desc(Language::browser_info_initial);
    info_spawnint.set_desc(Language::win_var_spawnint + ":");
    info_clock   .set_desc(Language::browser_info_clock_2);
    info_initial .set_value(initial);
    info_spawnint.set_value(spawnint);
    info_clock   .set_value_seconds_as_time(seconds);
    info_initial .draw();
    info_spawnint.draw();
    info_clock   .draw();
    osd.draw(canvas, skill_button.get_xl() * gloB->skill_max, size_y);

    // Draw torus information, copied from menu/preview.cpp
    Graphic icon_torus(GraLib::get(gloB->file_bitmap_preview_icon), canvas);
    icon_torus.set_x_frame(torus_x + 2 * torus_y);
    icon_torus.set_y_frame(1);
    icon_torus.draw();

    save_bmp(filename.get_rootful().c_str(), canvas.get_al_bitmap(), 0);

    // Cleanup what we have done, redraw the browser that's open
    Api::Manager::force_redraw();
}
