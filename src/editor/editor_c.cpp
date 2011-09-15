/*
 * editor/editor_c.cpp
 *
 * Die Hauptfunktion des Editors,
 * void Editor::calc_without_windows()
 *
 * 1. Maus und Koordinaten
 * 2. Hoverobjekt bestimmen
 * 3. Objekt waehlen und bewegen
 * 4. Buttons und Tasten pruefen, Fenster erschaffen
 *
 */

#include "editor.h"

#include "../api/manager.h"
#include "../other/user.h"

void Editor::calc_self()
{
    // Aendert die Mauskoordinaten ggf., also frueh aufrufen!
    map.calc_scrolling();

    // Wo ist die Maus auf dem Land?
    mx = map.get_mouse_x();
    my = map.get_mouse_y();

    const bool mouse_on_panel = hardware.get_my() > map.get_screen_yl();

    const int grid    = get_grid();
    const int mx_grid = (mx + grid/2) - Help::mod(mx, grid);
    const int my_grid = (my + grid/2) - Help::mod(my, grid);

    // Maus wird erst beim Zeichnen positioniert, damit auch die Unterfenster
    // davon profitieren, denn bei ihnen wird dieser main_loop uebersprungen.
    mouse_cursor.set_x_frame(0);
    mouse_cursor.set_y_frame(0);
    if (map.get_scrollable()
     && ((useR->scroll_right  && hardware.get_mrh())
      || (useR->scroll_middle && hardware.get_mmh())))
     mouse_cursor.set_x_frame(3);

    hover.clear();












    // Hotkey ohne Button: Cycle around all the buttons, and every button off
    if (hardware.key_once(useR->key_ed_grid)) {
        if (!panel[GRID_2 ].get_on()
         && !panel[GRID_CUSTOM].get_on()
         && !panel[GRID_16].get_on()) panel[GRID_2].set_on();
        else if (panel[GRID_2].get_on()) {
            panel[GRID_2 ].set_off();
            panel[GRID_CUSTOM].set_on();
        }
        else if (panel[GRID_CUSTOM].get_on()) {
            panel[GRID_CUSTOM].set_off();
            panel[GRID_16].set_on();
        }
        else panel[GRID_16].set_off();
    }

    // Moving objects with the arrow keys. This doesn't have an OSD button.
    // Testing for key_hold is good for this: If the drawing of the level is
    // slow, lots of keypresses get stuck in the buffer and the object(s) will
    // move on despite you've let got. Only moving the object(s) when you
    // first press the key (one could write a new function in Hardware for
    // this, of course) is also bad if the drawing is quick, because
    // you'd have to repeatedly tap the key to move it a distance.
    if (hardware.key_once(useR->key_ed_up)
     && hardware.key_hold(useR->key_ed_up)) {
        for (SelIt i = selection.begin(); i != selection.end(); ++i)
         i->o->set_y(i->o->get_y() - grid);
        if (!selection.empty()) draw_required = true;
    }
    else if (hardware.key_once(useR->key_ed_right)
     &&      hardware.key_hold(useR->key_ed_right)) {
        for (SelIt i = selection.begin(); i != selection.end(); ++i)
         i->o->set_x(i->o->get_x() + grid);
        if (!selection.empty()) draw_required = true;
    }
    else if (hardware.key_once(useR->key_ed_down)
     &&      hardware.key_hold(useR->key_ed_down)) {
        for (SelIt i = selection.begin(); i != selection.end(); ++i)
         i->o->set_y(i->o->get_y() + grid);
        if (!selection.empty()) draw_required = true;
    }
    else if (hardware.key_once(useR->key_ed_left)
     &&      hardware.key_hold(useR->key_ed_left)) {
        for (SelIt i = selection.begin(); i != selection.end(); ++i)
         i->o->set_x(i->o->get_x() - grid);
        if (!selection.empty()) draw_required = true;
    }

    // Buttons mit Hotkeys, die aber nicht auf normale Weise damit
    // assoziiert sind, werden jetzt speziell behandelt.
    // Es gibt bloede Ueberschneidungen mit dem Druecken und Loslassen von
    // Shift waehrend einer Mauszieh-Aktion, daher beheben wir das hier
    // von vornherein und nicht unten im Maus-Code.
    if (hardware.key_hold(useR->key_ed_sel_frame) && ! hardware.get_mlh())
     panel[SELECT_FRAME].set_on();
    if (hardware.key_release(useR->key_ed_sel_frame) && ! hardware.get_mlh())
     panel[SELECT_FRAME].set_off();

    if (hardware.key_hold(useR->key_ed_sel_add))
     panel[SELECT_ADD].set_on();
    if (hardware.key_release(useR->key_ed_sel_add))
     panel[SELECT_ADD].set_off();

    // Das Eindruecken von Buttons sperren, solange etwas umhergezogen wird:
    // Ansonsten wird die For-Schleife durchlaufen.
    if ((!hardware.get_mlh() && !hardware.get_mlr())
     || !mouse_hold_started_outside_panel || !mouse_on_panel)
     // Steuerleisten-Buttons abfragen
     for (unsigned int i = 0; i < panel.size(); ++i) {
        Api::BitmapButton& b = panel[i];
        if (b.get_clicked()) switch (i) {
        case FILE_NEW:
        case FILE_EXIT:
            save_lists();
            if (Level(useR->single_last_level) != level) {
                box_unsaved_data = new_box_unsaved_data(level);
                Api::Manager::add_focus(box_unsaved_data);
                b.set_on();
            }
            else if (i == FILE_NEW) delete_everything();
            else                    exit = true;
            break;

        case FILE_SAVE:
            if (useR->single_last_level.get_file().empty()) {
                // same as FILE_SAVE_AS
                b.set_on();
                browser_save = new Api::SaveBrowser(gloB->dir_levels,
                               gloB->ext_level,
                               useR->single_last_level,
                               Api::SaveBrowser::search_criterion_level,
                               Api::SaveBrowser::new_box_overwrite_level);
                browser_save->set_info_level_name(level.get_name());
                browser_save->set_info_filename(useR->single_last_level);
                Api::Manager::add_focus(browser_save);
            }
            else save_lists_and_level();
            break;

        case FILE_SAVE_AS:
            b.set_on();
            browser_save = new Api::SaveBrowser(gloB->dir_levels,
                               gloB->ext_level,
                               useR->single_last_level,
                               Api::SaveBrowser::search_criterion_level,
                               Api::SaveBrowser::new_box_overwrite_level);
            browser_save->set_info_level_name(level.get_name());
            browser_save->set_info_filename(useR->single_last_level);
            Api::Manager::add_focus(browser_save);
            break;

        case GRID_2:
        case GRID_CUSTOM:
        case GRID_16:
            {
                bool was_on = b.get_on();
                panel[GRID_2 ].set_off();
                panel[GRID_CUSTOM].set_off();
                panel[GRID_16].set_off();
                panel[i].set_on(!was_on);
            }
            break;

        case SELECT_ALL:
            selection.clear();
            for  (int type = Object::TERRAIN; type != Object::MAX; ++type)
             for (GraIt i = object[type].begin(); i != object[type].end(); ++i)
             selection.push_back(Selection(&object[type], i));
            break;

        case SELECT_FRAME:
        case SELECT_ADD:
            b.set_on(!b.get_on()); // Der Maus-Code kontrolliert, ob diese
            break;                 // Buttons eingeschaltet sind

        case SELECT_COPY:
            for (SelIt i = selection.begin(); i != selection.end(); ++i) {
                int offset = grid;
                while (offset < 10) offset *= 2;
                i->l->push_back(*i->o); // An einem Listenende anhaengen
                i->o = --i->l->end();   // Neue Position fuer den Auswahliter.
                i->o->set_x(i->o->get_x() + offset);
                i->o->set_y(i->o->get_y() + offset);
            }
            if (!selection.empty()) draw_required = true;
            break;

        case SELECT_DELETE:
            for (SelIt i = selection.begin(); i != selection.end(); ++i) {
                i->l->erase(i->o);
            }
            if (!selection.empty()) draw_required = true;
            selection.clear();
            break;

        case SELECT_MINUS: selection_fore_background(true,  false); break;
        case SELECT_PLUS:  selection_fore_background(false, false); break;
        case SELECT_BACK:  selection_fore_background(true,  true ); break;
        case SELECT_FRONT: selection_fore_background(false, true ); break;

        case SELECT_FLIP:
            flip_selection();
            if (!selection.empty()) draw_required = true;
            break;

        case SELECT_ROTATE:
            rotate_selection();
            if (!selection.empty()) draw_required = true;
            break;

        case SELECT_DARK:
            for (SelIt i = selection.begin(); i != selection.end(); ++i)
             if (i->l == &object[Object::TERRAIN])
             i->o->set_mode(i->o->get_mode() == Cutbit::DARK_EDITOR
              ? Cutbit::NORMAL : Cutbit::DARK_EDITOR);
            if (!selection.empty()) draw_required = true;
            break;

        case SELECT_NOOW:
            for (SelIt i = selection.begin(); i != selection.end(); ++i)
             if (i->l == &object[Object::TERRAIN])
             i->o->set_mode(i->o->get_mode() == Cutbit::NOOW_EDITOR
              ? Cutbit::NORMAL : Cutbit::NOOW_EDITOR);
            if (!selection.empty()) draw_required = true;
            break;

        case VIEW_ZOOM:
            b.set_on(!b.get_on());
            map.set_zoom(b.get_on());
            break;

        case MENU_SIZE:
            b.set_on();
            window_size     = new Api::WindowSize(level, map);
            Api::Manager::add_focus(window_size);
            break;

        case MENU_SCROLL:
            b.set_on();
            window_scroll   = new Api::WindowScroll(level, map);
            Api::Manager::add_focus(window_scroll);
            break;

        case MENU_VARS:
            b.set_on();
            window_variable = new Api::WindowVariables(level);
            Api::Manager::add_focus(window_variable);
            break;

        case MENU_SKILL:
            b.set_on();
            window_skill    = new Api::WindowSkill(level);
            Api::Manager::add_focus(window_skill);
            break;

        case HELP:
            b.set_on(!b.get_on());
            break;

        case ADD_TERRAIN:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_terrain,
                             useR->editor_last_dir_terrain,
                             Language::add_terrain,
                             browser_terrain_last_page);
            Api::Manager::add_focus(browser_bitmap);
            break;

        case ADD_STEEL:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_steel,
                             useR->editor_last_dir_steel,
                             Language::add_steel);
            Api::Manager::add_focus(browser_bitmap);
            break;

        case ADD_HATCH:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_hatch,
                             useR->editor_last_dir_hatch,
                             Language::add_hatch);
            Api::Manager::add_focus(browser_bitmap);
            break;

        case ADD_GOAL:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_goal,
                             useR->editor_last_dir_goal,
                             Language::add_goal);
            Api::Manager::add_focus(browser_bitmap);
            break;

        case ADD_DECO:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_deco,
                             useR->editor_last_dir_deco,
                             Language::add_deco);
            Api::Manager::add_focus(browser_bitmap);
            break;

        case ADD_HAZARD:
            b.set_on();
            browser_bitmap = new Api::BitmapBrowser(
                             search_criterion_hazard,
                             useR->editor_last_dir_hazard,
                             Language::add_hazard);
            Api::Manager::add_focus(browser_bitmap);
            break;

        default:
            break;
        }
    }













    // Dies verhindert Umherziehen und dergleichen, wenn das Maushalten
    // nicht im Spielfeld mit Objektwahl o. Ae. begann
    if (hardware.get_ml() && mouse_on_panel) {
        mouse_hold_started_outside_panel = false;
    }

    // Hoverobjekt nicht beim Umherziehen waehlen
    if (!mouse_on_panel && hardware.get_mlh() < 2) {
        Selection s = find_under_mouse_cursor();
        if (s.is_valid() && !panel[SELECT_FRAME].get_on()) hover.push_back(s);
    }

    // Vielleicht doch klicken und Rahmen ziehen? Wenn man ins Nirvana
    // klickt, dann noch schnell den Button aktivieren
    if (!mouse_on_panel && !panel[SELECT_FRAME].get_on()
     && hardware.get_ml() && hover.empty()) {
        panel[SELECT_FRAME].set_on();
    }

    // Mehrere grosse Moeglichkeiten zur Auswahl
    // Moeglichkeit: Rahmen ziehen
    if (panel[SELECT_FRAME].get_on()) {
        // Rahmenziehen beginnt
        if (hardware.get_ml() && !mouse_on_panel) {
            mouse_hold_started_outside_panel = true;
            frame_start_x  = mx;
            frame_start_y  = my;
        }
        // Beim Rahmenziehen Objekte markieren, auch beim Loslassen diesen
        // Code nutzen, denn beim Loslassen wird einfach alles aus der
        // Hover-Liste in die Auswahl uebernommen!
        if ((hardware.get_mlh() || hardware.get_mlr())
         && mouse_hold_started_outside_panel) {
            int x1 = (frame_start_x < mx) ? frame_start_x : mx;
            int y1 = (frame_start_y < my) ? frame_start_y : my;
            int x2 = (frame_start_x > mx) ? frame_start_x : mx;
            int y2 = (frame_start_y > my) ? frame_start_y : my;
//            // Rahmen ueber dem Panel lassen, gaebe beim Torus sonst Kaese
//            if (hardware.is_mouse_over_panel()) {
//                y1 = frame_start_y;
//                y2 = map.get_screen_y()
//                   + (map.get_zoom() ? Level::min_y*3/4 : Level::min_y) - 2;
//                if (level.torus_y) y2 = Help::mod(y2, map.get_yl());
//                if (y1 < y2) std::swap(y1, y2);
//            }
            // Bei Toruskarten sollte der Rahmen so gezogen werden, dass er
            // nur im sichtbaren Bereich verlaeuft. Dieser Test funktioniert
            // auch, wenn der Zoom an ist!
            if (level.torus_x && x1 < map.get_screen_x()
                              && x2 > map.get_screen_x()) frame_torus_x=true;
            else                                           frame_torus_x=false;
            if (level.torus_y && y1 < map.get_screen_y()
                              && y2 > map.get_screen_y()) frame_torus_y=true;
            else                                           frame_torus_y=false;
            // Stopfe Gefundenes in die Hoverliste
            for (int type = Object::TERRAIN; type != Object::MAX; ++type)
             find_check_frame(object[Object::perm(type)], x1, y1, x2, y2);
            frame_draw_x1 = x1;
            frame_draw_y1 = y1;
            frame_draw_x2 = x2;
            frame_draw_y2 = y2;
            // Beim Loslassen alles aus der Hover-Liste in die Auswahl setzen
            if (hardware.get_mlr()) {
                if (!hardware.get_shift()) panel[SELECT_FRAME].set_off();
                // Kein CTRL gedrueckt - genau alles im Hover wird zur Auswahl
                if (!panel[SELECT_ADD].get_on()) selection.clear();
                for (SelIt i = hover.begin(); i != hover.end(); ++i) {
                    selection.push_back(*i);
                }
                // Radikal auf Dubletten ueberpruefen - zuvor hatte ich hier
                // etwas schlankeren Code stehen fuer CTRL-Auswahl, mit Ein-
                // fuehrung der Toruskarten kommen wir glaube ich nicht mehr
                // unter der O(n^2)-Loesung weg.
                // Kommen wir doch, die CTRL-Loesung von damals sollte reichen.
                // Mache ich aber (noch) nicht, kostet bisher noch nix.
                delete_dublicates_from_selection();
            }
            // Fertig mit dem Uebertragen in die Auswahl
        }
        // Rahmen fertig gezogen
    }



    // Weitere grosse Moeglichkeit:
    // Keinen Rahmen ziehen
    else {
        // Neuauswahl der Grafikobjekte
        if (hardware.get_ml() && !mouse_on_panel) {
            mouse_hold_started_outside_panel = true;
            // Wir haben hier garantiert etwas angeklickt, denn sonst
            // haette eine Abfrage oben bereits den SELECT_FRAME-Button
            // aktiviert.
            // Soll zur Auswahl hinzugefuegt werden (CTRL/entspr. Button)?
            // Falls das Element bereits in der Auswahl ist, wird es
            // wieder hieraus entfernt.
            if (panel[SELECT_ADD].get_on()) {
                bool is_already_in_selection = false;
                for (SelIt i = selection.begin(); i!=selection.end(); ++i){
                    if (*i == *hover.begin()) {
                        selection.erase(i);
                        is_already_in_selection = true;
                        hover.clear(); // Verhindert dunklen Rahmen, weil im
                                       // naechsten Tick umhergezogen wird
                        break;
                    }
                }
                if (!is_already_in_selection) {
                    selection.push_back(*hover.begin());
                }
            }
            // Nicht CTRL gedrueckt:
            // - Wenn in eine Auswahl geklickt, passiert nichts, denn man
            //   will ja moeglicherweise umherziehen
            // - Wenn noch nicht ausgewaehlt, dies als neue Auswahl nehmen
            else {
                bool is_already_in_selection = false;
                for (SelIt i = selection.begin(); i!=selection.end(); ++i){
                    if (*i == *hover.begin()) {
                        is_already_in_selection = true;
                        break;
                    }
                }
                if (!is_already_in_selection) {
                    selection.clear();
                    selection.push_back(*hover.begin());
                }
            }
            // Andere Dinge bei Mausklick
            mx_grid_last = mx_grid;
            my_grid_last = my_grid;
        }







        // Umherziehen von Objekten mit der Maus
        else if (hardware.get_mlh() && mouse_hold_started_outside_panel) {
            if (mx_grid != mx_grid_last || my_grid != my_grid_last) {
                for (SelIt i = selection.begin(); i != selection.end(); ++i) {
                    const int iox = i->o->get_x() + grid/2;
                    const int ioy = i->o->get_y() + grid/2;
                    i->o->set_x(
                        iox - Help::mod(iox, grid) - mx_grid_last + mx_grid
                    );
                    i->o->set_y(
                        ioy - Help::mod(ioy, grid) - my_grid_last + my_grid
                    );
                }
                if (!selection.empty()) draw_required = true;
            }
            mx_grid_last = mx_grid;
            my_grid_last = my_grid;

            if (selection.size() > 0 && mouse_on_panel) {
                mouse_cursor.set_y_frame(2);
            }
        }

        // Loslassen von umhergezogenen Objekten - nur wichtig fuer's
        // Wegschmeissen in das ewige Nirwana am unteren Bildschirmrand :-D
        else if (hardware.get_mlr() && mouse_hold_started_outside_panel) {
            if (mouse_on_panel) {
                // Komplette Auswahl loeschen
                for (SelIt i = selection.begin(); i != selection.end(); ++i) {
                    i->l->erase(i->o);
                }
                if (!selection.empty()) draw_required = true;
                selection.clear();
            }
        }
        // Ende Loslassen

    }
    // Ende der grossen Moeglichkeit fuer "Keinen Rahmen ziehen"


}
// Ende von main_loop_calc()
