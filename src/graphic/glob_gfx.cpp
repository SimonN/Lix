// Alle Bitmaps

#include "../other/myalleg.h"
#include <fstream>

#include "glob_gfx.h"
#include "gra_lib.h" // initialisieren
#include "graphic.h" // Weil bitmap.cpp die Lix-Graphics erschafft/killt

#include "../api/manager.h" // initializes the manager
#include "../lix/lix.h" // call the initializer of Lixxie
#include "../level/obj_lib.h"
#include "../other/globals.h"
#include "../other/hardware.h" // center mouse after setting gfx mode
#include "../other/help.h" // Dateisuche
#include "../other/language.h"
#include "../other/user.h"

Torbit *pre_screen;

// Farben
std::vector <AlCol> color(COL_MAX);

// Schriftarten
FONT* font_sml;
FONT* font_med;
FONT* font_nar;
FONT* font_big;

// Diese werden in load_all_bitmaps() aufgerufen
void make_all_bitmap_replacements();
void make_all_lix_colors();
void make_lix_color(const LixEn::Style, const int = 0, const int = 0,
                                        const int = 0, const int = 0);

// similar to a straight load_font(), but will try out a priotized sequence
// of slightly different filenames to allow for (advanced) users to provide
// an alternate custom font or supplement the included one
static FONT* load_font_for_lix(Filename const& filename) {
    FONT* result = 0;
    std::string baseparts_rootless = filename.get_dir_rootless() +
                                     filename.get_file_no_ext_no_pre_ext();
    // sequence to try:
    //   (.txt provides best flexibility, while .png and .bmp
    //    have wider software support than .pcx and .tga)
    static char const* const seq[] = {
        ".txt", ".fnt", ".I.png", ".I.bmp", ".I.pcx", ".I.tga",
    };
    static size_t const n = sizeof(seq) / sizeof(seq[0]);

    for(size_t i = 0; 0 == result && i < n; i++) {
        std::string fullpath = Filename(baseparts_rootless + seq[i]).get_rootful();
        if (exists(fullpath.c_str()))
            result = load_font(fullpath.c_str(), 0, 0);
    }

    return result;
}

void load_all_bitmaps(GraLib::RecolorLix recolor_lix_speed_switch)
{
    Api::Manager::initialize(LEMSCR_X, LEMSCR_Y);
    Torbit* osd = &Api::Manager::get_torbit();
    osd->clear_to_color(0);

    // Set the glyph to display for characters not covered in our font.
    // For the loading screen we're using Allegro's built-in font. We
    // will use ? for undisplayable characters.
    allegro_404_char = '?';

    // Anzeige, dass geladen wird
    textout_centre_ex(osd->get_al_bitmap(), font,
     Language::main_loading_1.c_str(),
     LEMSCR_X/2, 0, makecol(255, 255, 255), -1);
    textout_centre_ex(osd->get_al_bitmap(), font,
     Language::main_loading_2.c_str(),
     LEMSCR_X/2, 8, makecol(255, 255, 255), -1);

    const int txtlen = 8 * ustrlen(Language::main_loading_1.c_str());
    const int factor = SCREEN_W/txtlen;
    stretch_blit(osd->get_al_bitmap(), screen,
                 LEMSCR_X/2 - txtlen/2, 0, txtlen, 16,
                 SCREEN_W/2 - factor * (txtlen/2), 2 * factor,
                 txtlen * factor, 16 * factor);
    rectfill(osd->get_al_bitmap(), 0, 0, LEMSCR_X-1, 15, 0);

    // Nun vernuenftige Sachen laden
    pre_screen = new Torbit(LEMSCR_X, LEMSCR_Y);
    pre_screen->clear_to_color(0);

    // Etliche Sachen aus bitlist.cpp
    make_all_colors();
    Lixxie::initialize_this_gets_called_from_glob_gfx_cpp();

    // Bilder laden und so Zeugs
    GraLib::initialize(recolor_lix_speed_switch);
    ObjLib::initialize();

    // Schriftarten laden
    font_sml = load_font_for_lix(gloB->file_bitmap_font_sml);
    font_med = load_font_for_lix(gloB->file_bitmap_font_med);
    font_nar = load_font_for_lix(gloB->file_bitmap_font_nar);
    font_big = load_font_for_lix(gloB->file_bitmap_font_big);

    // For our current fonts, ASCII 0x7F gets mapped to a square, which
    // is a more standard representation for "undisplayble character".
    allegro_404_char = 0x7F;

    // Falls Datei nicht gefunden, Standardfont nutzen, damit das Programm
    // nicht abstuerzt
    font_sml = font_sml ? font_sml : font;
    font_med = font_med ? font_med : font;
    font_nar = font_nar ? font_nar : font;
    font_big = font_big ? font_big : font;
}



void destroy_all_bitmaps()
{
    color.clear();

    // Schriftarten
    if (font_sml != font) destroy_font(font_sml);
    if (font_med != font) destroy_font(font_med);
    if (font_nar != font) destroy_font(font_nar);
    if (font_big != font) destroy_font(font_big);

    ObjLib::deinitialize();
    GraLib::deinitialize();
    Api::Manager::deinitialize();

    delete pre_screen;
}



// Beim ersten Zeichnen auf jeden Fall mit ggf. farbigem Trauerrand zeichnen
bool clear_screen_at_next_blit = true;

void blit_to_screen(BITMAP* b)
{
    const bool screen_aspect_ratio    = useR->screen_scaling >  0;
    const bool screen_integer_scaling = useR->screen_scaling == 2;

    if (gloB->screen_vsync) vsync();
    if (SCREEN_W == LEMSCR_X && SCREEN_H == LEMSCR_Y) {
        blit(b, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }
    else if (screen_aspect_ratio) {
        if (clear_screen_at_next_blit) {
            clear_screen_at_next_blit = false;
            clear_to_color(screen, useR->screen_border_colored
                                 ? color[COL_SCREEN_BORDER]
                                 : 0);
        }
        // Etwas aus ../api/button_c.cpp kopierter Code:
        // Eine Seite soll genau passen, wie viel Platz bei der anderen
        // verschwendet wird, ist egal.
        double factor = (double) SCREEN_W/LEMSCR_X;
        if (factor > (double) SCREEN_H/LEMSCR_Y) {
            factor = (double) SCREEN_H/LEMSCR_Y;
        }
        // Wenn unterschiedlich grosse Pixel vermieden werden sollen, abrunden
        if (screen_integer_scaling) factor = (int) factor;

        stretch_blit(b, screen, 0, 0, LEMSCR_X, LEMSCR_Y,
                     (int) (SCREEN_W/2 - LEMSCR_X*factor/2),
                     (int) (SCREEN_H/2 - LEMSCR_Y*factor/2),
                     (int) (LEMSCR_X*factor), (int) (LEMSCR_Y*factor));
    }
    else {
        stretch_blit(b, screen, 0, 0, LEMSCR_X, LEMSCR_Y,  // Woher kommt's.
                                0, 0, SCREEN_W, SCREEN_H); // Wohin kommt's.
    }
}



// May change gloB->screen_fullscreen_right_now.
// Other globals are not changed.
void set_screen_mode(
    bool full,
    const std::string& modestr,
    int res_x,
    int res_y)
{
    int mode = full ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED;

    if (! modestr.empty()) {
        #if defined _WIN64 || defined _WIN32
            if      (modestr == "GFX_TEXT")
                { mode = GFX_TEXT; full = true; }
            else if (modestr == "GFX_AUTODETECT")
                { mode = GFX_AUTODETECT; full = true; }
            else if (modestr == "GFX_AUTODETECT_FULLSCREEN")
                { mode = GFX_AUTODETECT_FULLSCREEN; full = true; }
            else if (modestr == "GFX_AUTODETECT_WINDOWED")
                { mode = GFX_AUTODETECT_WINDOWED; full = false; }
            else if (modestr == "GFX_SAFE")
                { mode = GFX_SAFE; full = true; }
            else if (modestr == "GFX_DIRECTX")
                { mode = GFX_DIRECTX; full = true; }
            else if (modestr == "GFX_DIRECTX_ACCEL")
                { mode = GFX_DIRECTX_ACCEL; full = true; }
            else if (modestr == "GFX_DIRECTX_SOFT")
                { mode = GFX_DIRECTX_SOFT; full = true;}
            else if (modestr == "GFX_DIRECTX_SAFE")
                { mode = GFX_DIRECTX_SAFE; full = true; }
            else if (modestr == "GFX_DIRECTX_WIN")
                { mode = GFX_DIRECTX_WIN; full = false; }
            else if (modestr == "GFX_DIRECTX_OVL")
                { mode = GFX_DIRECTX_OVL; full = false; }
            else if (modestr == "GFX_GDI")
                { mode = GFX_GDI; full = false; }
        #endif
    }

    if (res_x == 0 || res_y == 0) {
        if (full) {
            res_x = gloB->screen_resolution_x;
            res_y = gloB->screen_resolution_y;
            if (res_x == 0 || res_y == 0)
             get_desktop_resolution(&res_x, &res_y);
        }
        else {
            res_x = gloB->screen_windowed_x;
            res_y = gloB->screen_windowed_y;
        }
    }
    bool      fail = (set_gfx_mode(mode, res_x,    res_y,    0, 0) != 0);
    if (fail) fail = (set_gfx_mode(mode, LEMSCR_X, LEMSCR_Y, 0, 0) != 0);
    if (fail && full) {
        if (res_x == 0 || res_y == 0) {
            get_desktop_resolution(&res_x, &res_y);
            fail = (set_gfx_mode(mode, res_x, res_y, 0, 0) != 0);
        }
    }
    if (fail) {
        mode = GFX_AUTODETECT_WINDOWED;
        set_gfx_mode(mode, LEMSCR_X, LEMSCR_Y, 0, 0);
    }
    if (set_display_switch_mode(SWITCH_BACKGROUND) == -1)
        set_display_switch_mode(SWITCH_BACKAMNESIA);

    clear_screen_at_next_blit = true;
    gloB->screen_fullscreen_now = (mode == GFX_AUTODETECT_FULLSCREEN);
    gloB->screen_modestr_now    = modestr;

    // Do this so the mouse doesn't scroll stupidly after a switch.
    // In hardware.cpp, the mouse is always set to the center anyway, to trap
    // it in the program (scrolling at the sides) and for infinite movement
    // (for scrolling with right mouse button).
    hardware.center_mouse();
}
