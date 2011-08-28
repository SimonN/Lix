/*
 * editor.cpp
 *
 */

#include "../other/myalleg.h"
#include <fstream>

#include "editor.h"

#include "../api/manager.h" // mouse ground
#include "../graphic/glob_gfx.h"
#include "../graphic/gra_lib.h"
#include "../level/obj_lib.h"
#include "../other/hardware.h"
#include "../other/help.h"
#include "../other/log.h"
#include "../other/user.h"

Editor::Editor()
:
    exit(false),
    timer_ticks_offset(Help::timer_ticks - 1), // - 1 prevents mouse flicker
    ticks(0),
    mouse_cursor_offset(15),
    mouse_cursor(GraLib::get(gloB->file_bitmap_mouse), Api::Manager::get_torbit()),

    level     (useR->single_last_level),
    bg_color  (makecol(level.bg_red,
                       level.bg_green,
                       level.bg_blue)),
    map       (level.size_x, level.size_y, LEMSCR_X, LEMSCR_Y - 60),
    map_frames(level.size_x, level.size_y, LEMSCR_X, LEMSCR_Y - 60),

    object(Object::MAX),

    frame_start_x(-1),
    frame_start_y(-1),
    draw_required(true),
    browser_terrain_last_page(0)
{
    map.set_screen_x(level.start_x);
    map.set_screen_y(level.start_y);
    map.set_torus_x(level.torus_x);
    map.set_torus_y(level.torus_y);

    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (Level::PosIt itr =  level.pos[type].begin();
                       itr != level.pos[type].end(); ++itr)
     if (itr->ob && itr->ob->cb) {
        object[type].push_back(EdGraphic(map, itr->ob, itr->x, itr->y));
        EdGraphic& eg = *object[type].rbegin();
        eg.set_draw_info();
        if (itr->ob->type == Object::TERRAIN) {
            if      (itr->dark) eg.set_mode(Cutbit::DARK_EDITOR);
            else if (itr->noow) eg.set_mode(Cutbit::NOOW_EDITOR);
            eg                    .set_mirror  (itr->mirr);
            eg                    .set_rotation(itr->rot );
        }
        else if (itr->ob->type == Object::HATCH) {
            eg.set_x_frame(eg.get_x_frames() - 1);
            eg.set_rotation(itr->rot);
        }
        else if (itr->ob->type == Object::TRAP) {
            eg.set_x_frame(1);
        }
    }

    // Panel vorbereiten
    for (unsigned int i = 0; i < MAX; ++i) {
        panel.push_back(Api::BitmapButton(
         GraLib::get(gloB->file_bitmap_edit_panel)));
        panel[i].set_x(i/2*40);
        panel[i].set_y(420 + i%2*30);
        panel[i].set_x_frame(i);
    }
    panel[FILE_NEW]     .set_hotkey();
    panel[FILE_EXIT]    .set_hotkey(KEY_ESC);
    panel[FILE_SAVE]    .set_hotkey();
    panel[FILE_SAVE_AS] .set_hotkey();
    panel[GRID_2]       .set_hotkey(); // will be done later
    panel[GRID_CUSTOM]  .set_hotkey();
    panel[GRID_16]      .set_hotkey();
    panel[SELECT_ALL]   .set_hotkey(useR->key_ed_sel_all);
    panel[SELECT_FRAME] .set_hotkey(); // done later
    panel[SELECT_ADD]   .set_hotkey(); // done later
    panel[SELECT_COPY]  .set_hotkey(useR->key_ed_copy);
    panel[SELECT_DELETE].set_hotkey(useR->key_ed_delete);
    panel[SELECT_MINUS] .set_hotkey(useR->key_ed_background);
    panel[SELECT_PLUS]  .set_hotkey(useR->key_ed_foreground);
    panel[SELECT_FLIP]  .set_hotkey(useR->key_ed_mirror);
    panel[SELECT_ROTATE].set_hotkey(useR->key_ed_rotate);
    panel[SELECT_DARK]  .set_hotkey(useR->key_ed_dark);
    panel[SELECT_NOOW]  .set_hotkey(useR->key_ed_noow);
    panel[VIEW_ZOOM]    .set_hotkey(useR->key_ed_zoom);
    panel[HELP]         .set_hotkey(useR->key_ed_help);
    panel[MENU_SIZE]    .set_hotkey(useR->key_ed_menu_size);
    panel[MENU_SCROLL]  .set_hotkey();
    panel[MENU_VARS]    .set_hotkey(useR->key_ed_menu_vars);
    panel[MENU_SKILL]   .set_hotkey(useR->key_ed_menu_skills);

    panel[ADD_TERRAIN].set_hotkey(useR->key_ed_add_terrain);
    panel[ADD_STEEL]  .set_hotkey(useR->key_ed_add_steel);
    panel[ADD_HATCH]  .set_hotkey(useR->key_ed_add_hatch);
    panel[ADD_GOAL]   .set_hotkey(useR->key_ed_add_goal);
    panel[ADD_DECO]   .set_hotkey(useR->key_ed_add_deco);
    panel[ADD_HAZARD] .set_hotkey(useR->key_ed_add_hazard);

    for (unsigned int i = 0; i < MAX; ++i) Api::Manager::add_elder(&panel[i]);

    // Use the specified grid.
    if      (useR->editor_grid_selected ==  2) panel[GRID_2].set_on();
    else if (useR->editor_grid_selected == 16) panel[GRID_16].set_on();
    else if (useR->editor_grid_selected ==  0) panel[GRID_CUSTOM].set_on();
    else {
        // Use grid of 1, and set user variable to avoid ambiguity.
        // If an original level is loaded, use appropriate grid instead of 1
        useR->editor_grid_selected = 1;
        if (useR->single_last_level.get_extension() == gloB->ext_level_orig)
            panel[GRID_2].set_on();
    }
    if (useR->editor_grid_custom < 1) useR->editor_grid_custom = 8;

    browser_save     = 0;
    browser_bitmap   = 0;
    window_size      = 0;
    window_scroll    = 0;
    window_variable  = 0;
    window_skill     = 0;
    box_unsaved_data = 0;
}

Editor::~Editor()
{
    useR->editor_grid_selected = get_grid();
    if (useR->editor_grid_selected == useR->editor_grid_custom)
        useR->editor_grid_selected = 0;

    selection.clear();

    if (browser_save)     delete browser_save;
    if (browser_bitmap)   delete browser_bitmap;
    if (window_size)      delete window_size;
    if (window_scroll)    delete window_scroll;
    if (window_variable)  delete window_variable;
    if (window_skill)     delete window_skill;
    if (box_unsaved_data) delete box_unsaved_data;
}



// ############################################################################
// ############################################################################
// ############################################################################



void Editor::calc() {

    // Frameskipping koennte den Block umschliessen, aber es macht nichts,
    // wenn der Editor mit bestmoeglicher Geschwindigkeit laeuft
    ticks++;

    if (browser_save) {
        if (browser_save->get_exit()) {
            if (browser_save->get_exit_with()) {
                useR->single_last_level
                 = Filename(browser_save->get_current_file());
                save_lists_and_level();
                // Dies ist wichtig, falls der Speichern-Dialog vom Code, der
                // die Exit-Box behandelt, aufgerufen wurde
                if (panel[FILE_EXIT].get_on()) exit = true;
            }
            delete browser_save;
            browser_save = 0;
            panel[FILE_NEW]    .set_off();
            panel[FILE_EXIT]   .set_off();
            panel[FILE_SAVE]   .set_off();
            panel[FILE_SAVE_AS].set_off();
        }
    }

    else if (browser_bitmap) {
        for (int p = ADD_TERRAIN; p != MAX; ++p)
         if (panel[p].get_on()) {
            calc_bitmap_browser(
               p == ADD_TERRAIN ? useR->editor_last_dir_terrain
             : p == ADD_STEEL   ? useR->editor_last_dir_steel
             : p == ADD_HATCH   ? useR->editor_last_dir_hatch
             : p == ADD_GOAL    ? useR->editor_last_dir_goal
             : p == ADD_DECO    ? useR->editor_last_dir_deco
             : p == ADD_HAZARD  ? useR->editor_last_dir_hazard
             :                    useR->editor_last_dir_terrain);
            if (!browser_bitmap) panel[p].set_off();
            break;
        }
    }

    else if (window_size) {
        if (window_size->get_exit()) {
            int x = window_size->get_exit_with_x_to_adjust();
            int y = window_size->get_exit_with_y_to_adjust();

            for  (int type = Object::TERRAIN; type != Object::MAX; ++type)
             for (GraIt i = object[type].begin(); i != object[type].end();++i){
                i->set_ground(map);
                if (x != 0) i->set_x(i->get_x() + x);
                if (y != 0) i->set_y(i->get_y() + y);
            }
            delete window_size;
            window_size = 0;
            panel[MENU_SIZE].set_off();
            draw_required = true; // if we only did this for x != 0 || y != 0,
                                  // altering only the right and bottom
                                  // margins would not trigger a redraw.
        }
    } else if (window_scroll) {
        if (window_scroll->get_exit()) {
            delete window_scroll;
            window_scroll = 0;
            panel[MENU_SCROLL].set_off();
            bg_color = makecol(level.bg_red,
                               level.bg_green,
                               level.bg_blue);
            draw_required = true;
        }
    } else if (window_variable) {
        if (window_variable->get_exit()) {
            delete window_variable;
            window_variable = 0;
            panel[MENU_VARS].set_off();
        }
    } else if (window_skill) {
        if (window_skill->get_exit()) {
            delete window_skill;
            window_skill = 0;
            panel[MENU_SKILL].set_off();
        }
    }

    else if (box_unsaved_data) {
        unsigned i = box_unsaved_data->get_button_clicked();
        if (i == 1) {
            if (useR->single_last_level.get_file().empty()) {
                browser_save = new Api::SaveBrowser(
                 gloB->dir_levels,
                 gloB->ext_level,
                 useR->single_last_level,
                 Api::SaveBrowser::search_criterion_level,
                 Api::SaveBrowser::new_box_overwrite_level);
                browser_save->set_info_level_name(level.get_name());
                browser_save->set_info_filename  (useR->single_last_level);
                delete box_unsaved_data;
                box_unsaved_data = 0;
                // Anhand des Status von panel[FILE_EXIT] erkennt der Code
                // oben zum Speichern-unter-Dialog, ob anschliessend beendet
                // werden muss.
            }
            else {
                save_lists_and_level();
                delete box_unsaved_data;
                box_unsaved_data = 0;
                if (panel[FILE_NEW].get_on()) delete_everything();
                else                          exit = true;
                panel[FILE_NEW].set_off();
                panel[FILE_EXIT].set_off();
            }
        }
        else if (i > 1) {
            delete box_unsaved_data;
            box_unsaved_data = 0;
            if (i == 2) {
                if (panel[FILE_NEW].get_on()) delete_everything();
                else                          exit = true;
            }
            panel[FILE_NEW].set_off();
            panel[FILE_EXIT].set_off();
        }
    }

    else calc_self();
}


// Dies koennen verschiedene Browser sein
void Editor::calc_bitmap_browser(Filename& save_last_dir)
{
    Api::BitmapBrowser*& br = browser_bitmap;
    if (br->get_exit()) {
        const Object* ob = br->get_exit_with();
        GraLi* li = ob ? &object[ob->type] : 0;

        if (li && ob->cb) {
            li->push_back(EdGraphic(map, ob));
            GraIt a = --li->end();

            // Neues Terrain-Objekt mittig unter den Mauszeiger legen
            a->set_x(map.get_mouse_x() - a->get_xl()/2);
            a->set_y(map.get_mouse_y() - a->get_yl()/2);
            a->set_draw_info();
            if (ob->type == Object::HATCH) a->set_x_frame(a->get_x_frames()-1);
            else if (ob->type == Object::TRAP) a->set_x_frame(1);

            // Korrektur, falls nicht sichtbar, weil Maus zu hoch auf Karte
            // mit sehr kleiner yl
            if (!map.get_torus_y() && map.get_yl() < map.get_screen_yl()
             && a->get_y() < 0) a->set_y(0);

            selection.clear();

            Selection s;
            s.o = a;
            s.l = li;
            selection.push_back(s);
            draw_required = true;

            // As the original graphics sets have been scaled to fit L++'s
            // gameplay, use twice the current grid if it's not an even number.
            // If it's an L2 graphic, use the 16 x 16 grid. (That is 8 x 8
            // pixels in L2, and that game works with a strict 16 x 8 grid.)
            if (br->get_current_dir().get_dir_rootful().find(
             gloB->dir_bitmap_orig_l2.get_dir_rootful())
             != std::string::npos) {
                panel[GRID_2 ].set_off();
                panel[GRID_CUSTOM].set_off();
                panel[GRID_16].set_on();
            }
            else if (br->get_current_dir().get_dir_rootful().find(
                     gloB->dir_bitmap_orig.get_dir_rootful())
             != std::string::npos) {
                if (!panel[GRID_2 ].get_on()
                 && !panel[GRID_CUSTOM].get_on()
                 && !panel[GRID_16].get_on()) panel[GRID_2 ].set_on();
            }
            // We've already positioned the object under the mouse.
            // Move the object a few pixels so that it's aligned to the grid.
            const int grid = get_grid();
            a->set_x(a->get_x() + grid/2 - Help::mod(a->get_x()+grid/2,grid));
            a->set_y(a->get_y() + grid/2 - Help::mod(a->get_y()+grid/2,grid));
        }
        save_last_dir = Filename(br->get_current_dir());
        if (ob && ob->type == Object::TERRAIN && ob->subtype == 0) {
            browser_terrain_last_page = br->get_page();
        }
        delete (br);
        br = 0;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



void Editor::save_lists_and_level()
{
    save_lists();

    Level l(useR->single_last_level);
    if (level != l) {
        level.built = Date();
        // Always save in the L++ format, and also provide the correct
        // file extension
        if (useR->single_last_level.get_extension() != gloB->ext_level)
            useR->single_last_level = Filename(
                useR->single_last_level.get_rootless_no_extension()
                + gloB->ext_level);
        level.save_to_file(useR->single_last_level);
    }
}



void Editor::save_lists()
{
    for (int type = Object::TERRAIN; type != Object::MAX; ++type) {
        level.pos[type].clear();
        for (GraIt i = object[type].begin(); i != object[type].end(); ++i) {
            // Ausserhalb des sichtbaren Bereichs keine Objekte abspeichern
            if (i->get_x()               >= level.size_x
             || i->get_y()               >= level.size_y
             || i->get_x() + i->get_xl() <= 0
             || i->get_y() + i->get_yl() <= 0) continue;
            // Sonst abspeichern!
            Level::Pos temp;
            temp.ob   = i->get_object();
            temp.x    = i->get_x();
            temp.y    = i->get_y();
            temp.mirr = i->get_mirror();
            temp.rot  = (int) i->get_rotation();
            temp.dark = i->get_mode() == Cutbit::DARK_EDITOR;
            temp.noow = i->get_mode() == Cutbit::NOOW_EDITOR;
            level.pos[type].push_back(temp);
        }
    }
}
