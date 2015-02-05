/*
 * hardware.cpp
 *
 */

#include "hardware.h"
#include "help.h" // Timer fuer Doppelklick-Geschw.
#include "user.h"

const unsigned int Hardware::doubleclick_speed(Help::timer_ticks_per_second/3);
const unsigned int Hardware::doubleclick_for60(20);

Hardware::Hardware()
:
    al_mouse_x_last(LEMSCR_X/2),
    al_mouse_y_last(LEMSCR_Y/2),
    mouse_own_x    (LEMSCR_X/2),
    mouse_own_y    (LEMSCR_Y/2),
    mickey_x(0),
    mickey_y(0),
    rest_x(0),
    rest_y(0)
{
    mouse_buffer[0]            = false;
    mouse_buffer[1]            = false;
    mouse_buffer[2]            = false;
    mouse_since [0]            = 0;
    mouse_since [1]            = 0;
    mouse_since [2]            = 0;
    key_from_buffer            = -1;
    key_from_buffer_ascii      = -1;
    clear_keybuf();
}

Hardware::~Hardware() {}



void Hardware::set_mouse_accel_on_windows(bool b)
{
    // According to this:
    // https://www.allegro.cc/manual/4/api/configuration-routines/
    // ... these values will only do something on Windows
    const char *override_0 = "[mouse]\nmouse_accel_factor = 0\n";
    const char *override_1 = "[mouse]\nmouse_accel_factor = 1\n";
    if (b) ::override_config_data(override_1, ::ustrsize(override_1));
    else   ::override_config_data(override_0, ::ustrsize(override_0));
}



void Hardware::center_mouse()
{
    position_mouse(LEMSCR_X/2, LEMSCR_Y/2);
    al_mouse_x_last = LEMSCR_X/2;
    al_mouse_y_last = LEMSCR_Y/2;
}



void Hardware::main_loop() {

    //////////
    // Maus //
    //////////

    mickey_x = mouse_x - al_mouse_x_last;
    mickey_y = mouse_y - al_mouse_y_last;

    if (gloB->screen_fullscreen_now) {
        mickey_x = (rest_x + mickey_x * (int) useR->mouse_speed);
        mickey_y = (rest_y + mickey_y * (int) useR->mouse_speed);
        rest_x = mickey_x % 20;
        rest_y = mickey_y % 20;
        mickey_x /= 20;
        mickey_y /= 20;
    }
    if (mouse_x < LEMSCR_X/4 || mouse_x > LEMSCR_X*3/4
     || mouse_y < LEMSCR_Y/4 || mouse_y > LEMSCR_Y*3/4) center_mouse();
    else {
        al_mouse_x_last = mouse_x;
        al_mouse_y_last = mouse_y;
    }

    mouse_own_x += mickey_x;
    mouse_own_y += mickey_y;
    if (mouse_own_x < 0)         mouse_own_x = 0;
    if (mouse_own_y < 0)         mouse_own_y = 0;
    if (mouse_own_x >= LEMSCR_X) mouse_own_x = LEMSCR_X - 1;
    if (mouse_own_y >= LEMSCR_Y) mouse_own_y = LEMSCR_Y - 1;

    // Buttons
    this->poll_mouse(true);

    //////////////
    // Tastatur //
    //////////////

    last_tick_ctrl  = ctrl;
    last_tick_shift = shift;
    last_tick_alt   = alt;
    ctrl  = (key[KEY_LCONTROL] || key[KEY_RCONTROL]);
    shift = (key[KEY_LSHIFT  ] || key[KEY_RSHIFT  ]);
    alt   = (key[KEY_ALT     ] || key[KEY_ALTGR   ]);

    // Check for text typing
    if (keypressed()) {
        // "_ascii" is misnomer, we can accept Unicode typing here if
        // Allegro can handle it with the current keyboard layout
        key_from_buffer_ascii = ureadkey(&key_from_buffer);
    }
    // Also check for modifiers. This is good for the hotkey assignment menu.
    // key_from_buffer_ascii will never be > -1, but maybe key_from_buffer.
    else {
        key_from_buffer = -1;
        key_from_buffer_ascii = -1;

        if (!last_tick_ctrl && ctrl)   key_from_buffer
         = key[KEY_LCONTROL]  ? KEY_LCONTROL
         : key[KEY_RCONTROL]  ? KEY_RCONTROL : -1;
        if (!last_tick_shift && shift) key_from_buffer
         = key[KEY_LSHIFT]    ? KEY_LSHIFT
         : key[KEY_RSHIFT]    ? KEY_RSHIFT : -1;
        if (!last_tick_alt && alt)     key_from_buffer
         = key[KEY_ALT]       ? KEY_ALT
         : key[KEY_ALTGR]     ? KEY_ALTGR : -1;
    }

}
// Ende Hardware::main_loop()



void Hardware::poll_mouse(const bool actual_tick)
{
    // Moegliche Mausaktivitaet puffern
    // Dies immer tun, egal ob ein Tick ansteht oder nicht]
    mouse_buffer[0] = mouse_buffer[0] || (mouse_b & 1);
    mouse_buffer[1] = mouse_buffer[1] || (mouse_b & 2);
    mouse_buffer[2] = mouse_buffer[2] || (mouse_b & 4);

    // Wenn wirklich ein Tick ansteht
    if (actual_tick) for (unsigned b = 0; b < 3; ++b) {
        ++mouse_since[0];
        // Wurde seit dem letzten Tick die Maus gedrueckt?
        if (mouse_buffer[b]) {
            // Schon in den vorherigen Ticks gedrueckt?
            if (mouse_hold[b]) {
                mouse_click[b]  = false;
                mouse_double[b] = false;
            }
            // Jetzt im ersten Tick geklickt?
            else {
                mouse_click[b] = true;
                // Doppelklick?
                if (mouse_since [b] > doubleclick_speed) {
                    mouse_double[b] = false;
                    mouse_since [b] = 0;
                } else {
                    mouse_double[b] = true;
                    mouse_since [b] = doubleclick_speed; // Gegen Dreifachklick
                }
            }
            ++mouse_hold[b];
            mouse_release[b] = false;
        }
        // Maus nicht gedrueckt, also Puffer war == false
        else {
            mouse_click  [b] = false;
            mouse_double [b] = false;
            mouse_release[b] = mouse_hold[b];
            mouse_hold   [b] = 0;
        }
        // Egal, ob Maus gedrueckt war oder nicht:
        // Puffer hat seine Dienste getan, fuer naechsten Tick neu sammeln
        mouse_buffer[b] = false;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



bool Hardware::key_release(int scancode) {
    if (scancode < 0) return false;
    if      (!key_buffer[scancode] && !key[scancode]) return false;
    else if ( key_buffer[scancode] && !key[scancode]) {
        key_buffer[scancode] = false;
        return true; // Das ist, was wir wollen.
    }
    else {
        key_buffer[scancode] = true;
        return false;
    }
}
// Ende von key_release



bool Hardware::key_enter_once()
{
    // Don't trigger on fullscreen/windowed switch
    return !get_alt() && (key_once(KEY_ENTER) || key_once(KEY_ENTER_PAD));
}



bool Hardware::key_enter_release()
{
    return key_release(KEY_ENTER) || key_release(KEY_ENTER_PAD);
}



void Hardware::freeze_mouse_x()
{
    mouse_own_x -= mickey_x;
    if (mouse_own_x < 0)         mouse_own_x = 0;
    if (mouse_own_x >= LEMSCR_X) mouse_own_x = LEMSCR_X - 1;
    mickey_x = 0;
}
void Hardware::freeze_mouse_y()
{
    mouse_own_y -= mickey_y;
    if (mouse_own_y < 0)         mouse_own_y = 0;
    if (mouse_own_y >= LEMSCR_Y) mouse_own_y = LEMSCR_Y - 1;
    mickey_y = 0;
}



// Und das einzelne Objekt erschaffen
Hardware hardware;
