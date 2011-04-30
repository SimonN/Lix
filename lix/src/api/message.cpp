#include "message.h"
#include "manager.h"

namespace Api {

BoxMessage::BoxMessage(const unsigned int xl,
                       const unsigned int lines, const std::string& s)
:
    //     X-Koordinate,      Y-Koordinate,
    Window(LEMSCR_X/2 - xl/2, LEMSCR_Y/2 - 50 - lines*10,
    //           X-Laenge,          Y-Laenge,       Fenstertitel
                 xl,                100 + lines*20, s)
{
}

BoxMessage::~BoxMessage()
{
}





void BoxMessage::add_text(const std::string& t)
{
    for (std::vector <Label> ::iterator itr = labels.begin();
     itr != labels.end(); ++itr) remove_child(*itr);
    labels.push_back(Label(20, 40 + labels.size()*20, t));
    for (std::vector <Label> ::iterator itr = labels.begin();
     itr != labels.end(); ++itr) add_child(*itr);
}



void BoxMessage::add_button(const std::string& t, const unsigned int hotkey)
{
    for (unsigned int i = 0; i < button.size(); ++i) {
        button[i].set_x(button[i].get_x() - 60);
    }
    TextButton but(get_xl()/2 + button.size()*60 - 50, get_yl() - 40, 100, 20);
    but.set_text(t);
    but.set_hotkey(hotkey);

    for (std::vector <TextButton> ::iterator itr = button.begin();
     itr != button.end(); ++itr) remove_child(*itr);
    button.push_back(but);
    for (std::vector <TextButton> ::iterator itr = button.begin();
     itr != button.end(); ++itr) add_child(*itr);
}







void BoxMessage::calc_self()
{
    button_number_clicked = 0;
    for (unsigned int i = 0; i < button.size(); ++i) {
        if (button[i].get_clicked()) {
            button_number_clicked = i + 1;
            Manager::remove_focus(this);
        }
    }
}

} // Ende Namensraum Api
