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

    if (frame_draw_x1 != -1) draw_selection_frame(color[COL_BLACK]);
}



// ############################################################################
// ############################################################################
// ############################################################################



void Editor::draw()
{
    // Draw the map
    // If drawing takes very long, wait until after dragging.
    if (draw_required && (draw_dragging
     || (!Hardware::get_key_hold(useR->key_ed_up)
      && !Hardware::get_key_hold(useR->key_ed_right)
      && !Hardware::get_key_hold(useR->key_ed_down)
      && !Hardware::get_key_hold(useR->key_ed_left)
      && !Hardware::get_mlh()) ) ) {
        draw_required = false;
        int clock     = Help::get_timer_ticks();
        // clear_screen_rectangle() is not enough to prevent drag remainders
        map.clear_to_color(color[COL_PINK]);
        for (int type = Object::TERRAIN; type != Object::MAX; ++type)
         for (GraIt i =  object[Object::perm(type)].begin();
                    i != object[Object::perm(type)].end(); ++i) i->draw();
        clock = Help::get_timer_ticks() - clock;
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
        al_set_target_bitmap(Api::Manager::get_torbit().get_al_bitmap());
        al_draw_filled_rectangle(0, LEMSCR_Y - 80, LEMSCR_X, LEMSCR_Y - 60,
         color[COL_PINKAF]);
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
                str += al_keycode_to_name(key);
                str += "]";
            }
            Help::draw_shadow_text(Api::Manager::get_torbit(), font_med,
             str.c_str(), 3, LEMSCR_Y - 80, color[COL_TEXT]);
        }
    }



    // Mouse cursor on top
    // The mouse positioning code is here as well, as Editor::calc_self()
    // is skipped whenever a sub-window is open.
    mouse_cursor.set_x(Hardware::get_mx()-mouse_cursor_offset);
    mouse_cursor.set_y(Hardware::get_my()-mouse_cursor_offset);
    mouse_cursor.draw();

    pre_screen->clear_to_color(color[COL_BLACK]);

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
    unsigned t = ticks;
    int col = t%20;
    if (t%40 >= 20) col = 20 - (t%20);
    const int c1 = 100 - col*2;
    const int c2 = 255 - col*4;
    const ALLEGRO_COLOR col_hov = al_map_rgb(c1, c1, c1);
    const ALLEGRO_COLOR col_sel = al_map_rgb(c2, c2, c2);

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
    if (panel[SELECT_FRAME].get_on() && Hardware::get_mlh()
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











void Editor::draw_selection_border(const Selection& i,
                                   const ALLEGRO_COLOR& col)
{
    const bool& tx = level.torus_x;
    const bool& ty = level.torus_y;
    al_set_target_bitmap(map_frames.get_al_bitmap());
                  draw_selection_border_at(i, col, 0,            0           );
    if (tx)       draw_selection_border_at(i, col, map.get_xl(), 0           );
    if (ty)       draw_selection_border_at(i, col, 0,            map.get_yl());
    if (tx && ty) draw_selection_border_at(i, col, map.get_xl(), map.get_yl());
}

void Editor::draw_selection_border_at(
    const Selection& i,
    const ALLEGRO_COLOR& col,
    const int px,
    const int py
) {
    // this assumes a correctly set target bitmap!
    const int x1 = i.o->get_x() + i.o->get_selbox_x()  - px;
    const int y1 = i.o->get_y() + i.o->get_selbox_y()  - py;
    const int x2 = x1           + i.o->get_selbox_xl();
    const int y2 = y1           + i.o->get_selbox_yl();
    al_draw_rectangle(x1 + 0.5, y1 + 0.5, x2 - 0.5, y2 - 0.5, col, 1);
}



// Auswahlrahmen zeichnen
// Dieser Code liegt ggf. etwas redundant nochmal in Editor::find_check_frame.
void Editor::draw_selection_frame(const ALLEGRO_COLOR& col)
{
    const int& x1 = frame_draw_x1;
    const int& y1 = frame_draw_y1;
    const int& x2 = frame_draw_x2;
    const int& y2 = frame_draw_y2;
    const int  x3 = map.get_xl() + 1;
    const int  y3 = map.get_yl() + 1;
    al_set_target_bitmap(map_frames.get_al_bitmap());
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

void Editor::draw_selection_frame_at(
    const int fdx1,
    const int fdy1,
    const int fdx2,
    const int fdy2,
    const ALLEGRO_COLOR& col
) {
    // assumes correctly set target_bitmap!
    al_draw_rectangle(fdx1 + 0.5, fdy1 + 0.5, fdx2 - 0.5, fdy2 - 0.5, col, 1);
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
     i->get_y() - py, color[COL_TEXT]);
}


// Diese Funktionen waren mir zu umfangreich fuer direktes Einfuegen oben
