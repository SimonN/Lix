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
#include "../other/help.h" // Dateisuche
#include "../other/language.h"
#include "../other/log.h"
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

void load_all_bitmaps()
{
    Api::Manager::initialize(LEMSCR_X, LEMSCR_Y);
    Torbit* osd = &Api::Manager::get_torbit();
    osd->clear_to_color(0);

    // Anzeige, dass geladen wird
    textout_centre_ex(osd->get_al_bitmap(), font,
     Language::main_loading_1.c_str(),
     LEMSCR_X/2, 0, makecol(255, 255, 255), -1);
    textout_centre_ex(osd->get_al_bitmap(), font,
     Language::main_loading_2.c_str(),
     LEMSCR_X/2, 8, makecol(255, 255, 255), -1);

    const int txtlen = 8 * Language::main_loading_1.size();
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
    GraLib::initialize();
    ObjLib::initialize();

    // Schriftarten laden
    font_sml = load_font(gloB->file_bitmap_font_sml.get_rootful().c_str(),0,0);
    font_med = load_font(gloB->file_bitmap_font_med.get_rootful().c_str(),0,0);
    font_nar = load_font(gloB->file_bitmap_font_nar.get_rootful().c_str(),0,0);
    font_big = load_font(gloB->file_bitmap_font_big.get_rootful().c_str(),0,0);

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
void set_screen_mode(const bool full, int res_x, int res_y)
{
    int mode = full ? GFX_AUTODETECT_FULLSCREEN : GFX_AUTODETECT_WINDOWED;
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
    if (fail) {
        mode = GFX_AUTODETECT_WINDOWED;
        set_gfx_mode(mode, LEMSCR_X, LEMSCR_Y, 0, 0);
    }
    if (set_display_switch_mode(SWITCH_BACKGROUND) == -1)
        set_display_switch_mode(SWITCH_BACKAMNESIA);

    clear_screen_at_next_blit = true;
    gloB->screen_fullscreen_now = (mode == GFX_AUTODETECT_FULLSCREEN);

    // Do this so the mouse doesn't scroll stupidly after a switch.
    // In hardware.cpp, the mouse is always set to the center anyway, to trap
    // it in the program (scrolling at the sides) and for infinite movement
    // (for scrolling with right mouse button).
    position_mouse(LEMSCR_X/2, LEMSCR_Y/2);
}
