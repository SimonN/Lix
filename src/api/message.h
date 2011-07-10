/*
 * api/message.h
 *
 * Eine Nachrichten-Box, auf der Text erscheint und die einen bis drei
 * Buttons zum Beantworten bietet.
 *
 * int get_button_clicked()
 *
 *   Diese Funktion liefert 0, wenn kein Button in diesem Tick angeklickt
 *   worden ist; oder aber 1 fuer den ersten, 2 fuer den zweiten etc.
 *
 */

#pragma once

#include "button/b_text.h"
#include "label.h"
#include "window.h"

namespace Api {
class BoxMessage : public Window {
private:

    int button_number_clicked;

    std::vector <Label>       labels;
    std::vector <TextButton>  button;

    // Kopieren verboten
    BoxMessage(const BoxMessage&);
    void operator = (const BoxMessage&);

public:

    //         Breite,             Textzeilen,         Fenstertitel
    BoxMessage(const unsigned int, const unsigned int, const std::string&);
    virtual ~BoxMessage();

    void add_text  (const std::string&);
    void add_button(const std::string&, const unsigned int hotkey = 0);

    inline int get_button_clicked() { return button_number_clicked; }

protected:

    virtual void calc_self();

};
}
