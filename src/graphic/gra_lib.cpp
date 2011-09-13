/*
 * level/gra_lib.cpp
 *
 */

#include "gra_lib.h"

#include "../lix/lix.h" // create countdown matrix
#include "../other/help.h" // remove_extension, Dateisuche

GraLib* GraLib::singleton = 0;

static const int magicnr_sheet = 1;
static const int magicnr_icons = 2;



GraLib::GraLib()
{
    // Die Verzeichnisse nach Bilddateien durchsuchen
    // Abk.-Deklarationen, um die Funktionsaufrufe in einer Zeile zu haben
    const Filename& dd                   = gloB->dir_data_bitmap;
    void (*ssii)(const Filename&, void*) = sort_string_into_internal;

    Help::find_tree(dd, gloB->mask_ext_bmp, ssii, (void*) this);
    Help::find_tree(dd, gloB->mask_ext_tga, ssii, (void*) this);
    Help::find_tree(dd, gloB->mask_ext_pcx, ssii, (void*) this);

    // Countdown-Matrix erstellen
    const Cutbit& cb = internal[gloB->file_bitmap_lix.
                                get_rootless_no_extension()];
          BITMAP* b  = cb.get_al_bitmap();
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
            if (_getpixel16(b, real_x, real_y) == color[COL_LIXFILE_EYE]) {
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

    // Prepare Lix sprites in multiple colors
    recolor_into_vector(cb, style, magicnr_sheet);
    recolor_into_vector(internal[gloB->file_bitmap_game_icon.
                                 get_rootless_no_extension()], icons, magicnr_icons);

    // Make GUI elements have the correct colors
    eidrecol_api(gloB->file_bitmap_api_number);
    eidrecol_api(gloB->file_bitmap_checkbox);
    eidrecol_api(gloB->file_bitmap_edit_flip);
    eidrecol_api(gloB->file_bitmap_edit_hatch);
    eidrecol_api(gloB->file_bitmap_edit_panel);
    eidrecol_api(gloB->file_bitmap_game_arrow);
    eidrecol_api(gloB->file_bitmap_game_icon);
    eidrecol_api(gloB->file_bitmap_game_nuke);
    eidrecol_api(gloB->file_bitmap_game_panel);
    eidrecol_api(gloB->file_bitmap_game_panel_2);
    eidrecol_api(gloB->file_bitmap_game_pause);
    eidrecol_api(gloB->file_bitmap_lobby_spec);
    eidrecol_api(gloB->file_bitmap_preview_icon);
    eidrecol_api(gloB->file_bitmap_preview_icon);

    // Alle Namensersetzungen
    add_replace("bitmap/Universal/water.W",  "bitmap/matt/water.W"  );
    add_replace("bitmap/Universal/10tons.T", "bitmap/matt/10tons.T" );

    add_substr_replace("bitmap/Universal/", "bitmap/matt/steel/");
    add_substr_replace("bitmap/Rebuilds/",  "bitmap/matt/");
}



// Wir speichern alle Bilddateinamen ohne (Endung inklusive Punkt). Das hilft
// beim einfachen Austauschen des benutzten Grafikformates: Man kann einfach
// seine Grafiken konvertieren und die Endung aendern, wenn man den Datei-
// namen ansonsten konstant haelt.
void GraLib::sort_string_into_internal(const Filename& fn, void* v) {
    // zweites Argument: Nur Schneideversuch unternehmen, wenn mit Prae-End.
    const Cutbit c(fn, fn.get_pre_extension());
    ((GraLib*) v)->internal.insert(
        std::make_pair(fn.get_rootless_no_extension(), c));
}



void GraLib::eidrecol_api(const Filename& filename)
{
    Cutbit& cutbit = internal[filename.get_rootless_no_extension()];
    eidrecol_api(cutbit);
}



void GraLib::eidrecol_api(Cutbit& cutbit, int magicnr)
{
    BITMAP* bitmap = cutbit.get_al_bitmap();
    if (! bitmap) return;

    if (! magicnr)
     for (int y = 0; y < bitmap->h; ++y) {
        const bool light = (y > cutbit.get_yl());
        for (int x = 0; x < bitmap->w; ++x) {
            const AlCol c = ::getpixel(bitmap, x, y);
            if      (c == color[COL_BLACK]) putpixel(bitmap, x, y,
                          color[COL_PINK]);
            else if (c == color[COL_API_FILE_SHADOW]) putpixel(bitmap, x, y,
                          color[COL_API_SHADOW]);
            else if (c == color[COL_API_FILE_D]) putpixel(bitmap, x, y,
                  color[light ? COL_API_PIC_ON_D : COL_API_PIC_D]);
            else if (c == color[COL_API_FILE_M]) putpixel(bitmap, x, y,
                  color[light ? COL_API_PIC_ON_M : COL_API_PIC_M]);
            else if (c == color[COL_API_FILE_L]) putpixel(bitmap, x, y,
                  color[light ? COL_API_PIC_ON_L : COL_API_PIC_L]);
        }
    }
    else if (magicnr == magicnr_sheet)
     for (int y = 0; y < bitmap->h; ++y) {
        for (int x = 0; x < 2 * (cutbit.get_xl() + 1); ++x) {
            const AlCol c = ::getpixel(bitmap, x, y);
            if      (c == color[COL_API_FILE_SHADOW]) putpixel(bitmap, x, y,
                          color[COL_API_SHADOW]);
            else if (x < cutbit.get_xl() + 1) continue;
            else if (c == color[COL_BLACK]) putpixel(bitmap, x, y,
                          color[COL_PINK]);
            else if (c == color[COL_API_FILE_D]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_D]);
            else if (c == color[COL_API_FILE_M]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_M]);
            else if (c == color[COL_API_FILE_L]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_L]);
        }
    }
    else if (magicnr == magicnr_icons)
     for (int y = cutbit.get_yl() + 1; y < 2 * (cutbit.get_yl() + 1); ++y) {
        for (int x = 0; x < bitmap->w; ++x) {
            const AlCol c = ::getpixel(bitmap, x, y);
            if      (c == color[COL_BLACK]) putpixel(bitmap, x, y,
                          color[COL_PINK]);
            else if (c == color[COL_API_FILE_SHADOW]) putpixel(bitmap, x, y,
                          color[COL_API_SHADOW]);
            else if (c == color[COL_API_FILE_D]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_D]);
            else if (c == color[COL_API_FILE_M]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_M]);
            else if (c == color[COL_API_FILE_L]) putpixel(bitmap, x, y,
                          color[COL_API_PIC_L]);
        }
    }
}



void GraLib::recolor_into_vector(
    const Cutbit&         cutbit,
    std::vector <Cutbit>& vector,
    int                   magicnr
) {
    BITMAP* recol = internal[gloB->file_bitmap_lix_recol.
                             get_rootless_no_extension()].get_al_bitmap();
    BITMAP* lix   = cutbit.get_al_bitmap();
    if (!recol || !lix) return;

    int col_break = getpixel(lix, lix->w - 1, 0);
    vector = std::vector <Cutbit> (LixEn::STYLE_MAX, cutbit);
    // The first row (y == 0) contains the source pixels. The first style
    // (garden) is at y == 1. Thus the recol->h - 1 is correct as we count
    // styles starting at 0.
    for  (int y = 0; y < lix->h; y++)
     for (int x = 0; x < lix->w; x++) {
        if (x == cutbit.get_xl() + 1 && magicnr == magicnr_sheet) {
            // skip the column with the greyed out skill icons
            x += cutbit.get_xl();
            continue;
        }
        else if (magicnr == magicnr_icons && y >= cutbit.get_yl() + 1
                                          && y <  2 * (cutbit.get_yl() + 1)) {
            // skip all x pixels in the second row in this
            continue;
        }
        for (int conv = 0; conv < recol->w; conv++) {
            const int col = getpixel(lix, x, y);
            if (col == col_break) {
                break;
                // immediately begin next pixel, but not next row, because
                // we have separating col_break-colored frames in the file.
            }
            if (col == getpixel(recol, conv, 0)) {
                for (int style_loop = 0; style_loop != LixEn::STYLE_MAX
                 && style_loop < recol->h - 1; ++style_loop) {
                    ::putpixel(vector[style_loop].get_al_bitmap(), x, y,
                     ::getpixel(recol, conv, style_loop + 1));
                }
                break; // break out of conv loop, don't replace pixel again
            }
            // end if color matches
        }
        // end of all color replacement
    }
    // end of for-all-pixels in source

    for (std::vector <Cutbit> ::iterator itr = vector.begin();
        itr != vector.end(); ++itr) {
        eidrecol_api(*itr, magicnr);
    }

    //    // This can save the palette-replaced style to a file
    //    for (size_t i = 0; i < 2; ++i) {
    //        std::string filename = "./lixstyle";
    //        filename += (int) i;
    //        filename += ".bmp";
    //        save_bmp(filename.c_str(), style[i].get_al_bitmap(), 0);
    //    }
}



void GraLib::add_replace(const std::string& a, const std::string& b) {
    replace_exact .insert(std::make_pair <std::string, std::string> (a, b));
}
void GraLib::add_substr_replace(const std::string& a, const std::string& b) {
    replace_substr.insert(std::make_pair <std::string, std::string> (a, b));
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



const Cutbit& GraLib::get(const Filename& fn)
{
    return singleton->internal[fn.get_rootless_no_extension()];
}



const Cutbit& GraLib::get_lix(const LixEn::Style st)
{
    return singleton->style[st];
}



const Cutbit& GraLib::get_icon(const LixEn::Style st)
{
    return singleton->icons[st];
}



std::string GraLib::replace_filestring(const std::string& str)
{
    typedef std::map <std::string, std::string> ::const_iterator CItr;

    const CItr exactitr = singleton->replace_exact.find(str);
    if (exactitr != singleton->replace_exact.end()) return exactitr->second;

    // If we get to here, no match has been found in the exact replacements
    for (CItr itr =  singleton->replace_substr.begin();
              itr != singleton->replace_substr.end(); ++itr) {
        size_t pos = str.find(itr->first);
        if (pos != std::string::npos) {
            std::string replcopy = str;
            replcopy.replace(pos, itr->first.length(), itr->second);
            return replcopy;
        }
    }

    // Nothing found in the substring replacements
    return str;
}
