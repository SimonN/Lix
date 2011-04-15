#include "chat.h"
#include "../other/globals.h" // player name
#include "../other/user.h"
#include "../network/network.h"

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
        add_child(*itr);
        y += 12;
    }
    add_child(name);
    add_child(type);

    name.set_undraw_color(color[COL_PINK]);
    type.set_undraw_color(color[COL_PINK]);

    std::string str = gloB->user_name + ':';
    name.set_text(str);
    type.set_x (name.get_xl());
    type.set_xl(LEMSCR_X - name.get_xl());
    type.set_on_enter(this, type_on_enter_callback);
    type.set_on_esc  (this, type_on_esc_callback);
    type.set_invisible();
}



void GameplayChat::set_type_on()  { type.set_on();  }
void GameplayChat::set_type_off() { type.set_off(); }

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
    // Clear according to the old yl when drawing
    set_yl(type.get_y() + type.get_yl());
    set_draw_required();

    const std::list <Console::Line>& li = Console::get_lines_recent();
    size_t nr = 0;

    for (Console::LiIt i = li.begin(); i != li.end(); ++i) {
        msgs[nr].set_text(i->text);
        msgs[nr].set_color(i->white ? color[COL_WHITE] : color[COL_TEXT]);
        ++nr;
    }
    type.set_y(12 * nr - 2);
    name.set_y(12 * nr - 2);
    for (; nr < msgs.size(); ++nr) {
        msgs[nr].set_text();
    }
    type.set_hidden(!type.get_on());
    name.set_hidden(!type.get_on());
    if (type_on_last_frame > 0) --type_on_last_frame;
    if (type.get_on()) type_on_last_frame = 2;

    set_draw_required();
}



void GameplayChat::draw_self()
{
    // undraw_self();
}
