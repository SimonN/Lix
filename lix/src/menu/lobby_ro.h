/*
 * ./menu/lobby_ro.h
 *
 * A room list for the networking menu. It's displayed when the player is
 * in the lobby.
 *
 * The code here is a bit similar to the directory/file lists.
 *
 * If the "create new room" button has been clicked, clicked() will return
 * true and get_room_clicked() will return 0. (The lobby is room 0, so it's
 * not a meaningful room to switch into at first.)
 *
 */

#pragma once

#include "../network/network.h"

#include "../api/label.h"
#include "../api/button/b_text.h"
#include "../api/frame.h"
#include "../other/console.h" // chat

namespace Api {
class RoomList : public Frame {

public:

    RoomList(int x = 0, int y = 0, int xl = 140, int yl = 160);
    virtual ~RoomList() {}

    void        set_room_data   (const std::set <NetClient::RoomData>&);
    void        clear           ();

    bool        get_clicked();
    inline int  get_room_clicked() { return room_clicked; }

private:

    static const int         button_yl;

    std::vector <int>        button_rooms;
    std::vector <TextButton> buttons;

    bool                     last_button_is_create;
    int                      room_clicked;

    // noncopyable
    RoomList(const RoomList&);
    void operator = (const RoomList&);

};



class LobbyChat : public Frame {

public:

    LobbyChat(int x = 0, int y = 0, int xl = 0);

    void set_chat_data(const std::list <Console::Line>&);
    void clear();

private:

    std::vector <Label> labels;

};

}
