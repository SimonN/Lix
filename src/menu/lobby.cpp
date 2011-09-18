/*
 * menu/lobby.cpp
 *
 * We do some fancy stuff to update the preview, chat history etc. even while
 * chat_type is focussed. Search for LOBBY_HACK in this file for information.
 *
 */

#include <sstream>

#include "lobby.h"

#include "../api/manager.h"
#include "../graphic/gra_lib.h" // Spectator button
#include "../network/network.h"
#include "../other/console.h"
#include "../other/user.h"

namespace Api {

const unsigned Lobby::dir_list_xl  = 120;
const unsigned Lobby::lev_list_xl  = 300;
const unsigned Lobby::any_list_yl  = 160;
const unsigned Lobby::preview_xl   = 380;
const unsigned Lobby::medium_row_y = 220;

const unsigned Lobby::console_y    = 260;
const unsigned Lobby::console_xl   = 600;
const unsigned Lobby::console_yl   = 160;
const unsigned Lobby::chat_type_x  =  80;
const unsigned Lobby::chat_type_y  = console_y + console_yl + 20;

const unsigned Lobby::start_but_x  = 160;
const unsigned Lobby::start_but_xl = LEMSCR_X - 2*start_but_x;
const unsigned Lobby::start_but_y  =  80;
const unsigned Lobby::start_but_ys =  30;

const unsigned Lobby::button_xl    = 140;

Lobby::Lobby()
:
    Window(0, 0, LEMSCR_X, LEMSCR_Y, Language::win_lobby_title),
    exit_with   (EXIT_WITH_NOTHING),
    mode        (NOT_CONNECTED),
    browser     (0),

    preview     (LEMSCR_X - 20 - preview_xl, 40,
                               preview_xl, 160), // 160 ist loblist_y_len
    room_list   (LEMSCR_X - 20 - preview_xl, 40,
                               preview_xl, 160),
    button_level(LEMSCR_X - 20 - button_xl,
                               medium_row_y,   button_xl),
    button_ready(20,
                               medium_row_y,   button_xl),
    button_exit (LEMSCR_X - 20 - button_xl,
                               chat_type_y,    button_xl),
    unstable_central(get_xl() / 2, start_but_y,
                 Language::win_lobby_unstable_central, Api::Label::CENTERED),
    start_central(start_but_x, start_but_y,                  start_but_xl),
    start_server(start_but_x, start_but_y + 1*start_but_ys, start_but_xl),
    start_client(start_but_x, start_but_y + 2*start_but_ys, start_but_xl/2),
    start_ip    (LEMSCR_X/2,  start_but_y + 2*start_but_ys, start_but_xl/2),

    chat        (20, console_y, console_xl),
    chat_type   (chat_type_x, chat_type_y,
                               LEMSCR_X - 40 - chat_type_x - button_xl),
    desc_chat   (20, chat_type_y, Language::win_lobby_chat),
    players     (20, 40),
    button_spec (GraLib::get(gloB->file_bitmap_lobby_spec),
                               180,   200 - 32)
{
    // Don't show the GARDEN style by placing it off the screen
    for (unsigned i = LixEn::GARDEN; i < LixEn::STYLE_MAX; ++i) {
        button_color.push_back(LobbyColorButton(
         (i <= LixEn::GARDEN ? 10000 :
         180 + (i-1) % 2 * LobbyColorButton::this_xl),
          40 + (i-1) / 2 * LobbyColorButton::this_yl,
         static_cast <LixEn::Style> (i)));
    }

    // Adding children
    add_child(preview);
    add_child(room_list);
    add_child(button_level);
    add_child(button_ready);
    add_child(button_exit);
    add_child(unstable_central);
    add_child(start_central);
    add_child(start_server);
    add_child(start_client);
    add_child(start_ip);
    add_child(chat);
    add_child(chat_type);
    add_child(desc_chat);
    add_child(players);
    add_child(button_spec);
    for (std::vector <LobbyColorButton> ::iterator itr = button_color.begin();
     itr != button_color.end(); ++itr) {
        add_child(*itr);
    }
    for (std::set <Element*> ::iterator itr = get_children().begin();
     itr != get_children().end(); ++itr) {
        (**itr).set_undraw_color(color[COL_API_M]);
    }

    // Hotkeys and texts
    button_level .set_text(Language::win_lobby_select_level);
    button_ready .set_text(Language::win_lobby_ready);
    start_central.set_text(Language::win_lobby_start_central);
    start_server .set_text(Language::win_lobby_start_server);
    start_client .set_text(Language::win_lobby_start_client);

    button_ready .set_hotkey(KEY_SPACE);
    button_exit  .set_hotkey(KEY_ESC);
    chat_type    .set_hotkey(useR->key_chat);
    chat_type    .set_on_enter(this, chat_on_enter_callback);
    start_ip     .set_scroll();
    start_ip     .set_text(gloB->ip_last_used);
    start_ip     .set_on_enter(this, start_ip_on_enter_callback);
    chat_type    .set_scroll();

    // Coming back into the menu after a game
    if (Network::get_started()) {
        set_mode(INSIDE_ROOM);
        players.set_data(Network::get_player_data());
        if (Network::get_room() == 0)
         room_list.set_room_data(Network::get_room_data());
        else preview.set_level(Network::get_level());

        button_ready.set_on(Network::get_ready());
        for (unsigned i = 0; i < button_color.size(); ++i) {
            if (i == (unsigned)Network::get_style() && ! Network::get_spec()) {
                button_color[i].set_on();
                useR->network_last_style = i;
            }
            else button_color[i].set_off();
        }
        if (Network::get_spec()) button_spec.set_on();
    }
    // Or just about to start a network session
    else {
        set_mode(NOT_CONNECTED);
        Console::clear();
        if (gloB->version_stable) {
            Console::push_back(Language::net_chat_welcome_1);
            Console::push_back(Language::net_chat_welcome_2);
        }
        else {
            Console::push_back(Language::net_chat_unstable_1);
            Console::push_back(Language::net_chat_unstable_2);
        }
    }
}



Lobby::~Lobby()
{
    if (browser) delete browser;
    // Nicht Shutdown, das waere Unfug bei Spielstart!
}






void Lobby::set_mode(const Mode& mo)
{
    mode = mo;

    // prevent the flashing of button_ready when rendering everything hidden
    bool old_state_of_ready = button_ready.get_hidden();
    hide_all_children();

    button_exit.show();
    chat.show();
    preview.clear();

    set_subtitle(gloB->empty_string);
    button_exit.set_text(Language::cancel);

    if (mo == NOT_CONNECTED) {
        if (gloB->version_stable) start_central.show();
        else                      unstable_central.show();
        start_server.show();
        start_client.show();
        start_ip.show();
        button_spec.set_off();
        for (std::vector <LobbyColorButton> ::iterator
         itr = button_color.begin(); itr != button_color.end(); ++itr)
         itr->set_off();
    }
    else if (mo == CONNECTING) {
        // show nothing more
    }
    else {
        if (mo == CHOOSE_ROOM) {
            set_subtitle(Language::win_lobby_title_lobby);
            room_list.show();
            button_exit.set_text(Language::win_lobby_exit);
        }
        else if (mo == INSIDE_ROOM) {
            // Don't show the room yet, instead poll the network later
            // Instead, show this, so it doesn't flicker.
            set_subtitle(Language::win_lobby_title_lobby);
            button_level.show();
            preview.show();
            button_exit.set_text(Language::win_lobby_room_leave);
            // button_ready is set in work_self() after network polling
            button_ready.set_hidden(old_state_of_ready);
        }
        players.show();
        chat_type.show();
        desc_chat.show();
        button_spec.show();
        for (std::vector <LobbyColorButton> ::iterator
         itr = button_color.begin(); itr != button_color.end(); ++itr)
         itr->show();
    }
}



void Lobby::calc_self()
{
    // Unterfenster aktiv?
    if (browser) {
        if (browser    ->get_exit_with() == NetworkBrowser::EXIT_WITH_OKAY
         || browser    ->get_exit_with() == NetworkBrowser::EXIT_WITH_EXIT) {
            if (browser->get_exit_with() == NetworkBrowser::EXIT_WITH_OKAY) {
                // Browser should make sure that this is valid
                Network::set_level(browser->get_current_file());
                // Don't show the level immediately, wait for it to return
                // from the server again.
            }
            delete browser;
            browser = 0;
        }
        return;
    }



    // GUI stuff
    if (button_exit.get_clicked()
     || (button_exit.get_hotkey() == KEY_ESC && hardware.get_mr())) {
        if      (mode == NOT_CONNECTED) exit_with = EXIT_WITH_EXIT;
        else if (mode == INSIDE_ROOM) {
            Network::set_room(0);
            set_mode(CHOOSE_ROOM);
        }
        else {
            Network::shutdown();
            set_mode(NOT_CONNECTED);
        }
    }
    else if (start_central.get_clicked()) {
        Network::start_as_client(gloB->ip_central_server);
        set_mode(CONNECTING);
    }
    else if (start_server.get_clicked()) {
        Network::start_as_server();
        set_mode(CONNECTING);
    }
    else if (start_client.get_clicked()) {
        Network::start_as_client(start_ip.get_text());
        set_mode(CONNECTING);
    }
    else if (button_exit.get_clicked()) {
        exit_with = EXIT_WITH_EXIT;
    }
    else if (start_ip.get_on() && hardware.key_enter_once()) {
        start_ip.set_off();
        // Do the same as the button "start as client"
        set_mode(CONNECTING);
        Network::start_as_client(start_ip.get_text());
    }

    for (unsigned i = 0; i < button_color.size(); ++i)
     if (button_color[i].get_clicked()) {
        Network::set_style_and_spec((LixEn::Style) i, false);
        // Already select newest button to be pressed in, so that
        // there is no flicker because the packet takes time
        // Set all others off.
        for (std::vector <LobbyColorButton> ::iterator
         itr = button_color.begin(); itr != button_color.end(); ++itr)
         itr->set_off();
        button_spec.set_off();
        button_color[i].set_on();
        useR->network_last_style = i;
    }

    if (button_spec.get_clicked()) {
        Network::set_style_and_spec(Network::get_style(), true);
        for (std::vector <LobbyColorButton> ::iterator
         itr = button_color.begin(); itr != button_color.end(); ++itr)
         itr->set_off();
        button_spec.set_on();
    }


    // Chatnachricht senden
    // done by chat_on_enter_callback().

    // Inside the lobby
    if (room_list.get_clicked()) {
        if (room_list.get_room_clicked() == 0) Network::create_room();
        else Network::set_room(room_list.get_room_clicked());
        room_list.clear();
        set_mode(INSIDE_ROOM);
    }

    // Inside a game room
    // Do this "if", otherwise the title would show room #0 when creating room
    if (mode == INSIDE_ROOM && Network::get_room() > 0) {
        std::ostringstream subtitle;
        subtitle << Language::win_lobby_title_room << Network::get_room();
        set_subtitle(subtitle.str());
    }
    if (button_level.get_clicked()) {
        if (Network::get_ready()) {
            button_ready.set_off();
            Network::set_ready(false);
        }
        browser = new NetworkBrowser;
        Manager::add_focus(browser);
    }

    // Readyness
    if (button_ready.get_clicked() && Network::get_people_in_room() > 1) {
        button_ready.set_on(!button_ready.get_on()
                          && preview.get_status() == Level::GOOD);
        Network::set_ready(  button_ready.get_on());
    }
}



void Lobby::work_self()
{
    if (browser) return;
    // work_self() stuff:
    // This isn't triggered by GUI element clicks/hotkeys, but rather by
    // actions from the network.

    // Successfully connected
    if (mode == CONNECTING && Network::get_people_in_room()) {
        set_mode(CHOOSE_ROOM);
        if (!Network::get_server()) gloB->ip_last_used = start_ip.get_text();
    }
    // Level arrived
    if (Network::get_level_change()) {
        preview.set_level(Network::get_level());
    }
    // Player data arrived
    if (Network::get_player_data_change()) {
        players.set_data(Network::get_player_data());
        button_ready.set_on(Network::get_ready());
        for (unsigned i = 0; i < button_color.size(); ++i) {
            if (i == (unsigned)Network::get_style()
                && !Network::get_spec()) button_color[i].set_on();
            else                         button_color[i].set_off();
        }
        button_spec.set_on(Network::get_spec());
    }
    // Room data arrived
    if (Network::get_room_data_change()) {
        room_list.set_room_data(Network::get_room_data());
    }
    // Chat is set and thus redrawn every tick right now
    chat.set_chat_data(Console::get_lines_all());

    // Game starts
    if (Network::get_game_start()) exit_with = EXIT_WITH_PLAY;



    // Use the evaluated network data to modify other buttons
    if (Network::get_people_in_room() > 1) button_exit.set_hotkey();
    else                                   button_exit.set_hotkey(KEY_ESC);

    if (mode == INSIDE_ROOM) {
        if (button_spec.get_on() || Network::get_spec()
         || Network::get_people_in_room() <= 1) button_ready.hide();
        else                                    button_ready.show();
    }
}



void Lobby::chat_on_enter_callback(void* lobby_instance)
{
    Texttype& cht = static_cast <Lobby*> (lobby_instance)->chat_type;
    if (!cht.get_text().empty()) {
        Network::send_chat_message(cht.get_text());
        cht.set_text(gloB->empty_string);
        cht.set_on();
    }
}



void Lobby::start_ip_on_enter_callback(void* lobby_instance)
{
    Texttype& stip = static_cast <Lobby*> (lobby_instance)->start_ip;
    if (!stip.get_text().empty()) {
        // Do the same as for the button start_client
        Network::start_as_client(stip.get_text());
        static_cast <Lobby*> (lobby_instance)->set_mode(CONNECTING);
    }

}

}
// Ende Namensraum Api
