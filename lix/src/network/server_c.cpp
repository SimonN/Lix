/*
 * network/server_c.cpp
 *
 * NetServer::calc()
 *
 */

#include <iostream> // log == true
#include <fstream> // write levels
#include <list>

#include "server.h"



void NetServer::notify_lobby()
{
    // Notify people in room 0 (the lobby) about everyone in every room
    ENetPacket* pck_lobby = create_packet(2 + NETWORK_ROOMS_MAX);
    pck_lobby->data[0] = LEMNET_ROOM_DATA;
    pck_lobby->data[1] = server_number;
    for (int room = 0; room < NETWORK_ROOMS_MAX; ++room)
     pck_lobby->data[2 + room] = 0;
    for (int i = 0; i < NETWORK_PLAYERS_MAX; ++i)
     if (players[i].number != -1 && players[i].room >= 0)
     ++pck_lobby->data[2 + players[i].room];
    broadcast_to_room(pck_lobby, 0);
}



void NetServer::put_player_into_room(char player, char target_room)
{
    if (!host) return;

    std::cout << "Player " << (int) player
              << " switches into room " << (int) target_room << "."
              << std::endl;

    ENetPacket* p = create_packet(3);
    p->data[0] = LEMNET_ROOM_CHANGE;
    p->data[1] = server_number;
    p->data[2] = target_room;
    enet_peer_send(host->peers + player, LEMNET_CHANNEL_MAIN, p);

    // Tell the room switcher about who's already inside the new room
    for (PlDatCIt pl = players.begin(); pl != players.end(); ++pl)
     if (pl->room == target_room && pl->number != -1) {
        ENetPacket* pp = pl->create_packet();
        pp->data[0]    = LEMNET_PLAYER_BEFORE;
        enet_peer_send(host->peers + player, LEMNET_CHANNEL_MAIN, pp);
        std::cout << "  -> send info about player "
                  << (int) pl->number << std::endl;
    }

    // Notify people in the new room, do this before the room switcher enters
    PlayerData temp = players[player];
    temp.room = target_room;
    ENetPacket* pck_new_room = temp.create_packet();
    broadcast_to_room(pck_new_room, target_room);

    // Update the server's information about the switcher, so that now he's
    // in the new room for broadcasting purposes
    char old_room         = players[player].room;
    players[player].room  = target_room;
    players[player].ready = false;

    // Differentiate between lobby and game rooms
    // This will probably send an empty string for room 0.
    send_level_to(host->peers + player, server_number);

    // Notify people in the old room
    ENetPacket* pck_old_room = create_packet(3);
    pck_old_room->data[0] = LEMNET_PLAYER_OUT_TO;
    pck_old_room->data[1] = player;
    pck_old_room->data[2] = target_room;
    broadcast_to_room(pck_old_room, old_room);

    set_nobody_ready(old_room);
    set_nobody_ready(target_room);

    notify_lobby();
}



void NetServer::send_level_to(
    ENetPeer* peer,
    char      sender)
{
    if (!host || !peer) return;

    const std::string& str = rooms[players[peer - host->peers].room].level;

    // Kopiert von send_chat_message und angepasst
    ENetPacket* p = create_packet(str.size() + 3);
    p->data[0]    = LEMNET_LEVEL_FILE;
    p->data[1]    = sender;
    unsigned i    = 2;
    while (i < str.size() + 2) {
        p->data[i] = str[i - 2];
        ++i;
    }
    p->data[i] = '\0';
    enet_peer_send(peer, LEMNET_CHANNEL_MAIN, p);
    std::cout << "  -> submitting current level" << std::endl;
}



void NetServer::start_game_random_permutation(char room)
{
    if (!host) return;
    //if (room <= 0) return; // room 0 is the lobby, don't want to start here

    rooms[room].replay.clear();

    // How many people are in the room? Also reset the server's ready flags.
    size_t count = 0;
    for (PlDatIt itr = players.begin(); itr != players.end(); ++itr)
     if (itr->room == room && itr->number != -1) {
        itr->ready = false;
        ++count;
    }

    // Don't choose the last permu twice in a row. Only if we fail some times
    // to generate something fresh, use the old one anyway.
    for (size_t tries = 0; tries < count; ++tries) {
        Permu old_permu    = rooms[count].permu;
        rooms[count].permu = Permu(count);
        if ( ! (old_permu == rooms[room].permu)) break;
    }
    std::cout << "Starting game in room " << (int) room << std::endl
              << "  -> using permutation: " << rooms[room].permu << std::endl;

    ENetPacket* p = create_packet(count + 3);
    p->data[0] = LEMNET_GAME_START;
    p->data[1] = server_number;
    p->data[2] = rooms[room].permu.get_size();
    for (size_t i = 0; i < count; ++i) {
        p->data[3 + i] = rooms[room].permu[i];
    }
    broadcast_to_room(p, room);

    rooms[room].ticks_game_start = *ticks;
}



// ############################################################################
// ##################################################################### Server
// ############################################################################



void NetServer::calc()
{
    calc_update_notifying();

    ENetEvent event;

    if (host)
     while (enet_host_service(host, &event, 0) > 0) {

        char type = (event.type == ENET_EVENT_TYPE_RECEIVE)
                  ? (char) event.packet->data[0]
                  : LEMNET_NOTHING;

        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            std::cout << event.peer - host->peers << ": ";
            std::cout << "connect" << std::endl;

            // This is only important in the versions of nov/dec 2009.
            // This notifies clients older than Oct 25, 2009 about their
            // outdated version
            ENetPacket* kick_debug = create_packet_from_uint32(
             2009110100 > version_min ? 2009110100 : version_min);
            kick_debug->data[0] = 2;
            enet_peer_send(event.peer, LEMNET_CHANNEL_MAIN, kick_debug);
        }

        else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            char room = players[event.peer - host->peers].room;
            players[event.peer - host->peers] = PlayerData();
            ENetPacket* p = create_packet(2);
            p->data[0] = LEMNET_PLAYER_CLEAR;
            p->data[1] = event.peer - host->peers;
            broadcast_to_room(p, room);
            notify_lobby();
            std::cout << event.peer - host->peers << ": ";
            std::cout << "disconnect" << std::endl;
        }

        else if (type == LEMNET_WELCOME_DATA) {
            // The client always sends WelcomeData on connect.
            // The server doesn't send WelcomeData any more at all.
            WelcomeData wcd;
            wcd.read_from(event.packet);
            std::cout << event.peer - host->peers << ": ";
            std::cout << "welcome data" << std::endl;

            if ((int) wcd.protocol != NETWORK_PROTOCOL_VERSION
             || wcd.lpp_version < version_min) {
                std::cout << "  -> too old, he has: "
                          << wcd.lpp_version << std::endl;
                ENetPacket* p = create_packet_from_uint32(version_min);
                p->data[0]    = LEMNET_YOU_TOO_OLD;
                p->data[1]    = server_number;
                enet_peer_send(event.peer, LEMNET_CHANNEL_MAIN, p);
                enet_peer_disconnect_later(event.peer, 0); // 0 = irrelev. data

                p             = create_packet(2);
                p->data[0]    = LEMNET_SOMEONE_OLD;
                p->data[1]    = event.peer - host->peers;
                enet_host_broadcast(host, LEMNET_CHANNEL_MAIN, p);
            }
            else if (wcd.lpp_version_min > version_min) {
                if (dedicated) {
                    std::cout << "  -> use new minvsn.: "
                              << wcd.lpp_version_min << std::endl;
                    std::cout << "  -> recheck all client versions"
                              << std::endl;
                    version_min = wcd.lpp_version_min;
                    // Everybody must check if they can still compete with the
                    // new min version.
                    ENetPacket* p = create_packet(2);
                    p->data[0]    = LEMNET_RECHECK;
                    p->data[1]    = server_number;
                    enet_host_broadcast(host, LEMNET_CHANNEL_MAIN, p);
                }
                else {
                    std::cout << "  -> too new, he has: "
                              << wcd.lpp_version_min << std::endl;
                    ENetPacket* p = create_packet_from_uint32(version_min);
                    p->data[0]    = LEMNET_YOU_TOO_NEW;
                    p->data[1]    = server_number;
                    enet_peer_send(event.peer, LEMNET_CHANNEL_MAIN, p);
                    enet_peer_disconnect_later(event.peer, 0);

                    p             = create_packet(2);
                    p->data[0]    = LEMNET_SOMEONE_NEW;
                    p->data[1]    = event.peer - host->peers;
                    enet_host_broadcast(host, LEMNET_CHANNEL_MAIN, p);
                }
            }
            else {
                // Protocols and versions match, no updates required on either
                // site. Assign a player number to get started.
                const int nr = event.peer - host->peers;
                if (players[nr].number == -1) {
                    players[nr].number = nr;
                    ENetPacket* p = create_packet(3);
                    p->data[0] = LEMNET_ASSIGN_NUMBER;
                    p->data[1] = server_number;
                    p->data[2] = (char) nr;
                    enet_peer_send(event.peer, LEMNET_CHANNEL_MAIN, p);
                }
                else {
                    std::cout << "  -> version recheck okay" << std::endl;
                }
            }
        }

        else if (type == LEMNET_ROOM_CHANGE) {
            put_player_into_room(event.packet->data[1], event.packet->data[2]);
        }

        else if (type == LEMNET_ROOM_CREATE) {
            std::cout << (int) (event.peer - host->peers) << ": ";
            std::cout << "creating a new room" << std::endl;
            // Find first empty room
            std::vector <bool> full(NETWORK_ROOMS_MAX, 0);
            for (int plr = 0; plr < NETWORK_PLAYERS_MAX; ++plr)
             if (players[plr].number != -1 && players[plr].room != -1)
             full[players[plr].room] = true;
            for (int room = 1; room < NETWORK_ROOMS_MAX; ++room) {
                if (!full[room]) {
                    rooms[room] = Room();
                    std::cout << "  -> room " << room << std::endl;
                    put_player_into_room(event.packet->data[1], room);
                    break;
                }
            }
        }

        else if (type == LEMNET_PLAYER_DATA) {
            PlayerData& about_whom = players[event.peer - host->peers];

            std::cout << (int) about_whom.number << ": ";
            std::cout << "player data" << std::endl;

            // Set everybody non-ready if the player data change was something
            // else than a readyness change
            bool call_set_nobody_ready = about_whom.ready;
            about_whom.read_from(event.packet);
            call_set_nobody_ready = about_whom.ready == call_set_nobody_ready;

            // Was this an answer packet to our LEMNET_ASSIGN_NUMBER?
            if (about_whom.room == -1) {
                put_player_into_room(about_whom.number, 0);
            }
            else {
                // Den Benutzer broadcasten
                ENetPacket* p = about_whom.create_packet();
                broadcast_to_room(p, about_whom.room);
                if (call_set_nobody_ready) set_nobody_ready(about_whom.room);
            }

            // Falls dies die letzte Bereitmeldung war, geben wir das Signal
            // zum Spielstarten an alle. Lag wird im Spiel durch die Gameplay-
            // -Klasse korrigiert.
            bool everyone_ready = true;
            for (PlDatIt i = players.begin(); i != players.end(); ++i)
             if (i->room == about_whom.room && !i->ready && i->number != -1)
             everyone_ready = false;

            // This function resets the ready flags
            if (everyone_ready) start_game_random_permutation(about_whom.room);
        }

        else if (type == LEMNET_CHAT_MESSAGE
         ||      type == LEMNET_LEVEL_FILE
         ||      type == LEMNET_GAME_END)
        {
            const PlayerData& pd = players[event.peer - host->peers];
            if  (type == LEMNET_LEVEL_FILE) {
                rooms[pd.room].level = (char*) (event.packet->data + 2);
                set_nobody_ready(pd.room);
                std::cout << event.peer - host->peers << ": ";
                std::cout << "level file" << std::endl;
            }
            // broadcast everything
            ENetPacket* p = enet_packet_create((void*) event.packet->data,
                         event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
            // chat messages have their own channel, the rest goes over MAIN
            // Currently, chat messages go through the MAIN channel because
            // the broadcast_to_room function doesn't differentiate. But that's
            // not a problem.
            broadcast_to_room(p, pd.room);
        }

        else if (type == LEMNET_REPLAY_DATA) {
            // Save it for when someone enters in the middle of the game
            const PlayerData& pd = players[event.peer - host->peers];
            ReplayData rpd;
            rpd.read_from(event.packet);
            rooms[pd.room].replay.push_back(rpd);

            // Broadcast
            ENetPacket* p = enet_packet_create((void*) event.packet->data,
                         event.packet->dataLength, ENET_PACKET_FLAG_RELIABLE);
            // This was done before Feb 2010 over channel REPLAY, but I guess
            // it'll work with the MAIN channel as well.
            broadcast_to_room(p, pd.room);
        }

        // Dies nachholen, damit es nicht in jedem Receive-Ding fuer die
        // vielen verschiedenen Lemnet-IDs gemacht werden muss
        if (event.type == ENET_EVENT_TYPE_RECEIVE)
         enet_packet_destroy(event.packet);
    }

}
