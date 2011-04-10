/*
 * level/gra_lib.cpp
 *
 */

#include "gra_lib.h"

#include "../lix/lix.h" // create countdown matrix
#include "../other/help.h" // remove_extension, Dateisuche

GraLib* GraLib::singleton = 0;



GraLib::GraLib()
{
    // Die Verzeichnisse nach Bilddateien durchsuchen
    // Abk.-Deklarationen, um die Funktionsaufrufe in einer Zeile zu haben
    const std::string dd                    = gloB->dir_data_bitmap;
    void (*ssii)(const std::string&, void*) = sort_string_into_internal;

    Help::find_tree(dd, gloB->ext_bmp, ssii, (void*) this);
    Help::find_tree(dd, gloB->ext_tga, ssii, (void*) this);
    Help::find_tree(dd, gloB->ext_pcx, ssii, (void*) this);

    // Countdown-Matrix erstellen
    const Cutbit&    cb = internal[gloB->file_bitmap_lix];
          ALLEGRO_BITMAP*    b  = cb.get_al_bitmap();
    Lixxie::countdown = Lixxie::Matrix(
     cb.get_x_frames(), std::vector <Lixxie::XY> (cb.get_y_frames()) );
    // fx, fy = welcher X- bzw. Y-Frame
    // x,  y  = wo in diesem Frame
    for  (int fy = 0; fy < cb.get_y_frames(); ++fy)
     for (int fx = 0; fx < cb.get_x_frames(); ++fx) {
        for  (int y = 0; y < cb.get_yl(); ++y )
         for (int x = 0; x < cb.get_xl(); ++x ) {
            // Is it the pixel of the eye?
            const int real_x = 1 + fx * (cb.get_xl() + 1) + x;
            const int real_y = 1 + fy * (cb.get_yl() + 1) + y;
            if (al_get_pixel(b, real_x, real_y) == color[COL_LIXFILE_EYE]) {
                Lixxie::countdown[fx][fy].x = x;
                Lixxie::countdown[fx][fy].y = y - 1;
                goto GOTO_NEXTFRAME;
            }
            // If not yet gone to GOTO_NEXTFRAME:
            // Use the XY of the frame left to the current one if there was
            // nothing found, and a default value for the leftmost frames.
            // Frames (0, y) and (1, y) are the skill button images.
            if (y == cb.get_yl() - 1 && x == cb.get_xl() - 1) {
                if (fx < 3) {
                    Lixxie::countdown[fx][fy].x = cb.get_xl() / 2 - 1;
                    Lixxie::countdown[fx][fy].y = 12;
                }
                else Lixxie::countdown[fx][fy] = Lixxie::countdown[fx - 1][fy];
            }
        }
        GOTO_NEXTFRAME:
        if (fy == LixEn::BLOCKER - 1) {
            Lixxie::countdown[fx][fy].x = LixEn::ex_offset;
        }
    }
    // Alle Pixel sind abgegrast.

    recolor_into_vector(internal[gloB->file_bitmap_lix],       style);
    recolor_into_vector(internal[gloB->file_bitmap_game_icon], icons);

    // Alle Namensersetzungen
    replacement["./bitmap/goal/goal.G.bmp"         ] = "./bitmap/goal.G.bmp";
    replacement["./bitmap/hatch/hatch.H.bmp"       ] = "./bitmap/hatch.H.bmp";
    replacement["./bitmap/hazard/trap/10tons.T.bmp"] = "./bitmap/10tons.T.bmp";
    replacement["./bitmap/hazard/trap/spike.T.bmp" ] = "./bitmap/spike.T.bmp";
    replacement["./bitmap/hazard/water/water.W.bmp"] = "./bitmap/water.W.bmp";
}



// Wir speichern alle Bilddateinamen ohne (Endung inklusive Punkt). Das hilft
// beim einfachen Austauschen des benutzten Grafikformates: Man kann einfach
// seine Grafiken konvertieren und die Endung aendern, wenn man den Datei-
// namen ansonsten konstant haelt.
void GraLib::sort_string_into_internal(const std::string& s, void* v) {
    // zweites Argument: Nur Schneideversuch unternehmen, wenn mit Prae-End.
    const Cutbit c(s, Help::string_get_pre_extension(s));
    std::string str = s;
    Help::string_remove_extension(str);
    ((GraLib*) v)->internal.insert(std::make_pair(str, c));
}



void GraLib::recolor_into_vector(
    const Cutbit&         cutbit,
    std::vector <Cutbit>& vector)
{
    ALLEGRO_BITMAP* recol = internal[gloB->file_bitmap_lix_recol].get_al_bitmap();
    ALLEGRO_BITMAP* lix   = cutbit.get_al_bitmap();
    if (!recol || !lix) return;

    ALLEGRO_COLOR col_break = al_get_pixel(lix, cutbit.get_xl() - 1, 0);
    vector = std::vector <Cutbit> (LixEn::STYLE_MAX, cutbit);
    // The first row (y == 0) contains the source pixels. The first style
    // (garden) is at y == 1. Thus the recol->h - 1 is correct as we count
    // styles starting at 0.
    for  (int y = 0; y < cutbit.get_yl(); y++)
     for (int x = 0; x < cutbit.get_xl(); x++)
     for (int conv = 0; conv < al_get_bitmap_width(recol); conv++) {
        const ALLEGRO_COLOR col = al_get_pixel(lix, x, y);
        if (col == col_break) {
            break;
            // immediately begin next pixel, but not next row, because
            // we have separating col_break-colored frames in the file.
        }
        if (col == al_get_pixel(recol, conv, 0)) {
            for (int style_loop = 0; style_loop != LixEn::STYLE_MAX
             && style_loop < al_get_bitmap_height(recol) - 1; ++style_loop) {
                al_set_target_bitmap(vector[style_loop].get_al_bitmap());
                al_put_pixel(x, y, al_get_pixel(recol, conv, style_loop + 1));
            }
            break; // break out of conv loop, don't replace this pixel again
        }
        // end if color matches
    }
    // end of all color replacement

    //    // This saving is just while LEMDEBUG is happening
    //    for (size_t i = 0; i < singl->style.size(); ++i) {
    //        std::string filename = "./lixstyle";
    //        filename += (int) i;
    //        filename += ".bmp";
    //        save_bmp(filename.c_str(), singl->style[i].get_al_bitmap(), 0);
    //    }
}



GraLib::~GraLib()
{
}



void GraLib::initialize()
{
    if (!singleton) singleton = new GraLib;
}



void GraLib::deinitialize()
{
    if (singleton) {
        delete singleton;
        singleton = 0;
    }
}



const Cutbit& GraLib::get(const std::string& str)
{
    return singleton->internal[str];
}



const Cutbit& GraLib::get_lix(const LixEn::Style st)
{
    return singleton->style[st];
}



const Cutbit& GraLib::get_icon(const LixEn::Style st)
{
    return singleton->icons[st];
}



const std::string& GraLib::replace_string(const std::string& str)
{
    const std::map <std::string, std::string> ::const_iterator
     itr = singleton->replacement.find(str);
    if (itr != singleton->replacement.end()) return itr->second;
    else return str;
}
