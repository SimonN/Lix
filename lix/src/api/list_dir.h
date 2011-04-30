/*
 * api/dirlist.h
 *
 * Liste mit Verzeichnisnamen, wie sie in den verschiedenen Browsern zum
 * Einsatz kommt
 *
 * Den Rechtsklick fuer das Verzeichnishochwechseln muss die implementierende
 * Klasse uebernehmen. Sie kann ja vielleicht fuer den Rechtsklick andere
 * Aktionen im Sinne haben, daher ist er nicht in DirList eingebaut.
 *
 * unsigned int bottom_button
 *
 *   Der Vektor-Eintrag des untersten Buttons. Wenn zu viele Levels im Ver-
 *   zeichnis sind, dann wird der unterste Button zum Blaetter-Button.
 *
 */

#pragma once

#include <vector>

#include "button/b_text.h"
#include "frame.h"

#include "../other/globals.h" // empty_string

namespace Api {
class DirList : public Frame {

private:

    unsigned int page;
    unsigned int bottom_button;
    unsigned int real_buttons_per_page;

    std::vector <std::string> dir_list;
    std::vector <TextButton*> buttons;

    std::string base_dir;
    std::string current_dir;

    bool clicked;

    void add_button(const int, std::string&);
    void load_current_dir();
    static void static_put_to_dir_list(std::string&, void*);

public:

    DirList(const int, const int, const int, const int,
     const std::string&, const std::string& = gloB->empty_string);
     //    Basisverz.,         aktuelles Verz.,
    ~DirList();

    bool                get_clicked()     { return clicked;     }

    inline std::string& get_base_dir()    { return base_dir;    }
    inline std::string& get_current_dir() { return current_dir; }
    void                set_current_dir_to_parent_dir();
    void                set_current_dir(const std::string& s);

protected:

    virtual void calc_self();
    virtual void draw_self(); // clear remainders of buttons already erased

};
}

