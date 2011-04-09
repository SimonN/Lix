/*
 * Hardware::cpp
 *
 */

#include "hardware.h"
#include "help.h" // Timer fuer Doppelklick-Geschw.
#include "log.h"
#include "user.h"

Hardware* Hardware::hw(0);

const int Hardware::doubleclick_speed(Help::timer_ticks_per_second/3);
const int Hardware::doubleclick_for60(20);

Hardware::Hardware()
:
    mouse_own_x   (LEMSCR_X/2),
    mouse_own_y   (LEMSCR_Y/2),
    mickey_x(0),
    mickey_y(0),
    key_hold   (ALLEGRO_KEY_MAX, false),
    key_once   (ALLEGRO_KEY_MAX, false),
    key_just_up(ALLEGRO_KEY_MAX, false)
{
    mouse_hold [0] = false;
    mouse_hold [1] = false;
    mouse_hold [2] = false;
    mouse_since[0] = 0;
    mouse_since[1] = 0;
    mouse_since[2] = 0;

    al_install_keyboard();
    al_install_mouse();
    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
}

Hardware::~Hardware()
{
    al_destroy_event_queue(event_queue);
    al_uninstall_mouse();
    al_uninstall_keyboard();
}



void Hardware::initialize()
{
    if (!hw) hw = new Hardware;
}



void Hardware::deinitialize()
{
    if (hw) delete hw;
    hw = 0;
}



void Hardware::main_loop() {

    if (!hw) return;

    // reset keyboard
    hw->typed_characters.clear();
    for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
        hw->key_once[i]    = false;
        hw->key_just_up[i] = false;
    }
    // reset rodent
    for (size_t i = 0; i < 3; ++i) {
        hw->mouse_click  [i] = false;
        hw->mouse_double [i] = false;
        hw->mouse_hold   [i] = (hw->mouse_hold[i] ? hw->mouse_hold[i] + 1 : 0);
        hw->mouse_release[i] = 0;
        if (hw->mouse_since[i] < doubleclick_speed) ++hw->mouse_since[i];
    }



    ALLEGRO_EVENT event;
    while (al_get_next_event(hw->event_queue, &event)) switch (event.type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
        hw->key_hold[event.keyboard.keycode] = true;
        hw->key_once[event.keyboard.keycode] = true;
        break;

    case ALLEGRO_EVENT_KEY_UP:
        hw->key_hold   [event.keyboard.keycode] = false;
        hw->key_just_up[event.keyboard.keycode] = false;
        break;

    case ALLEGRO_EVENT_KEY_CHAR:
        if (event.keyboard.unichar >= 0x20 && event.keyboard.unichar <  0x7F)
         hw->typed_characters += static_cast <char> (event.keyboard.unichar);
        break;

    case ALLEGRO_EVENT_MOUSE_AXES:
        hw->mickey_x += event.mouse.dx;
        hw->mickey_y += event.mouse.dy;
        hw->mouse_own_x += event.mouse.dx;
        hw->mouse_own_y += event.mouse.dy;
        if (hw->mouse_own_x < 0)         hw->mouse_own_x = 0;
        if (hw->mouse_own_y < 0)         hw->mouse_own_y = 0;
        if (hw->mouse_own_x >= LEMSCR_X) hw->mouse_own_x = LEMSCR_X - 1;
        if (hw->mouse_own_y >= LEMSCR_Y) hw->mouse_own_y = LEMSCR_Y - 1;
        break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
        // Mouse buttons are numbered starting from 1. Nan nan nan!
        if (event.mouse.button > 3) break;
        if (hw->mouse_since [event.mouse.button - 1] < doubleclick_speed) {
            hw->mouse_double[event.mouse.button - 1] = true;
        }
        hw->mouse_hold [event.mouse.button - 1] = 1;
        hw->mouse_click[event.mouse.button - 1] = true;
        hw->mouse_since[event.mouse.button - 1] = 0;
        break;

    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
        if (event.mouse.button > 3) break;
        hw->mouse_release[event.mouse.button - 1]
         = hw->mouse_hold[event.mouse.button - 1];
        hw->mouse_hold   [event.mouse.button - 1] = 0;
        break;

    default:
        break;
    }
    // end of hardware event dispatching
}



// ############################################################################
// ############################################################################
// ############################################################################



int Hardware::get_typed_key()
{
    for (size_t i = 0; i < ALLEGRO_KEY_MAX; ++i) {
        if (hw->key_once[i]) return i;
    }
    return -1;
}
const std::string& Hardware::get_typed_characters()
{
    return hw->typed_characters;
}



bool Hardware::get_key_once   (int scancode) { return hw->key_once[scancode]; }
bool Hardware::get_key_hold   (int scancode) { return hw->key_hold[scancode]; }
bool Hardware::get_key_release(int scancode) {
    return hw->key_just_up[scancode];
}



bool Hardware::get_key_enter_once()
{
    // Don't trigger on fullscreen/windowed switch
    return !get_alt() && (hw->key_once[ALLEGRO_KEY_ENTER]
                       || hw->key_once[ALLEGRO_KEY_PAD_ENTER]);
}
bool Hardware::get_key_enter_release()
{
    return hw->key_just_up[ALLEGRO_KEY_ENTER]
        || hw->key_just_up[ALLEGRO_KEY_PAD_ENTER];
}
bool Hardware::get_ctrl()
{
    return hw->key_hold[ALLEGRO_KEY_LCTRL]
        || hw->key_hold[ALLEGRO_KEY_RCTRL];
}
bool Hardware::get_shift()
{
    return hw->key_hold[ALLEGRO_KEY_LSHIFT  ]
        || hw->key_hold[ALLEGRO_KEY_RSHIFT  ];
}
bool Hardware::get_alt()
{
    return hw->key_hold[ALLEGRO_KEY_ALT     ]
        || hw->key_hold[ALLEGRO_KEY_ALTGR   ];
}



void Hardware::freeze_mouse_x()
{
    hw->mouse_own_x -= hw->mickey_x;
    if (hw->mouse_own_x < 0)         hw->mouse_own_x = 0;
    if (hw->mouse_own_x >= LEMSCR_X) hw->mouse_own_x = LEMSCR_X - 1;
    hw->mickey_x = 0;
}
void Hardware::freeze_mouse_y()
{
    hw->mouse_own_y -= hw->mickey_y;
    if (hw->mouse_own_y < 0)         hw->mouse_own_y = 0;
    if (hw->mouse_own_y >= LEMSCR_Y) hw->mouse_own_y = LEMSCR_Y - 1;
    hw->mickey_y = 0;
}
