#include <algorithm>

#include "list_fil.h"

#include "../other/help.h"
#include "../other/language.h"

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
    search_criterion   (crit)
{
    set_undraw_color(color[COL_API_M]);
}

ListFile::~ListFile()
{
    button_clear();
}









void ListFile::static_put_to_file(std::string& s, void* which_object)
{
    ListFile* this_object = (ListFile*) which_object;
    if (this_object) {
        if (!this_object->search_criterion
         || this_object->search_criterion(s)) {
            Help::string_remove_dir(s);
            this_object->file.push_back(s);
        }
    }
}



void ListFile::load_dir(const std::string& s, const int which_page)
{
    if (current_dir != s) {
        current_dir = s;
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
    if (page * bottom_button >= file.size()) page = 0;
    file_number_at_top = page * bottom_button;
    // Die folgende While-Schleife macht, dass bei mehreren Seiten immer
    // alle Seiten voll gefuellt sind. Die letzte Seite wird dann ggf. oben
    // mit Eintraegen der vorletzten Seite aufgefuellt.
    while (page > 0 && file_number_at_top + bottom_button > file.size())
     --file_number_at_top;

    unsigned int next_from_file = file_number_at_top;
    for (unsigned int i = 0; i < bottom_button
     && next_from_file < file.size(); ++i) {
        add_file_button(i, next_from_file);
        ++next_from_file;
    }
    // Blaetter-Button anhaengen, es sei denn, es geht genau auf
    if (next_from_file == file.size() - 1 && page == 0) {
        add_file_button(bottom_button, next_from_file);
        ++next_from_file;
    // Das hier ist der Blaetter-Button
    } else if (next_from_file < file.size() || page > 0) {
        add_flip_button();
        bottom_button_flips_page = true;
    }

    // Eventuell Highlight-Button anklicken und Zeiger darauf setzen
    if (current_dir == current_file_dir)
     for (unsigned i = 0; i < button.size(); ++i) {
        if (i != bottom_button || !bottom_button_flips_page) {
            if (current_file == file[file_number_at_top + i]) {
                button_last_clicked = button[i];
                button_last_clicked->set_on();
    }   }   }
}



void ListFile::highlight_file(const std::string& filename)
{
    std::vector <std::string> ::iterator itr;
    unsigned                             itr_number = 0;

    // Suchen, ob diese Datei wirklich im Verzeichnis ist, sonst passiert nix
    for (itr = file.begin(); itr != file.end(); ++itr, ++itr_number) {
        if (*itr == filename) break;
    }
    if (itr != file.end()) {
        // Wenn es also eine Datei gibt, dann nehmen wir die einfach und setzen
        // auch gleich das current_file_dir korrekt hin.
        current_file_dir  = current_dir;
        unsigned page_new = 0;
        if (bottom_button_flips_page) page_new = itr_number / bottom_button;

        if (page != page_new) {
            page = page_new;
            load_current_dir();
        }
        current_file = filename;

        // Highlight-Button anklicken und Zeiger darauf setzen
        for (unsigned i = 0; i < button.size(); ++i) {
            if (i != bottom_button || !bottom_button_flips_page) {
                if (current_file == file[file_number_at_top + i]) {
                    button_last_clicked = button[i];
                    button_last_clicked->set_on();
        }   }   }
    }
}



void ListFile::calc_self()
{
    for (unsigned int i = 0; i < button.size(); ++i) {
        if (button[i]->get_clicked()) {
            // Seitenwechsel-Button angeklickt?
            if (i == bottom_button && bottom_button_flips_page){
                ++page;
                if (page * bottom_button >= file.size()) page = 0;
                load_current_dir();
                clicked = false;
                return;
            }
            // Ansonsten den Dateinamen nach aussen hin kenntlich machen
            else {
                current_file_dir = current_dir;
                current_file     = file[file_number_at_top + i];
                if (activate_clicked_button) {
                    if (button_last_clicked == button[i]) {
                        button[i]->set_on(!button[i]->get_on());
                    }
                    else if (button_last_clicked) {
                        button_last_clicked->set_off();
                        button[i]->set_on();
                    }
                    else {
                        button[i]->set_on();
                    }
                }
                button_last_clicked = button[i];
                clicked = true;
                return;
            }
        } // Ende von Button angeklickt
    } // Ende der For-Schleife, die alle Buttons durchlaeuft
    clicked = false;
}



void ListFile::draw_self()
{
    undraw_self();
    Frame::draw_self();
}

}
// Ende Namensraum Api
