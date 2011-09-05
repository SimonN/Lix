/*
 * network/server.cpp
 *
 */

#include <cstring> // memcpy

#include "server.h"

NetServer::Room::Room()
:
    owner            (-1),
    ticks_game_start (0),
    ticks_last_notify(0)
{
}



NetServer::NetServer(
    bool               b,
    unsigned           port,
    int                ntpsec,
    volatile Uint32*   nticks
) :
    server_number(127),

    dedicated  (b),
    version_min(0),

    host       (0),
    players    (NETWORK_PLAYERS_MAX),
    rooms      (NETWORK_ROOMS_MAX),

    updates_per_second(globals_updates_per_second),
    updates_for_notify(globals_updates_per_second / 5),
    ticks_per_second  (ntpsec),
    ticks             (nticks)
{
    srand(time(0));
    ENetAddress address;
    address.host = ENET_HOST_ANY;
    address.port = port;
    //                                Clientzahl,         beliebige Bandbreite
    //
    // #############################################################
    // # NOTE: If you get a compiler error for the following line, #
    // #  check your enet version. Lix assumes you have enet 1.3.  #
    // #############################################################
    host = enet_host_create(&address, NETWORK_PLAYERS_MAX,
                                      LEMNET_CHANNEL_MAX, 0, 0);
}



NetServer::~NetServer()
{
    if (host) {
        for (int i = 0; i < NETWORK_PLAYERS_MAX; ++i) kick(host->peers+i);
        enet_host_flush  (host);
        enet_host_destroy(host);
        host = 0;
    }
}



void NetServer::broadcast_to_room(ENetPacket* what, char room)
{
    if (host && room >= 0)
     for (PlDatCIt pl = players.begin(); pl != players.end(); ++pl)
     if (pl->room == room && pl->number >= 0) {
        ENetPacket* pck = create_packet(what->dataLength);
        memcpy(pck->data, what->data, what->dataLength);
        enet_peer_send(host->peers + pl->number,  // receiver
                       LEMNET_CHANNEL_MAIN, pck); // channel, packet
    }
    // We do not send *what. However, this function shall handle just like
    // enet_host_broadcast, i.e. *what shall be deallocated.
    enet_packet_destroy(what);
}


void NetServer::set_nobody_ready(char room)
{
    for (PlDatIt pl = players.begin(); pl != players.end(); ++pl)
     if (pl->room == room) pl->ready = false;
}



void NetServer::kick(ENetPeer* p)
{
    if (host) {
        char room = players[p - host->peers].room;
        enet_peer_disconnect(p, 0);
        enet_host_flush(host);
        enet_peer_reset(p);

        // Die Info ueber den geloeschten Benutzer broadcasten
        ENetPacket* pck = create_packet(3);
        pck->data[0] = LEMNET_PLAYER_CLEAR;
        pck->data[1] = p - host->peers;
        pck->data[2] = p - host->peers;
        broadcast_to_room(pck, room);
    }
}



void NetServer::calc_update_notifying()
{
    if (!host) return;
    for (size_t room = 0; room < rooms.size(); ++room)
     if (rooms[room].ticks_game_start > 0) {
        // Determine if we have to send updates. This is if we are at least
        // (updates_for_notify) updates ahead of the last notifying.
        if (*ticks - rooms[room].ticks_last_notify >= (unsigned)
         (updates_for_notify * ticks_per_second / updates_per_second)) {
            // Determine the time in updates after the start of the game and
            // send that to everybody in the game.
            rooms[room].ticks_last_notify = *ticks;
            Uint32 updates = (*ticks - rooms[room].ticks_game_start)
                              * updates_per_second / ticks_per_second;
            ENetPacket* p = create_packet_from_uint32(updates);
            p->data[0] = LEMNET_UPDATES;
            p->data[1] = server_number;
            broadcast_to_room(p, room);
        }
    }
}
