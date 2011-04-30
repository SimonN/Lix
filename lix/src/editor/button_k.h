/*
 * api/button_k.h
 *
 * Have the user press a key. This is used in the keybinding menu.
 *
 */

#pragma once

#include "button_t.h"

namespace Api {

class KeyButton : public TextButton {

private:

    int scancode;

public:

             //        where           x        y        x_len      y_len
             KeyButton(Torbit& = *osd, int = 0, int = 0, int = 100, int = 20,
                        const std::string& = Globals::empty_string);
             KeyButton(const KeyButton&);
    virtual ~KeyButton();

    inline int  get_scancode() const { return scancode; }
    inline void set_scancode(int i)  { scancode = i;    }



    inline bool get_left() const               { return left;                 }
    inline void set_left(const bool b = true)  { left = b;set_draw_required();}

    inline std::string& get_text()             { return text;                 }
    inline void set_text(const std::string& s) { text = s;set_draw_required();}

    inline int  get_check_frame() const        { return check_frame;          }
    inline void set_check_frame(const int i)   { check_frame = i;
                                                  set_draw_required();       }




public:

};
// end of class KeyButton
}
// end of namespace Api
