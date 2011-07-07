#include <algorithm>

#include "list_dir.h"

#include "../other/help.h"
#include "../other/globals.h"
#include "../other/language.h"

namespace Api {

DirList::DirList(const int x,  const int y,
                 const int xl, const int yl,
                 const std::string& bdir, const std::string& cdir)
:
    Frame(x, y, xl, yl),
    page              (0),
    bottom_button     (yl/20-1),
    base_dir    (bdir),
    current_dir (cdir),
    clicked     (false)
{
    if (cdir == gloB->empty_string) current_dir = bdir;
    load_current_dir();

    set_undraw_color(color[COL_API_M]);
}



DirList::~DirList()
{
    // Erase all dir buttons
    for (std::vector <TextButton*> ::iterator itr = buttons.begin();
     itr != buttons.end(); ++itr) {
        remove_child(**itr);
        delete *itr;
    }
    buttons.clear();
}







void DirList::add_button(const int i, std::string& s) {
    TextButton* t = new TextButton(0, i*20, get_xl(), 20);
    t->set_undraw_color(color[COL_API_M]);
    t->set_text(s);
    buttons.push_back(t);
    add_child(*t);
}



void DirList::load_current_dir() {
    set_draw_required();
    dir_list.clear();

    // Erase all dir buttons
    for (std::vector <TextButton*> ::iterator itr = buttons.begin();
     itr != buttons.end(); ++itr) {
        remove_child(**itr);
        delete *itr;
    }
    buttons.clear();

    if (!Help::dir_exists(current_dir))
     current_dir = base_dir;

    if (current_dir.size() > base_dir.size()) {
        real_buttons_per_page = bottom_button - 1;
    } else {
        real_buttons_per_page = bottom_button;
    }

    Help::find_dirs(current_dir, static_put_to_dir_list, (void*) this);
    std::sort(dir_list.begin(), dir_list.end());

    // Hochwechsler
    if (current_dir.size() > base_dir.size()) {
        add_button(0, Language::dir_parent);
    }
    // Verzeichnisbuttons erstellen
    unsigned int next_from_dir_list = page * real_buttons_per_page;
    // Die folgende While-Schleife macht, dass bei mehreren Seiten immer
    // alle Seiten voll gefuellt sind. Die letzte Seite wird dann ggf. oben
    // mit Eintraegen der vorletzten Seite aufgefuellt.
    while (page > 0 && next_from_dir_list + real_buttons_per_page
           > dir_list.size()) --next_from_dir_list;
    for (unsigned int i = buttons.size();
     i < bottom_button && next_from_dir_list < dir_list.size(); ++i) {
        std::string s = dir_list[next_from_dir_list];
        ++next_from_dir_list;
        if (s[s.size()-1] == '/') s.resize(s.size()-1);
        add_button(i, s);
    }
    // Blaetter-Button anhaengen, es sei denn, es geht genau auf
    if (next_from_dir_list == dir_list.size() - 1 && page == 0) {
        std::string s = dir_list[next_from_dir_list];
        ++next_from_dir_list;
        if (s[s.size()-1] == '/') s.resize(s.size()-1);
        add_button(bottom_button, s);
    // Das hier ist der Blaetter-Button
    } else if (next_from_dir_list < dir_list.size() || page > 0) {
        add_button(bottom_button, Language::dir_flip_page);
    }
}



void DirList::static_put_to_dir_list(std::string& s, void* which_object) {
    DirList* this_object = (DirList*) which_object;
    Help::string_remove_dir(s);
    this_object->dir_list.push_back(s);
}

void DirList::set_current_dir_to_parent_dir() {
    if (current_dir.size() > base_dir.size()) {
        do {
            current_dir.resize(current_dir.size()-1);
        } while (current_dir[current_dir.size()-1] != '/');
        load_current_dir();
    }
}

void DirList::set_current_dir(const std::string& s) {
    if (current_dir == s) return;
    // Wenn wirklich neu...
    current_dir = s;
    load_current_dir();
}











void DirList::calc_self() {
    // Verzeichnis wechseln?
    bool load_path_after_loop = false;
    for (unsigned int i = 0; i < buttons.size(); ++i) {
        if (buttons[i]->get_clicked()) {
            TextButton& t = *buttons[i];
            // Obersten Button angeklickt und es ist Hochwechsler?
            if (t.get_text() == Language::dir_parent) {
                // Ins uebergeordnete Verzeichnis wechseln:
                // Slash löschen, außerdem so lange weitere Buchstaben im
                // Pfadnamen, bis ein neuer Schrägstrich erreicht ist
                page = 0;
                do {
                    current_dir.resize(current_dir.size()-1);
                } while (current_dir[current_dir.size()-1] != '/');
                load_path_after_loop = true;
            }
            // Seitenwechsel-Button angeklickt?
            else if (t.get_text() == Language::dir_flip_page){
                ++page;
                if (page * real_buttons_per_page >= dir_list.size()) page = 0;
                load_path_after_loop = true;
            }
            // Ansonsten in ein Unterverzeichnis wechseln
            else {
                page = 0;
                current_dir += t.get_text() + '/';
                load_path_after_loop = true;
            }
        } // Ende von Button angeklickt
    } // Ende der For-Schleife, die alle Verzeichnisbuttons durchlaeuft
    if (load_path_after_loop) {
        load_current_dir();
        clicked = true;
    } else {
        clicked = false;
    }
    // Ende Verzeichniswechsel
}



void DirList::draw_self()
{
    undraw_self();
    Frame::draw_self();
}

}
// Ende Namensraum Api
