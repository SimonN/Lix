/*
 * api/button_k.h
 *
 * A button used in the keybinding menu. Click on it and press a key to
 * assign the button  a scancode.
 *
 */

#pragma once

#include "b_text.h"

namespace Api {

class KeyButton : public TextButton {

private:

    int  scancode;

public:

    //        wohin           x        y        x_len      y_len     scancode
    KeyButton(int = 0, int = 0, int = 100, int = 20, int = 0);
    virtual ~KeyButton();

    inline  int  get_scancode() const { return scancode; }
            void set_scancode(int);

    virtual void set_on(bool = true);
    virtual void set_off();

protected:

    virtual void calc_self();
    virtual void draw_self();

}; // end of class
}  // end of namespace
