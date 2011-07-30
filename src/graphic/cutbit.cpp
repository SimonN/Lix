/*
 * cutbit.cpp
 *
 */

#include <sstream>

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
    if (c.bitmap) {
        bitmap = create_bitmap(c.bitmap->w, c.bitmap->h);
        ::blit(c.bitmap, bitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
    }
}



Cutbit::Cutbit(BITMAP* b, const bool cut)
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
        xl = bitmap->w;
        yl = bitmap->h;
    }
}



Cutbit::Cutbit(const std::vector <BITMAP*>& vec)
:
    bitmap(0),
    xl(0),
    yl(0),
    x_frames(0),
    y_frames(0)
{
    if      (vec.empty()) return;
    else if (vec.size() == 1) {
        bitmap = create_bitmap(vec[0]->w, vec[0]->h);
        if (bitmap) {
            xl       = bitmap->w;
            yl       = bitmap->h;
            x_frames = 1;
            y_frames = 1;
            ::blit(vec[0], bitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
        }
        // do not cut
    }
    else {
        xl = vec[0]->w;
        yl = vec[0]->h;
        for (std::vector <BITMAP*> ::const_iterator
         itr = vec.begin(); itr != vec.end(); ++itr)
         if ((*itr)->w != xl || (*itr)->h != yl) return;

        x_frames = vec.size();
        y_frames = 1;
        bitmap = create_bitmap((xl+1) * vec.size() + 1,   yl + 2);
        if (bitmap) {
            clear_to_color(bitmap, makecol(127, 0, 127));
            for (int fr = 0; fr < (int) vec.size(); ++fr) {
                ::blit(vec[fr], bitmap, 0, 0, (xl+1)*fr+1, 1, xl, yl);
                // Do not cut, all important things from cut_bitmap are done
            }
        }
    }
}



Cutbit::Cutbit(const Filename& filename, const bool cut)
:
    bitmap  (0),
    xl      (0),
    yl      (0),
    x_frames(1),
    y_frames(1)
{
    // Angegebene Datei als Bild laden.
    // Wenn dies kein Bild ist, Fehlermeldung schreiben und abbrechen.
    bitmap = load_bitmap(filename.get_rootful().c_str(), 0);
    if (!bitmap) {
        std::string str = Language::log_bitmap_bad;
        str += " ";
        str += filename.get_rootless();
        Log::log(Log::ERROR, str);
        return;
    }
    if (cut) cut_bitmap();
    else {
        xl = bitmap->w;
        yl = bitmap->h;
    }
}



Cutbit::~Cutbit() {
    if (bitmap) destroy_bitmap(bitmap);
}



Cutbit& Cutbit::operator = (const Cutbit& c)
{
    // Bitmap neu erstellen: Altes loeschen, sofern vorhanden
    if (bitmap) destroy_bitmap(bitmap);
    if (c.bitmap) {
        bitmap = create_bitmap(c.bitmap->w, c.bitmap->h);
        ::blit(c.bitmap, bitmap, 0, 0, 0, 0, bitmap->w, bitmap->h);
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
    int c = getpixel(bitmap, 0, 0);
    if (getpixel(bitmap, 0, 1) == c
     && getpixel(bitmap, 1, 0) == c
     && getpixel(bitmap, 1, 1) != c) {

        // Test auf horizontale Frame-Laenge
        for (xl = 2; xl < bitmap->w; ++xl) {
            if (getpixel(bitmap, xl, 1) == c) {
                --xl;
                break;
            }
        }

        // Test auf vertikale Frame-Laenge
        for (yl = 2; yl < bitmap->h; ++yl) {
            if (getpixel(bitmap, 1, yl) == c) {
                --yl;
                break;
            }
        }

        // Wenn maximal ein Frame in jeder Richtung in das Bitmap passt...
        if (xl * 2 > bitmap->w && yl * 2 > bitmap->h) {
            xl = bitmap->w;
            yl = bitmap->h;
            x_frames = 1;
            y_frames = 1;
        }
        // ...sonst die Menge der Frames bestimmen.
        else {
            for (x_frames = 0; (x_frames+1)*(xl+1) < bitmap->w; ++x_frames);
            for (y_frames = 0; (y_frames+1)*(yl+1) < bitmap->h; ++y_frames);
        }
    }

    // Wenn kein Rahmen erkennbar ist
    else {
        xl = bitmap->w;
        yl = bitmap->h;
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



int Cutbit::get_pixel(const int px, const int py) const
{
    return get_pixel(0, 0, px, py);
}

int Cutbit::get_pixel(const int fx, const int fy,
                      const int px, const int py) const
{
    // Frame oder Pixel pro Frame nicht existent
    if  (fx < 0 || fy < 0 || fx >= x_frames || fy >= y_frames
     ||  px < 0 || py < 0 || px >= xl       || py >= yl) return -1;
    // Ansonsten liefere Farbe
    else if (x_frames == 1 && y_frames == 1)
         return _getpixel16(bitmap, px, py);
    else return _getpixel16(bitmap, fx * (xl+1) + 1 + px,
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
    BITMAP* target = target_torbit.get_al_bitmap();
    if (bitmap && fx >= 0 && fy >= 0 && fx < x_frames && fy < y_frames) {
        // Schneide zunaechst ein Subbitmap aus unter Beachtung der Framewahl
        // Hat das Bild keine Frames, entfaellt +1 fuer den aeusseren Rahmen.
        BITMAP* sprite;
        if (x_frames == 1 && y_frames == 1)
             sprite = create_sub_bitmap(bitmap, 0, 0, xl, yl);
        else sprite = create_sub_bitmap(bitmap, fx * (xl+1) + 1,
                                                fy * (yl+1) + 1, xl, yl);
        if (mode == NORMAL) {
            target_torbit.draw_from(sprite, x, y, mirr, rot, scal);
        }
        else {
            const int PINK  = color[COL_PINK];
            const int BLACK = color[COL_BLACK];
            const int GREY  = color[COL_EDITOR_DARK];
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
                    const int c = target_torbit.get_pixel(x + ix, y + iy);
                    const int e = excerpt      .get_pixel(    ix,     iy);
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
        destroy_bitmap(sprite);
    }

    // Keine gueltige Frame-Angabe
    else {
        int          col_text = makecol(255, 255, 255);
        int          col_back = makecol( 64,  64,  64);
        if (!bitmap) col_back = makecol(255,  64,  64);
        std::ostringstream str;
        str << "( " << fx << " | " << fy << " )";
        textout_ex(target, font, "Frame?!?!", x, y,        col_text, col_back);
        textout_ex(target, font, str.str().c_str(), x, y+8,col_text, col_back);
    }
}
