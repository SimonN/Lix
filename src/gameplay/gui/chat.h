/*
 * gameplay/chat.h
 *
 * The console and chat texttype at the top of the screen
 *
 */

#pragma once

#include "../../api/label.h"
#include "../../api/texttype.h"
#include "../../other/console.h"

class GameplayChat : public Api::Element {

public:

    GameplayChat();
    virtual inline ~GameplayChat() {}

    void set_type_on();
    void set_type_off();

    bool get_type_on();
    bool get_type_on_last_frame(); // a bit kludgy, but good for Gameplay
                                   // because it doesn't work with elders
private:

    std::vector <Api::Label> msgs;
    Api::Label               name;
    Api::Texttype            type;

    int                      type_on_last_frame;

    static void type_on_enter_callback(void*);
    static void type_on_esc_callback  (void*);

protected:

    virtual void calc_self();
    virtual void draw_self();

};
