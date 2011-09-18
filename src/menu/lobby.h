/*
 * menu/lobby.h
 *
 * Die Netzwerklobby.
 *
 */

#pragma once

#include "lobby_li.h"
#include "lobby_ro.h"
#include "browsers.h"

#include "../api/button/b_bitmap.h"
#include "../api/window.h"

namespace Api {
class Lobby : public Window {

public:

    enum ExitWith {
        EXIT_WITH_NOTHING,
        EXIT_WITH_PLAY,
        EXIT_WITH_EXIT
    };

private:

    enum Mode {
        NOT_CONNECTED,
        CONNECTING,
        CHOOSE_ROOM,
        INSIDE_ROOM
    };

    ExitWith        exit_with;
    Mode            mode;
    NetworkBrowser* browser;

    Preview     preview;
    RoomList    room_list;

    TextButton  button_level;
    TextButton  button_ready;

    TextButton  button_exit;

    Label       unstable_central;
    TextButton  start_central;
    TextButton  start_server;
    TextButton  start_client;
    Texttype    start_ip;

    LobbyChat   chat;
    Texttype    chat_type;
    Label       desc_chat;

    LobbyList   players;
    std::vector <LobbyColorButton> button_color;
    BitmapButton                   button_spec;

    static const unsigned dir_list_xl;
    static const unsigned lev_list_xl;
    static const unsigned any_list_yl;
    static const unsigned preview_xl;
    static const unsigned medium_row_y;

    static const unsigned console_y;
    static const unsigned console_xl;
    static const unsigned console_yl;
    static const unsigned chat_type_x;
    static const unsigned chat_type_y;

    static const unsigned start_but_x;
    static const unsigned start_but_xl;
    static const unsigned start_but_y;
    static const unsigned start_but_ys;

    static const unsigned button_xl;

    void undraw_bottom_buttons();

    // Kopierverbot
    Lobby(const Lobby&);
    void operator = (const Lobby&);

    void set_mode(const Mode&);

    static void chat_on_enter_callback(void*);
    static void start_ip_on_enter_callback(void*);

public:

             Lobby();
    virtual ~Lobby();

    inline void       reset_exit_with()      { exit_with = EXIT_WITH_NOTHING; }
    inline ExitWith     get_exit_with()      { return exit_with;              }
    // inline std::string& get_selected_level() { return selected_level; }

protected:

    virtual void calc_self();
    virtual void work_self();

};
}
