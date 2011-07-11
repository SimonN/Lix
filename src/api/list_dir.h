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

#include "../other/filename.h"

namespace Api {
class DirList : public Frame {

private:

    unsigned int page;
    unsigned int bottom_button;
    unsigned int real_buttons_per_page;

    std::vector <Filename>    dir_list;
    std::vector <TextButton*> buttons;

    Filename base_dir;
    Filename current_dir;

    bool clicked;

    void add_button(const int, const Filename&);
    void add_button(const int, const std::string&);

    void load_current_dir();
    static void static_put_to_dir_list(const Filename&, void*);

public:

    DirList(const int, const int, const int, const int,
     const Filename& base_dir, const Filename& current_dir);
    ~DirList();

    bool                   get_clicked()     { return clicked;     }

    inline const Filename& get_base_dir()    { return base_dir;    }
    inline const Filename& get_current_dir() { return current_dir; }
    void                   set_current_dir_to_parent_dir();
    void                   set_current_dir(const Filename& s);

protected:

    virtual void calc_self();
    virtual void draw_self(); // clear remainders of buttons already erased

};
}

