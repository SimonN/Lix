/*
 * network/client.h
 *
 * A concrete networking client.
 *
 * const std::vector <ReplayData>& get_replay_data();
 *
 *   This will return new replay data, but the player numbers will have
 *   already been substituted by the players' positions.
 *
 * void send_replay_data(const ReplayData&);
 *
 *   This will translate the positions into player numbers and send it.
 *
 */

#pragma once

#include "../level/level.h"

#include <vector>
#include <set>

#include "net_t.h"
#include "permu.h"

class NetClient {

public:

    struct RoomData {
        int number;
        int members;
        bool operator < (const RoomData& rhs) const
         { return number < rhs.number; }
    };
    typedef std::set <RoomData>                  RoomSet;
    typedef std::set <RoomData> ::const_iterator RoomCIt;

private:

    typedef std::list <PlayerData> ::iterator PlDatIt;

    ENetHost* me;
    ENetPeer* server;

    bool exit;       // true = whoever manages us should destroy us.
    bool is_game;    // nein = Lobby
    bool game_start; // Diese beiden werden auch immer nur dann kurz wahr,
    bool game_end;   // wenn ein Paket mit der entspr. Aufforderung kam.

    bool room_data_change;
    bool player_data_change;
    bool level_change;
    Uint32 updates_change; // Kurz != 0, wenn der Server Zeit synchr.

    std::set    <RoomData>              rooms;

    std::list   <PlayerData>            players;
    std::list   <PlayerData> ::iterator ourself;

    std::vector <ReplayData> replay_data;

    Permu permu;

    Level level;

    // Noncopyable
    NetClient       (const NetClient&);
    void operator = (const NetClient&);

    PlDatIt get_player(char number);  // returns players.end() if not found

    void send_welcome_data();
    void set_nobody_ready();

public:

    NetClient(const std::string&);
    ~NetClient();

    inline bool get_exit() { return exit; }

    int  get_active_players();
    int  get_people_in_room();

    void send_chat_message(const std::string&);
    void send_replay_data (const ReplayData&);

    bool                            get_player_data_change();
    const std::set    <RoomData>&   get_room_data();
    const std::vector <ReplayData>& get_replay_data(); // with pos, no pl. nrs
    const std::list   <PlayerData>& get_player_data(); // player nrs, no pos
    void                            clear_replay_data();

    inline int  get_number() { return ourself->number; }
    inline char get_style()  { return ourself->style;  }
    inline bool get_spec()   { return ourself->spec;   }

    int         get_room();
    void        create_room();
    void        set_room (const char);

    void        set_style_and_spec(const char, const bool);

    inline const Permu& get_permu() { return permu; }
    inline const Level& get_level() { return level; }

    bool        get_room_data_change();
    bool        get_level_change();
    void        set_level(const Filename&);

    bool        get_ready();
    void        set_ready(const bool = true);
    bool        get_game_start();
    bool        get_game_end();
    void        set_game_end();
    Uint32      get_updates();
    int         get_ping_millis();

    // Main loop
    void calc();

};
