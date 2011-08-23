/*
 * editor/editor_d.cpp
 *
 * Funktionen zum Entzeichnen und Zeichnen des Bildschirms im Editor:
 *
 *   void Editor::undraw()
 *   void Editor::draw()
 *
 */

#include <sstream>

#include "editor.h"

#include "../api/manager.h" // osd
#include "../other/user.h"

void Editor::undraw()
{
    // OSD zuerst entzeichnen
    mouse_cursor.undraw();

    if (frame_draw_x1 != -1) draw_selection_frame(bg_color);
}



// ############################################################################
// ############################################################################
// ############################################################################



void Editor::draw()
{
    // Draw the map
    // If drawing takes very long, wait until after dragging.
    if (draw_required && (draw_dragging || (
        !hardware.key_hold(KEY_UP)
     && !hardware.key_hold(KEY_RIGHT)
     && !hardware.key_hold(KEY_DOWN)
     && !hardware.key_hold(KEY_LEFT)
     && !hardware.get_mlh() ))) {
        draw_required = false;
        int clock     = Help::timer_ticks;
        // clear_screen_rectangle() is not enough to prevent drag remainders
        map.clear_to_color(color[COL_PINK]);
        for (int type = Object::TERRAIN; type != Object::MAX; ++type)
         for (GraIt i =  object[Object::perm(type)].begin();
                    i != object[Object::perm(type)].end(); ++i)
                        i->draw_with_trigger_area();
        clock = Help::timer_ticks - clock;
        if (clock < Help::timer_ticks_per_second / 5) draw_dragging = true;
        else                                          draw_dragging = false;
    }

    draw_selection_borders();

    // Write numbers onto hatches and goals
    GraIt    i;
    unsigned n;
    for (n = 0, i =  object[Object::GOAL] .begin();
                i != object[Object::GOAL] .end(); ++i)
     draw_object_with_numbers(i, ++n, object[Object::GOAL] .size());
    for (n = 0, i =  object[Object::HATCH].begin();
                i != object[Object::HATCH].end(); ++i)
     draw_object_with_numbers(i, ++n, object[Object::HATCH].size());



    // OSD help texts for buttons
    // A TextButton with left text alignment also starts at x = 3.
    if (Api::Manager::get_focus() == 0) {
        rectfill(Api::Manager::get_torbit().get_al_bitmap(),
         0, LEMSCR_Y-80, LEMSCR_X-1, LEMSCR_Y-61, color[COL_PINK]);
        if (panel[HELP].get_on())
         for (unsigned i = 0; i < panel.size(); ++i)
         if (panel[i].is_mouse_here()) {
            std::string str = Language::editor_button[i];
            int key   = panel[i].get_hotkey();
            bool hold = false;
            if (i>=GRID_2 && i<=GRID_16) key = useR->key_ed_grid;
            else if (i == SELECT_FRAME)  key = useR->key_ed_sel_frame;
            else if (i == SELECT_ADD)    key = useR->key_ed_sel_add;
            else if (i == SELECT_BACK)   key = useR->key_ed_background;
            else if (i == SELECT_FRONT)  key = useR->key_ed_foreground;
            if (i == SELECT_BACK || i == SELECT_FRONT) hold = true;
            if (key) {
                str += " ";
                str += hold ? Language::editor_hotkey_hold
                            : Language::editor_hotkey;
                str += " [";
                str += scancode_to_name(key);
                str += "]";
            }
            Help::draw_shadow_text(Api::Manager::get_torbit(), font_med,
             str.c_str(), 3, LEMSCR_Y - 80);
        }
    }



    // Mouse cursor on top
    // The mouse positioning code is here as well, as Editor::calc_self()
    // is skipped whenever a sub-window is open.
    mouse_cursor.set_x(hardware.get_mx()-mouse_cursor_offset);
    mouse_cursor.set_y(hardware.get_my()-mouse_cursor_offset);
    mouse_cursor.draw();

    pre_screen->clear_to_color(bg_color);

    // Draw everything to pre_screen
    map       .draw(*pre_screen);
    map_frames.draw(*pre_screen);
    Api::Manager::draw_to_pre_screen();
    blit_to_screen(pre_screen->get_al_bitmap());
}
// End of draw()














// Nebenfunktionen, die in der Hauptmalfunktion aufgerufen werden
void Editor::draw_selection_borders()
{
    // Farbverlauf errechnen
    unsigned t = ticks / Help::get_timer_ticks_per_draw();
    int col = t%20;
    if (t%40 >= 20) col = 20 - (t%20);
    const int c1 = 100 - col*2;
    const int c2 = 255 - col*4;
    const int col_hov = makecol(c1, c1, c1);
    const int col_sel = makecol(c2, c2, c2);

    if (map_frames.get_xl() != map.get_xl()
     || map_frames.get_yl() != map.get_yl())
     map_frames.resize(map.get_xl(), map.get_yl());
    map_frames.set_torus_x (map.get_torus_x ());
    map_frames.set_torus_y (map.get_torus_y ());
    map_frames.set_screen_x(map.get_screen_x());
    map_frames.set_screen_y(map.get_screen_y());
    map_frames.set_zoom    (map.get_zoom()    );

    map_frames.clear_screen_rectangle(color[COL_PINK]);

    // Draw frame around hover objects, after that around all selected objects
    for (SelIt i = hover.begin(); i != hover.end(); ++i)
     draw_selection_border(*i, col_hov);
    for (SelIt i = selection.begin(); i != selection.end(); ++i)
     draw_selection_border(*i, col_sel);

    // Drag-around frame
    if (panel[SELECT_FRAME].get_on() && hardware.get_mlh()
     && mouse_hold_started_outside_panel) {
        draw_selection_frame(col_hov);
    }
    else {
        frame_draw_x1 = -1;
        frame_draw_y1 = -1;
        frame_draw_x2 = -1;
        frame_draw_y2 = -1;
    }
}











void Editor::draw_selection_border(const Selection& i, const int col)
{
    const bool& tx = level.torus_x;
    const bool& ty = level.torus_y;
                  draw_selection_border_at(i, col, 0,            0           );
    if (tx)       draw_selection_border_at(i, col, map.get_xl(), 0           );
    if (ty)       draw_selection_border_at(i, col, 0,            map.get_yl());
    if (tx && ty) draw_selection_border_at(i, col, map.get_xl(), map.get_yl());
}

void Editor::draw_selection_border_at
(const Selection& i, const int col, const int px, const int py)
{
    const int x1 = i.o->get_x() + i.o->get_selbox_x()  - px;
    const int y1 = i.o->get_y() + i.o->get_selbox_y()  - py;
    const int x2 = x1           + i.o->get_selbox_xl() - 1;
    const int y2 = y1           + i.o->get_selbox_yl() - 1;
    rect(map_frames.get_al_bitmap(), x1,   y1,   x2,   y2,   col);
    //rect(map_frames.get_al_bitmap(), x1+1, y1+1, x2-1, y2-1, col);
}



// Auswahlrahmen zeichnen
// Dieser Code liegt ggf. etwas redundant nochmal in Editor::find_check_frame.
void Editor::draw_selection_frame(const int col)
{
    const int& x1 = frame_draw_x1;
    const int& y1 = frame_draw_y1;
    const int& x2 = frame_draw_x2;
    const int& y2 = frame_draw_y2;
    const int  x3 = map.get_xl() + 1;
    const int  y3 = map.get_yl() + 1;
    if      (!frame_torus_x && !frame_torus_y) {
        draw_selection_frame_at(x1, y1, x2, y2, col);
    }
    else if ( frame_torus_x && !frame_torus_y) {
        draw_selection_frame_at(x2, y1, x3, y2, col);
        draw_selection_frame_at(-2, y1, x1, y2, col);
    }
    else if (!frame_torus_x &&  frame_torus_y) {
        draw_selection_frame_at(x1, y2, x2, y3, col);
        draw_selection_frame_at(x1, -2, x2, y1, col);
    }
    else if ( frame_torus_x &&  frame_torus_y) {
        draw_selection_frame_at(x2, y2, x3, y3, col);
        draw_selection_frame_at(-2, y2, x1, y3, col);
        draw_selection_frame_at(x2, -2, x3, y1, col);
        draw_selection_frame_at(-2, -2, x1, y1, col);
    }
}

void Editor::draw_selection_frame_at
(const int fdx1, const int fdy1, const int fdx2, const int fdy2, const int col)
{
    rect(map_frames.get_al_bitmap(), fdx1,   fdy1,   fdx2,   fdy2,   col);
    //rect(map_frames.get_al_bitmap(), fdx1+1, fdy1+1, fdx2-1, fdy2-1, col);
}



// Wird von der Haupt-draw-Funktion fuer Ausgaenge und Klappen aufgerufen
void Editor::draw_object_with_numbers
(const GraIt& i, const int nr, const int max)
{
    const bool& tx  = level.torus_x;
    const bool& ty  = level.torus_y;
    const int&  txl = map.get_xl();
    const int&  tyl = map.get_yl();
                  draw_object_with_numbers_at(i, nr, max, 0,   0  );
    if (tx)       draw_object_with_numbers_at(i, nr, max, txl, 0  );
    if (ty)       draw_object_with_numbers_at(i, nr, max, 0,   tyl);
    if (tx && ty) draw_object_with_numbers_at(i, nr, max, txl, tyl);
}

void Editor::draw_object_with_numbers_at
(const GraIt& i, const int nr, const int max, const int px, const int py)
{
    std::ostringstream s;
    s << nr << "/" << max;
    Help::draw_shadow_centered_text(map_frames, font_med, s.str().c_str(),
     i->get_x() - px + i->get_xl()/2,
     i->get_y() - py, color[COL_TEXT], color[COL_API_SHADOW]);
}


// Diese Funktionen waren mir zu umfangreich fuer direktes Einfuegen oben
