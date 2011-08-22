#include <algorithm>

#include "list_dir.h"

#include "../other/help.h"
#include "../other/globals.h"
#include "../other/language.h"

namespace Api {

DirList::DirList(const int x,  const int y,
                 const int xl, const int yl,
                 const Filename& bdir, const Filename& cdir)
:
    Frame(x, y, xl, yl),
    page              (0),
    bottom_button     (yl/20-1),
    base_dir    (bdir.get_dir_rootful()),
    current_dir (cdir.get_dir_rootful()),
    clicked     (false)
{
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



void DirList::add_button(const int i, const Filename& fn) {
    // set button text to only the submost directory without any '/'
    const std::string& str = fn.get_dir_rootless();
    int il = 0, ir = 0;
    if (!str.empty()) {
        il = str.length() - 1;
        ir = str.length();
        if (str[il] == '/') { --il; --ir; }
        while (il >= 0 && str[il] != '/') --il;
        ++il; // don't take the '/', but always letter 0 if necessary
    }
    add_button(i, str.substr(il, ir - il));
}



void DirList::add_button(const int i, const std::string& str)
{
    TextButton* t = new TextButton(0, i*20, get_xl(), 20);
    t->set_undraw_color(color[COL_API_M]);
    t->set_text(str);
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

    if (current_dir.get_rootful().size() > base_dir.get_rootful().size()) {
        real_buttons_per_page = bottom_button - 1;
    } else {
        real_buttons_per_page = bottom_button;
    }

    Help::find_dirs(current_dir, static_put_to_dir_list, (void*) this);
    std::sort(dir_list.begin(), dir_list.end());

    // Hochwechsler
    if (current_dir.get_rootful().size() > base_dir.get_rootful().size()) {
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
        add_button(i, dir_list[next_from_dir_list]);
        ++next_from_dir_list;
    }
    // Blaetter-Button anhaengen, es sei denn, es geht genau auf
    if (next_from_dir_list == dir_list.size() - 1 && page == 0) {
        add_button(bottom_button, dir_list[next_from_dir_list]);
        ++next_from_dir_list;
    }
    // Das hier ist der Blaetter-Button
    else if (next_from_dir_list < dir_list.size() || page > 0) {
        add_button(bottom_button, Language::dir_flip_page);
    }
}



void DirList::static_put_to_dir_list(const Filename& fn, void* which_object) {
    DirList* this_object = (DirList*) which_object;
    this_object->dir_list.push_back(fn);
}

void DirList::set_current_dir_to_parent_dir() {
    if (current_dir.get_rootful().size() > base_dir.get_rootful().size()) {
        std::string s = current_dir.get_rootful();
        do {
            s.resize(s.size()-1);
        } while (s[s.size()-1] != '/');
        current_dir = Filename(s);
        load_current_dir();
    }
}



void DirList::set_current_dir(const Filename& s) {
    if (current_dir == s) return;
    // Wenn wirklich neu...
    current_dir = Filename(s.get_dir_rootful());
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
                std::string upperdir = current_dir.get_dir_rootful();
                do {
                    upperdir.resize(upperdir.size()-1);
                } while (upperdir[upperdir.size()-1] != '/');
                current_dir = Filename(upperdir);
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
                current_dir = Filename(current_dir.get_rootful()
                              + t.get_text() + '/');
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
