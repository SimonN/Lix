/*
 * other/net_t.h
 *
 * Structs that can be (de)serialized and sent over the network. Swoosh.
 *
 * Also specifies Uint32 as the platform-independent uint32_t. For this,
 * the non-C++-standard C header stdint.h is included.
 *
 */

#pragma once

#include <./enet/enet.h>
#undef ERROR // enet defines this, we don't need/want it because of Log::ERROR.

#include <stdint.h> // uint32_t

typedef uint32_t Uint32;
typedef char     PlNr;   // player number
typedef char     RoId;   // room ID

extern const int
 NETWORK_PROTOCOL_VERSION,
 NETWORK_TEXT_LENGTH,
 NETWORK_PLAYERS_MAX,
 NETWORK_ROOMS_MAX;

extern const unsigned char
 LEMNET_CHANNEL_MAIN,
 LEMNET_CHANNEL_REPLAY, // maybe == MAIN
 LEMNET_CHANNEL_CHAT;

extern const size_t
 LEMNET_CHANNEL_MAX;    // no. of channels to allocate

extern const char
 LEMNET_NOTHING,        // nothing to process/deallocate
 LEMNET_DISCON_SILENT,  // receiver shall disconnect without any more prints
 LEMNET_WELCOME_DATA,   // protocol version, lpp versions
 LEMNET_YOU_TOO_OLD,    // inform client that he shall disconnect
 LEMNET_YOU_TOO_NEW,    // only if the server has locked version (= no dedic.)
 LEMNET_SOMEONE_OLD,    // someone else has a non-fitting version
 LEMNET_SOMEONE_NEW,    // someone else has a non-fitting version
 LEMNET_RECHECK,

 LEMNET_ASSIGN_NUMBER,
 LEMNET_ROOM_DATA,      // s->c: Overview over all available rooms
 LEMNET_ROOM_CHANGE,    // c->s: I want to switch. s->c: You can/must switch.
 LEMNET_ROOM_CREATE,    // c->s: I want to open a new room.

 LEMNET_PLAYER_DATA,    // new player or altered player information
 LEMNET_PLAYER_BEFORE,  // only a message who was in the room before
 LEMNET_PLAYER_OUT_TO,  // s->c only, someone went into another room
 LEMNET_PLAYER_CLEAR,

 LEMNET_CHAT_MESSAGE,
 LEMNET_LEVEL_FILE,

 LEMNET_GAME_START,     // contains a permutation starting with byte 2
 LEMNET_GAME_END,
 LEMNET_REPLAY_DATA,
 LEMNET_UPDATES;



inline bool operator == (const ENetAddress& a, const ENetAddress& b)
{
    return a.host == b.host && a.port == b.port;
}



struct WelcomeData {

    Uint32 protocol;
    Uint32 lpp_version;
    Uint32 lpp_version_min;

    ENetPacket* create_packet() const;
    void        read_from    (ENetPacket*);

};



struct PlayerData {

    static const int size_of         = 39;
    static const int name_max_length = 32;

    char number;
    char room;
    char spec;  // 0 means active player, true means spectator
    char ready;
    char style;
    char name[name_max_length + 1];

    PlayerData(const char  = -1, // number
               const char* = "", // name
               const char  = 0); // style

    void set_name(const char* na);

    inline bool get_spec()       { return spec == 1; }
    inline void set_spec(bool b) { spec = b ? 1 : 0; }

    bool operator <  (const PlayerData&); // for storage using std::set
    bool operator == (const PlayerData&);
    bool operator != (const PlayerData&);

    // Serialization
    ENetPacket* create_packet() const;
    void        read_from    (ENetPacket*);

};



struct ReplayData {

    char   player;
    char   action;
    Uint32 update;
    Uint32 what;

    ReplayData(const char = 0); // value for action

    // Serialization
    ENetPacket* create_packet() const;
    void        read_from    (ENetPacket*);

};



// Funktionen
ENetPacket* create_packet            (const unsigned);
ENetPacket* create_packet_from_uint32(Uint32);
Uint32      get_uint32_from          (enet_uint8*);
