/*
 * menu/lobby_ro.cpp
 *
 */

#include <sstream>

#include "lobby_ro.h"

#include "../other/language.h"

namespace Api {

const int RoomList::button_yl(20);



RoomList::RoomList(
    int x,
    int y,
    int xl,
    int yl
) :
    Frame  (x, y, xl, yl),
    last_button_is_create(false),
    room_clicked(0)
{
}



void RoomList::set_room_data(const std::set <NetClient::RoomData>& rd)
{
    this->clear();
    button_rooms.clear();
    last_button_is_create = false;
    // Make one button for each room
    for (Network::RoomCIt itr = rd.begin(); itr != rd.end(); ++itr) {
        // don't display the lobby as a room to switch into
        if (itr->number == 0) continue;
        if (((int) buttons.size() + 1) * button_yl > get_yl()) break;
        button_rooms.push_back(itr->number);
        buttons.push_back(
         TextButton(0, buttons.size() * button_yl, get_xl(), button_yl));
        std::ostringstream str;
        str << Language::win_lobby_room_number << (int) itr->number
            << ", " << (int) itr->members << " "
            << (itr->members == 1 ? Language::win_lobby_room_player
                                  : Language::win_lobby_room_players);
        buttons.back().set_text(str.str());
        buttons.back().set_undraw_color(color[COL_API_M]);
    }
    // Room creation button
    if (((int) buttons.size() + 1) * button_yl <= (int) get_yl()) {
        button_rooms.push_back(0);
        buttons.push_back(
         TextButton(0, buttons.size() * button_yl, get_xl(), button_yl));
        buttons.back().set_text(Language::win_lobby_room_create);
        buttons.back().set_undraw_color(color[COL_API_M]);
        last_button_is_create = true;
    }
    for (std::vector <TextButton> ::iterator itr = buttons.begin();
     itr != buttons.end(); ++itr) add_child(*itr);

    set_draw_required();
}



void RoomList::clear()
{
    clear_children();
    buttons.clear();
    set_draw_required();
}



bool RoomList::get_clicked()
{
    for (unsigned i = 0; i < buttons.size(); ++i) {
        if (buttons[i].get_clicked()) {
            room_clicked = button_rooms[i];
            return true;
        }
    }
    // no button got clicked on
    return false;
}



// ############################################################################
// ################################################################## LobbyChat
// ############################################################################



LobbyChat::LobbyChat(int x, int y, int xl)
:
    Frame(x, y, xl, Console::lines_max_number * 20),
    labels(Console::lines_max_number)
{
    for (int i = 0; i < (int) labels.size(); ++i) {
        labels[i].set_x(3);
        labels[i].set_y(3 + 19 * i);
        add_child(labels[i]);
    }
}



void LobbyChat::clear()
{
    for (std::vector <Label> ::iterator itr = labels.begin();
     itr != labels.end(); ++itr) itr->set_text();
}



void LobbyChat::set_chat_data(const std::list <Console::Line>& li)
{
    int count = 0;
    for (Console::LiIt i = li.begin();
     i != li.end() && count < (int) labels.size(); ++i) {
        // + 3 sind's auch in api/button_t.cpp
        labels[count].set_text(i->text);
        labels[count].set_color(i->white?color[COL_TEXT_ON]:color[COL_TEXT]);
        ++count;
    }
}

}
// end of namespace Api
