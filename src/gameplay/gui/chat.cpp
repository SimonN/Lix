// see ./tutorial.cpp for more functions about filling the chat/hints

#include "chat.h"
#include "../../other/globals.h" // player name
#include "../../other/user.h"
#include "../../network/network.h"

const int GameplayChat::y_msg (12);
const int GameplayChat::y_hint_first(1);
const int GameplayChat::y_hint_plus (19);



GameplayChat::GameplayChat()
:
    Element(0, 0, LEMSCR_X, 20),
    msgs   (Console::lines_max_number),
    name   (0, 0),
    type   (0, 0, LEMSCR_X),
    type_on_last_frame(0)
{
    int y = 0;
    for (std::vector <Api::Label> ::iterator itr = msgs.begin();
     itr != msgs.end(); ++itr) {
        itr->set_font(font_sml);
        itr->set_undraw_color(color[COL_PINK]);
        itr->set_y(y);
        y += y_msg;
    }

    set_undraw_color(color[COL_PINK]);
    name.set_undraw_color(color[COL_PINK]);
    type.set_undraw_color(color[COL_PINK]);

    std::string str = gloB->user_name + ':';
    name.set_text(str);
    name.set_hidden();

    type.set_x (name.get_xl());
    type.set_xl(LEMSCR_X - name.get_xl());
    type.set_on_enter(this, type_on_enter_callback);
    type.set_on_esc  (this, type_on_esc_callback);
    type.set_invisible();
}



void GameplayChat::set_type_on()
{
    name.show();
    type.set_on();
    set_draw_required();
}



void GameplayChat::set_type_off()
{
    name.set_hidden();
    type.set_off();
    set_draw_required();
}



bool GameplayChat::get_type_on()            { return type.get_on();          }
bool GameplayChat::get_type_on_last_frame() { return type_on_last_frame > 0; }



void GameplayChat::type_on_enter_callback(void* chat_instance)
{
    GameplayChat& ch = *static_cast <GameplayChat*> (chat_instance);
    if (!ch.type.get_text().empty()) {
        Network::send_chat_message(ch.type.get_text());
        ch.type.set_text();
    }
}



void GameplayChat::type_on_esc_callback(void* chat_instance)
{
    GameplayChat& ch = *static_cast <GameplayChat*> (chat_instance);
    ch.type.set_text();
}



void GameplayChat::calc_self()
{
    const int yl_all_hints = y_hint_first + y_hint_plus * hints.size();

    const std::list <Console::Line>& li = Console::get_lines_recent();
    size_t nr = 0;

    for (Console::LiIt i = li.begin(); i != li.end(); ++i) {
        if (msgs[nr].get_text() != i->text) {
            msgs[nr].set_text(i->text);
            set_draw_required();
        }
        msgs[nr].set_color(i->white ? color[COL_WHITE] : color[COL_TEXT]);
        msgs[nr].set_y(yl_all_hints + y_msg * nr);
        ++nr;
    }
    for (size_t i = nr; i < msgs.size(); ++i)
     if (! msgs[i].get_text().empty()) {
        msgs[i].set_text();
        set_draw_required();
    }
    // Clear according to the old yl when drawing
    // yl is only used for drawing, so as a little kludge, we'll be setting
    // it again in draw_self()
    if (get_draw_required()
     && get_yl() < type.get_y() + type.get_yl()) {
        set_yl(type.get_y() + type.get_yl());
    }

    type.set_y(yl_all_hints + y_msg * nr - 2);
    name.set_y(yl_all_hints + y_msg * nr - 2);

    type.set_hidden(!type.get_on());
    name.set_hidden(!type.get_on());
    if (type_on_last_frame > 0) --type_on_last_frame;
    if (type.get_on()) type_on_last_frame = 2;
}



void GameplayChat::draw_self()
{
    undraw_self();
    for (std::vector <Api::Label> ::iterator itr = hints.begin();
     itr != hints.end(); ++itr) { itr->set_draw_required(); itr->draw(); }
    for (std::vector <Api::Label> ::iterator itr = msgs.begin();
     itr != msgs.end(); ++itr) itr->draw();
    name.draw();
    // type.draw(); this isn't necessary, it's drawn by focus

    // see comment in calc_self() for how this is a little kludge
    set_yl(type.get_y() + type.get_yl());
    set_draw_required(false);
}
