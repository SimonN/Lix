#include "b_key.h"
#include "../manager.h"

#include "../../other/help.h"
#include "../../other/language.h"

namespace Api {

KeyButton::KeyButton(int x, int y, int xl, int yl, int sc)
:
    TextButton(x, y, xl, yl),
    scancode  (sc)
{
    set_scancode(sc);
}



KeyButton::~KeyButton()
{
}



void KeyButton::set_on(const bool b)
{
    Button::set_on(b);
    if (!b) set_off();
    else {
        Manager::add_focus(this);
        clear_keybuf();
    }
}



void KeyButton::set_off()
{
    Button::set_off();
    Manager::remove_focus(this);
}



void KeyButton::set_scancode(const int sc)
{
    if (sc > 0 && sc < KEY_MAX) {
        scancode = sc;
        set_text(scancode_to_name(scancode));
    }
    else {
        scancode = 0;
        set_text(Language::option_key_unassigned);
    }
}



// This is some code that's copied and greatly simplified from TextType.
void KeyButton::calc_self()
{
    TextButton::calc_self();

    if (!get_on()) {
        if (get_clicked()) set_on();
    }
    // Schreibmodus
    else {
        // cancel
        if (hardware.key_once(KEY_ESC)
         || hardware.get_mr() || hardware.get_ml()) set_off();
        // Oder doch noch im Schreibmodus bleiben? Dann Tastatur lesen.
        else {
            int k = hardware.get_key();
            if (k >= 0 && k < KEY_MAX) {
                set_scancode(k);
                set_off();
            }
        }
        set_draw_required(); // to get the blinking "?" whenever necessary
    }
}



void KeyButton::draw_self()
{
    if (get_on() && Help::timer_ticks % 30 < 15) set_text("?");
    else set_scancode(scancode); // regular text, i.e. the key name

    TextButton::draw_self();
}

}
// end of namespace
