/*
 * network/server.h
 *
 * An abstract server.
 *
 */

#pragma once

#include <vector>
#include <string>

#include "net_t.h"
#include "permu.h"

class NetServer {

private:

    typedef std::vector <PlayerData>                  PlDatVec;
    typedef std::vector <PlayerData> ::iterator       PlDatIt;
    typedef std::vector <PlayerData> ::const_iterator PlDatCIt;

    const char  server_number; // != any player number

    const bool  log;            // write to std::cout
    const bool  dedicated;
    Uint32      version_min;

    ENetHost*   host;
    PlDatVec    players;

    struct Room {
        char                     owner;  // he may kick other people
        std::string              level;  // is sent to anyone that enters
        Permu                    permu;
        std::vector <ReplayData> replay; // also sent to newbies
        Uint32                   ticks_game_start;
        Uint32                   ticks_last_notify;
        Room();
    };

    std::vector <Room> rooms; // room 0 is the lobby

    // Timer
    const int        updates_per_second;
    const int        updates_for_notify;
    const int        ticks_per_second;
    volatile Uint32* ticks;

    // Singleton-Klasse
    NetServer       (const NetServer&);
    void operator = (const NetServer&);

    void set_nobody_ready(char room);
    void broadcast_to_room(ENetPacket* what, char room);
    void calc_update_notifying();

    void notify_lobby();
    void put_player_into_room(char player, char room);
    void send_level_to(ENetPeer* to_whom, char sender);
    void start_game_random_permutation(char room_number);

public:

    // log+dedicated, port,   ticks/sec,    ticks
    NetServer(bool, unsigned, int, volatile Uint32*);
    ~NetServer();

    inline void set_version_min(Uint32 v) { version_min = v; }

    void kick(ENetPeer*);

    // Main loop
    void calc();

};
