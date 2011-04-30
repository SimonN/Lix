/*
 * button_t.h
 *
 * Erweiterter Button mit Text, zentriert oder optional linksbuendig.
 *
 * Diesen Buttons kann mit der entsprechenden Methode auch ein Haekchen rechts
 * gesetzt werden. Dadurch verkuerzt sich die Textlaenge entsprechend.
 *
 */

#pragma once

#include "../../other/myalleg.h"
#include <string>

#include "button.h"

#include "../../other/globals.h"

namespace Api {

class TextButton : public Button {

private:

    std::string   text;
    bool          left;

    const Cutbit* check_cutbit;
    int           check_frame;

public:

             //     wohin               x        y        x_len      y_len
             TextButton(int = 0, int = 0, int = 100, int = 20,
                        const std::string& = gloB->empty_string);
             TextButton(const TextButton&);
    virtual ~TextButton();

    inline bool get_left() const               { return left;                 }
    inline void set_left(const bool b = true)  { left = b;set_draw_required();}

    inline std::string& get_text()             { return text;                 }
    inline void set_text(const std::string& s) { text = s;set_draw_required();}

    inline int  get_check_frame() const        { return check_frame;          }
    inline void set_check_frame(const int i)   { check_frame = i;
                                                  set_draw_required();       }

protected:

    virtual void draw_self();

}; // Klassenende

} // Namensraumende
