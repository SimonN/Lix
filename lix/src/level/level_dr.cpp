/*
 * level/level_dr.cpp
 *
 * Zeichnen von Levels
 *
 */

#include "level.h"
#include "obj_lib.h"

#include "../other/help.h"

void Level::draw_to(Torbit& ground, Torbit* steel_mask) const
{
    ground.clear_to_color(color[COL_PINK]);
    ground.set_torus_xy(torus_x, torus_y);
    if (steel_mask) {
        steel_mask->clear_to_color(color[COL_PINK]);
        steel_mask->set_torus_xy(torus_x, torus_y);
    }
    // Durch die Terrain-Liste iterieren, Wichtiges zuletzt blitten (obenauf)
    for (PosIt i =  pos[Object::TERRAIN].begin();
               i != pos[Object::TERRAIN].end(); ++i) {
        drit(i, ground, steel_mask);
    }
}



void Level::drit(PosIt itr, Torbit& ground, Torbit* steel_mask) const
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
    if (steel_mask) {
        const bool ter = itr->ob->type == Object::TERRAIN;
        const bool st  = ter && itr->ob->subtype == 1 && ! itr->dark;
        if (st)
         bit.draw(*steel_mask, itr->x, itr->y, 0, 0, itr->mirr, itr->rot, 0,
           itr->noow ? Cutbit::STEEL_NOOW
         : itr->dark ? Cutbit::DARK
         :             Cutbit::STEEL);
        else if (ter)
         bit.draw(*steel_mask, itr->x, itr->y, 0, 0, itr->mirr, itr->rot, 0,
            itr->noow ? Cutbit::DARK_SHOW_NOOW
          : itr->dark ? Cutbit::DARK
          :             Cutbit::DARK_EDITOR);
    }
}



Torbit Level::create_preview(unsigned w, unsigned h, int c) const
{
    Torbit b(w, h);
    b.clear_to_color(c);
    if (status == BAD_FILE_NOT_FOUND
     || status == BAD_EMPTY) return b;

    // Sonst hier mit dem Malen beginnen
    Torbit temp_ter(size_x, size_y);
    Torbit temp_obj(size_x, size_y);
    temp_ter.clear_to_color(color[COL_PINK]);
    temp_obj.clear_to_color(color[COL_PINK]);
    temp_ter.set_torus_xy(torus_x, torus_y);
    temp_obj.set_torus_xy(torus_x, torus_y);

    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (PosIt i =  pos[Object::perm(type)].begin();
                i != pos[Object::perm(type)].end(); ++i)
     drit(i, Object::perm(type) == Object::TERRAIN ? temp_ter : temp_obj);

    temp_obj.replace_color(color[COL_PINK], color[COL_BLACK]);
    temp_ter.draw(temp_obj);

    // Etwas aus ../api/button_c.cpp kopierter Code:
    // Eine Seite soll genau passen, wie viel Platz bei der anderen
    // verschwendet wird, ist egal.
    double                          factor = (double) w/size_x;
    if (factor > (double) h/size_y) factor = (double) h/size_y;

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
