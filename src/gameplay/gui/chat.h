/*
 * gameplay/chat.h
 *
 * The console and chat texttype at the top of the screen
 *
 * This also displays level hints. If a hint is set, the chat/console appears
 * under the hint.
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

    void set_type_on(bool = true);
    void set_type_off();

    inline const std::string& get_text()       { return type.get_text(); }
    inline void set_text(const std::string& s) { type.set_text(s);       }

    bool get_type_on();
    bool get_type_on_last_frame(); // a bit kludgy, but good for Gameplay
                                   // because it doesn't work with elders

    void set_hint(const std::string&); // this parses the hint

private:

    static const int         y_msg;
    static const int         y_hint_first;
    static const int         y_hint_plus;

    std::vector <Api::Label> hints;
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
