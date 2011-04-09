/*
 * cutbit.cpp
 *
 */

#include "cutbit.h"

#include "../other/help.h" // Fehlertext beim Blitten nicht vorhandener Frames
#include "../other/language.h"
#include "../other/log.h"

Cutbit::Cutbit()
:
    bitmap(0),
    xl    (0),
    yl    (0),
    x_frames(0),
    y_frames(0)
{
}



Cutbit::Cutbit(const Cutbit& c)
:
    bitmap(0), // Standardwert, wenn if weiter unten nicht gilt
    xl(c.xl),
    yl(c.yl),
    x_frames(c.x_frames),
    y_frames(c.y_frames)
{
    if (c.bitmap) bitmap = al_clone_bitmap(c.bitmap);
}



Cutbit::Cutbit(ALLEGRO_BITMAP* b, const bool cut)
:
    bitmap(b),
    xl(0),
    yl(0),
    x_frames(1),
    y_frames(1)
{
    if (!bitmap) return;
    if (cut) cut_bitmap();
    else {
        xl = al_get_bitmap_width (bitmap);
        yl = al_get_bitmap_height(bitmap);
    }
}



Cutbit::Cutbit(const std::vector <ALLEGRO_BITMAP*>& vec)
:
    bitmap(0),
    xl(0),
    yl(0),
    x_frames(0),
    y_frames(0)
{
    if      (vec.empty()) return;
    else if (vec.size() == 1) {
        bitmap = al_create_bitmap(al_get_bitmap_width (vec[0]),
                                  al_get_bitmap_height(vec[0]));
        if (bitmap) {
            xl       = al_get_bitmap_width (bitmap);
            yl       = al_get_bitmap_height(bitmap);
            x_frames = 1;
            y_frames = 1;
            al_set_target_bitmap(bitmap);
            al_draw_bitmap(vec[0], 0, 0, 0);
        }
        // do not cut
    }
    else {
        xl = al_get_bitmap_width (vec[0]);
        yl = al_get_bitmap_height(vec[0]);
        for (std::vector <ALLEGRO_BITMAP*> ::const_iterator
         itr = vec.begin(); itr != vec.end(); ++itr)
         if (al_get_bitmap_width (*itr) != xl
          || al_get_bitmap_height(*itr) != yl) return;

        x_frames = vec.size();
        y_frames = 1;
        bitmap = al_create_bitmap((xl+1) * vec.size() + 1,   yl + 2);
        if (bitmap) {
            al_set_target_bitmap(bitmap);
            al_clear_to_color(al_map_rgb(127, 0, 127));
            for (int fr = 0; fr < (int) vec.size(); ++fr) {
                al_draw_bitmap(vec[fr], (xl+1)*fr+1, 1, 0);
                // Do not cut, all important things from cut_bitmap are done
            }
        }
    }
}



Cutbit::Cutbit(const std::string& filename, const bool cut)
:
    bitmap  (0),
    xl      (0),
    yl      (0),
    x_frames(1),
    y_frames(1)
{
    // Angegebene Datei als Bild laden.
    // Wenn dies kein Bild ist, Fehlermeldung schreiben und abbrechen.
    bitmap = al_load_bitmap(filename.c_str());
    if (!bitmap) {
        std::string str = Language::log_bitmap_bad;
        str += " ";
        str += filename;
        Log::log(Log::ERROR, str);
        return;
    }
    al_set_target_bitmap(bitmap);
    al_convert_mask_to_alpha(bitmap, color[COL_REALPINK]);
    if (cut) cut_bitmap();
    else {
        xl = al_get_bitmap_width (bitmap);
        yl = al_get_bitmap_height(bitmap);
    }
}



Cutbit::~Cutbit()
{
    if (bitmap) al_destroy_bitmap(bitmap);
}



Cutbit& Cutbit::operator = (const Cutbit& c)
{
    // Bitmap neu erstellen: Altes loeschen, sofern vorhanden
    if (bitmap) al_destroy_bitmap(bitmap);
    if (c.bitmap) {
        bitmap = al_clone_bitmap(c.bitmap);
    }
    else bitmap = 0;
    // Restliche Werte richtig setzen
    xl = c.xl;
    yl = c.yl;
    x_frames = c.x_frames;
    y_frames = c.y_frames;
    return *this;
}



void Cutbit::cut_bitmap()
{
    // Wird nur fuer Bilder mit Prae-Erweiterung normal geschnitten.
    // Ist das Grundmuster eines Rahmens, eines Rasters erkennbar?
    ALLEGRO_COLOR c = al_get_pixel(bitmap, 0, 0);
    if (al_get_pixel(bitmap, 0, 1) == c
     && al_get_pixel(bitmap, 1, 0) == c
     && al_get_pixel(bitmap, 1, 1) != c) {

        // Test auf horizontale Frame-Laenge
        for (xl = 2; xl < al_get_bitmap_width(bitmap); ++xl) {
            if (al_get_pixel(bitmap, xl, 1) == c) {
                --xl;
                break;
            }
        }

        // Test auf vertikale Frame-Laenge
        for (yl = 2; yl < al_get_bitmap_height(bitmap); ++yl) {
            if (al_get_pixel(bitmap, 1, yl) == c) {
                --yl;
                break;
            }
        }

        // Wenn maximal ein Frame in jeder Richtung in das Bitmap passt...
        if (xl * 2 > al_get_bitmap_width (bitmap)
         && yl * 2 > al_get_bitmap_height(bitmap)) {
            xl = al_get_bitmap_width (bitmap);
            yl = al_get_bitmap_height(bitmap);
            x_frames = 1;
            y_frames = 1;
        }
        // ...sonst die Menge der Frames bestimmen.
        else {
            for (x_frames = 0;
             (x_frames+1)*(xl+1) < al_get_bitmap_width (bitmap); ++x_frames);
            for (y_frames = 0;
             (y_frames+1)*(yl+1) < al_get_bitmap_height(bitmap); ++y_frames);
        }
    }

    // Wenn kein Rahmen erkennbar ist
    else {
        xl = al_get_bitmap_width (bitmap);
        yl = al_get_bitmap_height(bitmap);
        x_frames = 1;
        y_frames = 1;
    }
    // Jetzt ist das Bild fertig zerschnitten.
}



bool Cutbit::operator == (const Cutbit& c) const
{
    return bitmap == c.bitmap;
}

bool Cutbit::operator != (const Cutbit& c) const
{
    return bitmap != c.bitmap;
}



ALLEGRO_COLOR Cutbit::get_pixel(const int px, const int py) const
{
    return get_pixel(0, 0, px, py);
}

ALLEGRO_COLOR Cutbit::get_pixel(const int fx, const int fy,
                                const int px, const int py) const
{
    // Frame oder Pixel pro Frame nicht existent
    if  (fx < 0 || fy < 0 || fx >= x_frames || fy >= y_frames
     ||  px < 0 || py < 0 || px >= xl       || py >= yl)
     return color[COL_PINKAF];
    // Ansonsten liefere Farbe
    else if (x_frames == 1 && y_frames == 1)
         return al_get_pixel(bitmap, px, py);
    else return al_get_pixel(bitmap, fx * (xl+1) + 1 + px,
                                     fy * (yl+1) + 1 + py);
}



// ############################################################################
// ################################################################### Blitting
// ############################################################################



void Cutbit::draw(
    Torbit&      target_torbit,
    const int    x,
    const int    y,
    const int    fx,
    const int    fy,
    const bool   mirr,
    const double rot,
    const double scal,
    const Mode   mode) const
{
    ALLEGRO_BITMAP* target = target_torbit.get_al_bitmap();
    if (bitmap && fx >= 0 && fy >= 0 && fx < x_frames && fy < y_frames) {
        // Schneide zunaechst ein Subbitmap aus unter Beachtung der Framewahl
        // Hat das Bild keine Frames, entfaellt +1 fuer den aeusseren Rahmen.
        ALLEGRO_BITMAP* sprite;
        if (x_frames == 1 && y_frames == 1)
             sprite = al_create_sub_bitmap(bitmap, 0, 0, xl, yl);
        else sprite = al_create_sub_bitmap(bitmap, fx * (xl+1) + 1,
                                                   fy * (yl+1) + 1, xl, yl);
        if (mode == NORMAL) {
            target_torbit.draw_from(sprite, x, y, mirr, rot, scal);
        }
        else {
            const ALLEGRO_COLOR PINK  = color[COL_PINK];
            const ALLEGRO_COLOR BLACK = color[COL_BLACK];
            const ALLEGRO_COLOR GREY  = color[COL_EDITOR_DARK];
            const int size  = xl > yl ? xl : yl;
            Torbit excerpt(size, size);
            excerpt.clear_to_color(PINK);
            excerpt.draw_from(sprite, 0, 0, mirr, rot, scal);
            if (mode == NOOW) {
                target_torbit.draw     (excerpt.get_al_bitmap(), x, y);
                target_torbit.draw_from(excerpt.get_al_bitmap(), x, y);
            }
            else if (mode == NOOW_EDITOR) {
                for  (int ix = 0; ix < size; ++ix)
                 for (int iy = 0; iy < size; ++iy) {
                    ALLEGRO_COLOR c = target_torbit.get_pixel(x + ix, y + iy);
                    ALLEGRO_COLOR e = excerpt      .get_pixel(    ix,     iy);
                    if ((c == BLACK || c == PINK || c == GREY)
                     &&  e != BLACK && e != PINK)
                     target_torbit.set_pixel(x + ix, y + iy, e);
            }   }
            else if (mode == DARK
                  || mode == DARK_EDITOR
                  || mode == DARK_SHOW_NOOW) {
                for  (int ix = 0; ix < size; ++ix)
                 for (int iy = 0; iy < size; ++iy)
                 if (excerpt      .get_pixel(  ix,   iy) != PINK
                 && (target_torbit.get_pixel(x+ix, y+iy) == PINK
                                                 || mode != DARK_SHOW_NOOW))
                 target_torbit.set_pixel(x+ix, y+iy,
                 mode == DARK ? PINK : GREY);
            }
            else if (mode == STEEL || mode == STEEL_NOOW) {
                // Fuer stahlfarbiges Zeichnen auf pinken Hintergrund
                for  (int ix = 0; ix < size; ++ix)
                 for (int iy = 0; iy < size; ++iy)
                 if (excerpt      .get_pixel(  ix,   iy) != PINK
                 && (target_torbit.get_pixel(x+ix, y+iy) == PINK
                                                 || mode != STEEL_NOOW))
                     target_torbit.set_pixel(x+ix, y+iy,
                      color[COL_STEEL_MASK]);
            }
        }
        // Fertig
        al_destroy_bitmap(sprite);
    }

    // Keine gueltige Frame-Angabe
    else {
        al_set_target_bitmap(target);
        if (bitmap) {
            ALLEGRO_COLOR col_text = al_map_rgb(255, 255, 255);
            std::string str = "( ";
            str += fx; str += " | "; str += fy; str += " )";
            al_draw_text(font_sml, col_text, x, y,    0, "Frame?");
            al_draw_text(font_sml, col_text, x, y+10, 0, str.c_str());
        }
        else {
             ALLEGRO_COLOR col_text = al_map_rgb(255,  64,  64);
             al_draw_text(font_sml, col_text, x, y,    0, "Not loaded!");
        }
    }
}
