#include <algorithm>

#include "list_fil.h"

#include "../other/help.h"
#include "../other/language.h"
#include "../other/user.h" // hotkeys for movement in the list

namespace Api {

ListFile::ListFile(
    const int x,  const int y,
    const int xl, const int yl,
    const SearchCrit crit)
:
    Frame(x, y, xl, yl),
    page               (0),
    bottom_button      (yl/20 - 1),
    file_number_at_top (0),
    bottom_button_flips_page(false),
    activate_clicked_button (false),
    clicked                 (false),
    button_last_clicked(0),
    current_dir        (""),
    current_file       (""),
    search_criterion   (crit)
{
    set_undraw_color(color[COL_API_M]);
}

ListFile::~ListFile()
{
    button_clear();
}









void ListFile::static_put_to_file(const Filename& fn, void* which_object)
{
    ListFile* this_object = (ListFile*) which_object;
    if (this_object) {
        if (!this_object->search_criterion
         || this_object->search_criterion(fn)) {
            this_object->file.push_back(fn);
        }
    }
}



void ListFile::load_dir(const Filename& s, const int which_page)
{
    if (current_dir.get_dir_rootful() != s.get_dir_rootful()) {
        current_dir = Filename(s.get_dir_rootful());
        page = which_page;
    }
    load_current_dir();
}



void ListFile::button_clear()
{
    set_draw_required();
    for (unsigned i = 0; i != button.size(); ++i) {
        delete button[i];
        remove_child(*button[i]);
    }
    button.clear();
}

void ListFile::button_push_back(Button* b)
{
    b->set_undraw_color(color[COL_API_M]);
    button.push_back(b);
    add_child(*b);
}

void ListFile::load_current_dir()
{
    set_draw_required();
    bottom_button_flips_page = false;
    file.clear();
    button_clear();
    button_last_clicked = 0;

    Help::find_files(current_dir, gloB->mask_anything,
     static_put_to_file, (void*) this);
    std::sort(file.begin(), file.end());
    // Hook/event function: derived classes may alter file via overriding
    // the empty on_dir_load() and calls to add_to_file_list().
    on_dir_load();

    // Verzeichnisbuttons erstellen
    if (page * bottom_button >= (int) file.size()) page = 0;
    file_number_at_top = page * bottom_button;
    // Die folgende While-Schleife macht, dass bei mehreren Seiten immer
    // alle Seiten voll gefuellt sind. Die letzte Seite wird dann ggf. oben
    // mit Eintraegen der vorletzten Seite aufgefuellt.
    while (page > 0 && file_number_at_top + bottom_button > (int) file.size())
     --file_number_at_top;

    int next_from_file = file_number_at_top;
    for (int i = 0; i < bottom_button
     && next_from_file < (int) file.size(); ++i) {
        add_file_button(i, next_from_file);
        ++next_from_file;
    }
    // Blaetter-Button anhaengen, es sei denn, es geht genau auf
    if (next_from_file == (int) file.size() - 1 && page == 0) {
        add_file_button(bottom_button, next_from_file);
        ++next_from_file;
    // Das hier ist der Blaetter-Button
    } else if (next_from_file < (int) file.size() || page > 0) {
        add_flip_button();
        bottom_button_flips_page = true;
    }

    // Eventuell Highlight-Button anklicken und Zeiger darauf setzen
    if (current_dir.get_dir_rootful() == current_file.get_dir_rootful())
     for (int i = 0; i < (int) button.size(); ++i) {
        if (i != bottom_button || !bottom_button_flips_page) {
            if (current_file == file[file_number_at_top + i]) {
                button_last_clicked = button[i];
                button_last_clicked->set_on();
    }   }   }
}



void ListFile::highlight_iterator(const std::vector <Filename> ::iterator& itr)
{
    if (itr != file.end()) {
        // If not on the current page, swap the page
        if (bottom_button_flips_page) {
            if (itr - file.begin() <  file_number_at_top
             || itr - file.begin() >= file_number_at_top + bottom_button) {
                page = (itr - file.begin()) / bottom_button;
                load_current_dir();
            }
        }
        current_file = *itr;

        // Highlight-Button anklicken und Zeiger darauf setzen
        Button* but = button[itr - file.begin() - file_number_at_top];
        if (activate_clicked_button) {
            if (button_last_clicked == but) {
                but->set_on(!but->get_on());
            }
            else if (button_last_clicked) {
                button_last_clicked->set_off();
                but->set_on();
            }
            else {
                but->set_on();
            }
        }
        button_last_clicked = but;
    }
    else {
        // file to be highlighted is not in the directory
        current_file = current_dir;
        button_last_clicked = 0;
    }
}



void ListFile::highlight_file(const Filename& filename)
{
    std::vector <Filename> ::iterator itr;

    // Suchen, ob diese Datei wirklich im Verzeichnis ist, sonst passiert nix
    for (itr = file.begin(); itr != file.end(); ++itr) {
        if (*itr == filename) break;
    }
    highlight_iterator(itr);
}



void ListFile::highlight_move(const int by)
{
    if (by == 0) return;
    if (file.empty()) return;

    // Do we have a valid highlight right now?
    std::vector <Filename> ::iterator itr = file.begin();
    while (itr != file.end() && *itr != current_file) ++itr;

    if (itr != file.end()) {
        // if first file and by < 0, select last one.
        // if last file  and by > 0, select first one.
        if      (itr == file.begin() && by < 0) itr = --file.end();
        else if (itr == --file.end() && by > 0) itr = file.begin();
        else {
            // If not first or last file, move by the given number of steps,
            // but stop on the first/last entries.
            int by_left = by;
            do {
                if (by > 0) { ++itr; --by_left; }
                else        { --itr; ++by_left; }
            } while (by_left != 0
                  && itr != file.begin() && itr !=--file.end());
        }
    }
    // If none of the files were highlighted, start on the bottom or top.
    // Do not move yet.
    else {
        if (by < 0) itr = --file.end();
        else        itr = file.begin();
    }
    highlight_iterator(itr);
}



void ListFile::calc_self()
{
    clicked = false;
    for (int i = 0; i < (int) button.size(); ++i) {
        if (button[i]->get_clicked()) {
            // Seitenwechsel-Button angeklickt?
            if (i == bottom_button && bottom_button_flips_page){
                ++page;
                if (page * bottom_button >= (int) file.size()) page = 0;
                load_current_dir();
                clicked = false;
                break;
            }
            // Ansonsten den Dateinamen nach aussen hin kenntlich machen
            else {
                highlight_iterator(file.begin() + file_number_at_top + i);
                clicked = true;
                break;
            }
        } // Ende von Button angeklickt
    } // Ende der For-Schleife, die alle Buttons durchlaeuft

    if (activate_clicked_button && ! button.empty()) {
        bool any_movement_with_keys = true;
        if      (hardware.key_once(useR->key_me_up_1))   highlight_move(-1);
        else if (hardware.key_once(useR->key_me_up_5))   highlight_move(-5);
        else if (hardware.key_once(useR->key_me_down_1)) highlight_move(1);
        else if (hardware.key_once(useR->key_me_down_5)) highlight_move(5);
        else any_movement_with_keys = false;
        if (any_movement_with_keys) clicked = true;
    }
}



void ListFile::draw_self()
{
    undraw_self();
    Frame::draw_self();
}

}
// Ende Namensraum Api
