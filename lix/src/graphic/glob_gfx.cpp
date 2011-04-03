// Alle Bitmaps

#include <fstream>

#include "glob_gfx.h"
#include "gra_lib.h" // initialisieren
#include "graphic.h" // Weil bitmap.cpp die Lemming-Graphics erschafft/killt

#include "../api/manager.h" // initializes the manager
#include "../lix/lix.h" // call the initializer of Lemming
#include "../level/obj_lib.h"
#include "../other/globals.h"
#include "../other/help.h" // Dateisuche
#include "../other/language.h"
#include "../other/log.h"
#include "../other/user.h"

ALLEGRO_DISPLAY* displaY = 0;

Torbit *pre_screen = 0;

// Farben
std::vector <ALLEGRO_COLOR> color(COL_MAX);

bool operator == (const ALLEGRO_COLOR& c1, const ALLEGRO_COLOR& c2)
{
    unsigned char r1, g1, b1, a1;
    unsigned char r2, g2, b2, a2;
    al_unmap_rgba(c1, &r1, &g1, &b1, &a1);
    al_unmap_rgba(c2, &r2, &g2, &b2, &a2);
    return r1 == r2 && g1 == g2 && b1 == b2 && a1 == a2;
}



bool operator != (const ALLEGRO_COLOR& c1, const ALLEGRO_COLOR& c2)
{
    return !(c1 == c2);
}



// Schriftarten
ALLEGRO_FONT* font_sml;
ALLEGRO_FONT* font_med;
ALLEGRO_FONT* font_nar;
ALLEGRO_FONT* font_big;

// Diese werden in load_all_bitmaps() aufgerufen
void make_all_colors();
void make_all_bitmap_replacements();
void make_all_lemming_colors();
void make_lemming_color(const LixEn::Style, const int = 0, const int = 0,
                                              const int = 0, const int = 0);

ALLEGRO_FONT* load_one_font(const std::string filename)
{
    ALLEGRO_BITMAP* bmp = al_load_bitmap(filename.c_str());
    ALLEGRO_FONT*   f   = 0;
    if (bmp) {
        al_convert_mask_to_alpha(bmp, color[COL_REALPINK]);
        int ranges[] = {32, 126};
        f = al_grab_font_from_bitmap(bmp, 1, ranges);
        al_destroy_bitmap(bmp);
    }
    return f;
}



void load_all_bitmaps()
{
    // afdebug: set the bitmap flags here

    Api::Manager::initialize(LEMSCR_X, LEMSCR_Y);
    Torbit* osd = &Api::Manager::get_torbit();
    osd->clear_to_color(al_map_rgb(0, 0x80, 0));

    // Nun vernuenftige Sachen laden
    pre_screen = new Torbit(LEMSCR_X, LEMSCR_Y);
    pre_screen->clear_to_color(al_map_rgb(0, 0, 0));

    // Etliche Sachen aus bitlist.cpp
    make_all_colors();
    Lixxie::initialize_this_gets_called_from_glob_gfx_cpp();

    // Bilder laden und so Zeugs
    GraLib::initialize();
    ObjLib::initialize();

    // Schriftarten laden
    al_init_font_addon();
    font_sml = load_one_font(gloB->file_bitmap_font_sml.c_str());
    font_med = load_one_font(gloB->file_bitmap_font_med.c_str());
    font_nar = load_one_font(gloB->file_bitmap_font_nar.c_str());
    font_big = load_one_font(gloB->file_bitmap_font_big.c_str());
}



void destroy_all_bitmaps()
{
    color.clear();

    // Schriftarten
    if (font_sml) al_destroy_font(font_sml);
    if (font_med) al_destroy_font(font_med);
    if (font_nar) al_destroy_font(font_nar);
    if (font_big) al_destroy_font(font_big);
    al_shutdown_font_addon();

    ObjLib::deinitialize();
    GraLib::deinitialize();
    Api::Manager::deinitialize();

    delete pre_screen;
}



// Beim ersten Zeichnen auf jeden Fall mit ggf. farbigem Trauerrand zeichnen
bool clear_screen_at_next_blit = true;

void blit_to_screen(ALLEGRO_BITMAP* b)
{
    const int  screen_xl = al_get_display_width (displaY);
    const int  screen_yl = al_get_display_height(displaY);

    const bool screen_aspect_ratio    = useR->screen_scaling >  0;
    const bool screen_integer_scaling = useR->screen_scaling == 2;

    al_set_target_backbuffer(displaY);

    // afdebug: not necessary in A5?
    // if (gloB->screen_vsync) vsync();
    if (screen_xl == LEMSCR_X && screen_yl == LEMSCR_Y) {
        al_draw_bitmap(b, 0, 0, 0);
    }
    else if (screen_aspect_ratio) {
        if (clear_screen_at_next_blit) {
            clear_screen_at_next_blit = false;
            al_clear_to_color(useR->screen_border_colored
                              ? color[COL_SCREEN_BORDER]
                              : color[COL_BLACK]);
        }
        // Etwas aus ../api/button_c.cpp kopierter Code:
        // Eine Seite soll genau passen, wie viel Platz bei der anderen
        // verschwendet wird, ist egal.
        double factor = (double) screen_xl/LEMSCR_X;
        if (factor > (double) screen_yl/LEMSCR_Y) {
            factor = (double) screen_yl/LEMSCR_Y;
        }
        // Wenn unterschiedlich grosse Pixel vermieden werden sollen, abrunden
        if (screen_integer_scaling) factor = (int) factor;

        al_draw_scaled_bitmap(b, 0, 0, LEMSCR_X, LEMSCR_Y,
                              (int) (screen_xl/2 - LEMSCR_X*factor/2),
                              (int) (screen_yl/2 - LEMSCR_Y*factor/2),
                              (int) (LEMSCR_X*factor), (int) (LEMSCR_Y*factor),
                              0);
    }
    else {
        al_draw_scaled_bitmap(b, 0, 0, LEMSCR_X, LEMSCR_Y,   // Woher kommt's.
                                 0, 0, screen_xl, screen_yl, // Wohin kommt's.
                                 0);                         // flags: !rotate
    }
}



// May change gloB->screen_fullscreen_right_now.
// Other globals are not changed.
void set_screen_mode(const bool full, int res_x, int res_y)
{
    if (displaY) {
        al_destroy_display(displaY);
        displaY = 0;
    }

    if (full) al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    else      al_set_new_display_flags(ALLEGRO_WINDOWED);

    if (res_x == 0 || res_y == 0) {
        if (full) {
            res_x = gloB->screen_resolution_x;
            res_y = gloB->screen_resolution_y;
            if (res_x == 0 || res_y == 0) {
                al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
            }
        }
        else {
            res_x = gloB->screen_windowed_x;
            res_y = gloB->screen_windowed_y;
        }
    }

    if (!displaY) displaY = al_create_display(res_x, res_y);
    if (!displaY) displaY = al_create_display(LEMSCR_X, LEMSCR_Y);
    al_set_new_display_flags(ALLEGRO_WINDOWED);
    if (!displaY) displaY = al_create_display(res_x, res_y);
    if (!displaY) displaY = al_create_display(LEMSCR_X, LEMSCR_Y);

    clear_screen_at_next_blit = true;

    // Do this so the mouse doesn't scroll stupidly after a switch.
    // In hardware.cpp, the mouse is always set to the center anyway, to trap
    // it in the program (scrolling at the sides) and for infinite movement
    // (for scrolling with right mouse button).
    al_set_mouse_xy(displaY, LEMSCR_X/2, LEMSCR_Y/2);
}
