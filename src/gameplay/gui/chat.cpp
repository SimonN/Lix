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
    name.set_undraw_color(0); // don't cut away a part of the lowermost msg

    std::string str = gloB->user_name + ':';
    name.set_text(str);
    name.set_hidden();

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



void GameplayChat::set_hint(const std::string& hi)
{
    set_draw_required();
    hints.clear();
    if (hi.empty()) return;

    typedef std::vector <std::string> VecStr;
    VecStr lines;
    Console::break_lines(lines, hi, font_med, LEMSCR_X - 6);

    for (VecStr::iterator itr = lines.begin(); itr != lines.end(); ++itr) {
        Api::Label lab(3, y_hint_first + y_hint_plus * hints.size());
        hints.push_back(lab);
        hints.back().set_text(*itr);
        hints.back().set_undraw_color(color[COL_PINK]);
    }
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
    for (; nr < msgs.size(); ++nr) if (! msgs[nr].get_text().empty()) {
        msgs[nr].set_text();
        set_draw_required();
    }
    // Clear according to the old yl when drawing
    if (get_draw_required())
        set_yl(type.get_y() + type.get_yl());

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
    type.draw();
}
