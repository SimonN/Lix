/*
 * client_c.cpp
 *
 * NetClient::calc()
 *
 */

#include <sstream>
#include <fstream> // write levels

#include "client.h"
#include "../other/log.h" // DEBUGGING rohan

#include "../other/console.h"
#include "../other/language.h"
#include "../other/user.h"

void NetClient::set_level(const std::string& filename)
{
    if (!me || !server) return;

    Level level(filename);
    std::ostringstream s;
    s << level;

    // Kopiert von send_chat_message und angepasst
    ENetPacket* p = create_packet(s.str().length() + 3);
    p->data[0]    = LEMNET_LEVEL_FILE;
    p->data[1]    = ourself->number;
    unsigned i    = 2;
    while (i < s.str().length() + 2) {
        p->data[i] = s.str()[i - 2];
        ++i;
    }
    p->data[i] = '\0';
    enet_peer_send(server, LEMNET_CHANNEL_MAIN, p);
}



void NetClient::send_welcome_data()
{
    if (!me || !server) return;
    WelcomeData our_wcd;
    our_wcd.protocol        = NETWORK_PROTOCOL_VERSION;
    our_wcd.lpp_version     = gloB->version;
    our_wcd.lpp_version_min = gloB->version_min;
    ENetPacket* p = our_wcd.create_packet();
    enet_peer_send(server, LEMNET_CHANNEL_MAIN, p);
}



// ############################################################################
// ##################################################################### Client
// ############################################################################



void NetClient::calc()
{
    ENetEvent event;

    if (me && server)
     while (enet_host_service(me, &event, 0) > 0) {

        char type = (event.type == ENET_EVENT_TYPE_RECEIVE)
                  ? (char) event.packet->data[0]
                  : LEMNET_NOTHING;

        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            // Server shall check if protocols and versions match.
            send_welcome_data();
        }

        else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            exit = true;
        }

        else if (type == LEMNET_DISCON_SILENT) {
            exit = true;
        }

        else if (type == LEMNET_WELCOME_DATA) {
            // The server sends out packets of this sort, but that only
            // happens to inform clients before 2009 Oct 25 of their very
            // old version. The server will probably stop that in some
            // months, and until then we simply ignore these.
        }

        else if (type == LEMNET_YOU_TOO_OLD
         ||      type == LEMNET_YOU_TOO_NEW) {
            const bool old = (type == LEMNET_YOU_TOO_OLD);
            if  (old) Console::push_back(Language::net_chat_we_too_old);
            else      Console::push_back(Language::net_chat_we_too_new);
            std::string msg = Language::net_chat_version_yours;
            msg += Help::version_to_string(gloB->version);
            msg += Language::net_chat_version_server;
            msg += Help::version_to_string(
                    get_uint32_from(event.packet->data + 2));
            msg += '.';
            Console::push_back(msg);
            if (old) Console::push_back(Language::net_chat_please_download);
            else     Console::push_back(Language::net_chat_server_update);
            // The server will throw us out.
        }

        else if (type == LEMNET_SOMEONE_OLD
         ||      type == LEMNET_SOMEONE_NEW) {
            int who_nr = event.packet->data[1];
            PlDatIt who = players.begin();
            while (who != players.end() && who->number != who_nr) ++who;

            if (type == LEMNET_SOMEONE_NEW) {
                Console::push_back(Language::net_chat_someone_new);
                Console::push_back(Language::net_chat_server_update);
            }
            // There exists someone with that number
            else if (who != players.end()) {
                std::string s = who->name + ' '
                              + Language::net_chat_named_guy_old;
                Console::push_back(s);
            }
            else Console::push_back(Language::net_chat_someone_old);
            // This is independent from new or old
            if (who != players.end()) {
                // reset player
                players.erase(who);
                set_nobody_ready();
                player_data_change = true;
            }
        }

        else if (type == LEMNET_RECHECK) {
            send_welcome_data();
        }

        else if (type == LEMNET_ASSIGN_NUMBER) {
            PlayerData pd(event.packet->data[2],
             gloB->user_name.c_str());
            pd.style = pd.number % (LixEn::STYLE_MAX-1) + 1;

            players.clear();
            players.push_back(pd);
            ourself = players.begin();

            ENetPacket* p = pd.create_packet();
            enet_peer_send(server, LEMNET_CHANNEL_MAIN, p);
        }

        else if (type == LEMNET_ROOM_DATA) {
            Log::log("debugging rohan: room data");
            rooms.clear();
            for (int room = 0; room < NETWORK_ROOMS_MAX; ++room)
             if (event.packet->data[2 + room] > 0) {
                RoomData rd;
                rd.number  = room;
                rd.members = event.packet->data[2 + room];
                rooms.insert(rd);
            }
            room_data_change = true;
            Log::log("debugging rohan: room data done");
        }

        else if (type == LEMNET_ROOM_CHANGE) {
            Log::log("debugging rohan: room change");
            // Throw out all people other than us, because we'll soon receive
            // LEMNET_PLAYER_BEFORE packets for everyone in the new room.
            PlayerData temp_ourself = *ourself;
            players.clear();
            players.push_back(temp_ourself);
            ourself = players.begin();
            const char target_room = event.packet->data[2];
            if (ourself->room == -1)
             Console::push_back(Language::net_chat_we_connected);
            else if (target_room == 0)
             Console::push_back(Language::net_chat_we_in_lobby);
            else {
                std::ostringstream message;
                message << Language::net_chat_we_in_room
                        << (int) target_room
                        << Language::net_chat_we_in_room_2;
                Console::push_back(message.str());
            }
            ourself->room = target_room;
            Log::log("debugging rohan: room change done");
        }

        else if (type == LEMNET_PLAYER_DATA
         ||      type == LEMNET_PLAYER_BEFORE) {
            Log::log("debugging rohan: player before");
            PlayerData pd;
            pd.read_from(event.packet);
            PlDatIt known = get_player(pd.number);

            // A new person has joined
            if (known == players.end()) {
                // Insert new data before the first higher data, so that
                // the players list is still sorted by operator <.
                PlDatIt insert_here = --players.end();
                while (insert_here != --players.begin()
                 && pd < *insert_here) --insert_here;
                players.insert(++insert_here, pd);
                set_nobody_ready();
                if (type == LEMNET_PLAYER_DATA) {
                    std::ostringstream message;
                    message << pd.name;
                    if (ourself->room == 0)
                        message << Language::net_chat_player_in_lobby;
                    else {
                        message << Language::net_chat_player_in_room;
                        message << (int) ourself->room;
                        message << Language::net_chat_player_in_room_2;
                    }
                    Console::push_back(message.str());
                }
            }
            else {
                // Only the information about readiness doesn't set back
                // all ready states. Everything else does.
                // snr = set nobody ready
                bool dont_call_snr = *known == pd;
                known->ready  = !known->ready; // *known will be overwr. anyway
                dont_call_snr = dont_call_snr || *known == pd;
                *known = pd;
                if (!dont_call_snr) set_nobody_ready();
            }
            player_data_change = true;
            Log::log("debugging rohan: player before done");
        }

        else if (type == LEMNET_PLAYER_CLEAR
         ||      type == LEMNET_PLAYER_OUT_TO) {
            unsigned nr = event.packet->data[1];
            PlDatIt who = get_player(nr);
            if (who != players.end()) {
                std::ostringstream message;
                message << who->name;
                if (type == LEMNET_PLAYER_CLEAR) {
                    message << Language::net_chat_disconnection;
                }
                else {
                    int room = event.packet->data[2];
                    if (room == 0) {
                        message << Language::net_chat_player_out_lobby;
                    }
                    else {
                        message << Language::net_chat_player_out_room;
                        message << room;
                        message << Language::net_chat_player_out_room_2;
                    }
                }
                Console::push_back(message.str());
                players.erase(who);
                player_data_change = true;
                set_nobody_ready();
            }
        }

        else if (type == LEMNET_LEVEL_FILE) {
            Log::log("debugging rohan: level file");
            set_nobody_ready();
            std::istringstream str((char*) event.packet->data + 2);
            level.load_from_stream(str);
            level_change = true;

            PlDatIt who = get_player(event.packet->data[1]);
            if (who != players.end()) {
                std::ostringstream msg;
                msg << who->name << ' ' << Language::net_chat_level_change
                    << ' ' << level.get_name();
                Console::push_back(msg.str());
            }
            Log::log("debugging rohan: level file done");
        }

        else if (type == LEMNET_CHAT_MESSAGE) {
            std::string message = (const char*) &event.packet->data[2];
            Console::push_back(message, true); // true = weiss
        }

        else if (type == LEMNET_GAME_START) {
            // Reset things to make it possible to play multiple games one
            // after another
            set_nobody_ready();
            replay_data.clear();
            permu = Permu(event.packet->data[2],(char*)event.packet->data + 3);

            // Jetzt aber... auf geht's!
            game_start = true;
            updates_change = 0;

            std::ostringstream start_message;
            start_message << Language::net_game_start;
            if (useR->key_chat > 0
             && useR->key_chat < KEY_MAX) {
                start_message << Language::net_game_how_to_chat_1;
                start_message << scancode_to_name(useR->key_chat);
                start_message << Language::net_game_how_to_chat_2;
            }
            Console::push_back(start_message.str());
        }

        else if (type == LEMNET_GAME_END) {
            game_end = true;
            Console::push_back(Language::net_game_end);
        }

        else if (type == LEMNET_REPLAY_DATA) {
            // Only make available if we aren't the sender
            if (event.packet->data[1] != ourself->number) {
                ReplayData data;
                data.read_from(event.packet);
                replay_data.push_back(data);
            }
        }

        else if (type == LEMNET_UPDATES) {
            updates_change = get_uint32_from(&event.packet->data[2]);
        }

        if (event.type == ENET_EVENT_TYPE_RECEIVE)
         enet_packet_destroy(event.packet);
    }
}
