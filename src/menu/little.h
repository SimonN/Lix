/*
 * menu/little.h
 *
 * Einige kleine Menues: Die Namensabfrage und die Sprachwahl.
 *
 */

#pragma once

#include "../api/button/b_text.h"
#include "../api/texttype.h"
#include "../api/window.h"
#include "../other/language.h"

namespace Api {

class NewPlayerMenu : public Window {

private:

    bool        exit;
    Texttype    name;

    Label       desc_hello;
    Label       desc_name;

    static void name_on_enter(void*);

public:

                NewPlayerMenu();
    virtual    ~NewPlayerMenu();

    inline bool get_exit() { return exit; }

protected:

    virtual void work_self();
    virtual void draw_self();

};



class LanguageMenu : public Window {

private:

    bool                     exit;
    Language::Language       exit_with;

    std::vector <TextButton> button;

public:

                LanguageMenu();
    virtual    ~LanguageMenu();

    inline const bool               get_exit()      { return exit;      }
    inline const Language::Language get_exit_with() { return exit_with; }

protected:

    void        calc_self();

};

} // Ende Namensraum Api
