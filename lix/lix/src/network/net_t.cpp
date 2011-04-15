/*
 * other/net_t.cpp
 *
 */

#include <cstring> // strncpy, memmove, strcmp

#include "net_t.h"

const int
 NETWORK_PROTOCOL_VERSION   =   2, // enet change, gui rewrite
 NETWORK_TEXT_LENGTH        = 300,
 NETWORK_PLAYERS_MAX        = 127,
 NETWORK_ROOMS_MAX          = 127;

const unsigned char
 LEMNET_CHANNEL_MAIN    =  0,
 LEMNET_CHANNEL_REPLAY  =  0, // Mal sehen, sollte klappen
 LEMNET_CHANNEL_CHAT    =  1;

const size_t
 LEMNET_CHANNEL_MAX     =  2;

const char
 LEMNET_NOTHING         =  0,
 LEMNET_DISCON_SILENT   =  1,
 LEMNET_WELCOME_DATA    =  2,
 LEMNET_YOU_TOO_OLD     =  3,
 LEMNET_YOU_TOO_NEW     =  4,
 LEMNET_SOMEONE_OLD     =  5,
 LEMNET_SOMEONE_NEW     =  6,
 LEMNET_RECHECK         =  7,

 LEMNET_ASSIGN_NUMBER   = 10,
 LEMNET_ROOM_DATA       = 11,
 LEMNET_ROOM_CHANGE     = 12,
 LEMNET_ROOM_CREATE     = 13,

 LEMNET_PLAYER_DATA     = 20,
 LEMNET_PLAYER_BEFORE   = 21,
 LEMNET_PLAYER_OUT_TO   = 22,
 LEMNET_PLAYER_CLEAR    = 23,

 LEMNET_CHAT_MESSAGE    = 30,
 LEMNET_LEVEL_FILE      = 31,

 LEMNET_GAME_START      = 40,
 LEMNET_GAME_END        = 41,
 LEMNET_REPLAY_DATA     = 42,
 LEMNET_UPDATES         = 43;



// ############################################################################
// ################################################################ WelcomeData
// ############################################################################



ENetPacket* WelcomeData::create_packet() const
{
    ENetPacket* pck = enet_packet_create(0, 2 + 4*3,
                       ENET_PACKET_FLAG_RELIABLE);
    pck->data[0] = LEMNET_WELCOME_DATA;
    pck->data[1] = 0; // irrelevant
    Uint32 temp = htonl(protocol);        memmove(pck->data +  2, &temp, 4);
           temp = htonl(lpp_version);     memmove(pck->data +  6, &temp, 4);
           temp = htonl(lpp_version_min); memmove(pck->data + 10, &temp, 4);
    return pck;
}



void WelcomeData::read_from(ENetPacket* pck)
{
    protocol        = ntohl(*(Uint32*) (pck->data +  2));
    lpp_version     = ntohl(*(Uint32*) (pck->data +  6));
    lpp_version_min = ntohl(*(Uint32*) (pck->data + 10));
}



// ############################################################################
// ################################################################# PlayerData
// ############################################################################



PlayerData::PlayerData(const char nr, const char* na, const char st)
:
    number  (nr),
    room    (-1),
    spec    (0),
    ready   (false),
    style   (st)
{
    set_name(na);
    name[name_max_length] = '\0';
}



bool PlayerData::operator < (const PlayerData& rhs)
{
    // Numbers should be different, as the server gives them out
    return number < rhs.number;
}



bool PlayerData::operator == (const PlayerData& rhs)
{
    return number == rhs.number
     &&    room   == rhs.room
     &&    spec   == rhs.spec
     &&    ready  == rhs.ready
     &&    style  == rhs.style
     &&    !strcmp(name, rhs.name);
}



bool PlayerData::operator != (const PlayerData& rhs)
{
    return ! (*this == rhs);
}



void PlayerData::set_name(const char* na)
{
    strncpy(name, na, name_max_length);
}



ENetPacket* PlayerData::create_packet() const
{
    ENetPacket* pck = enet_packet_create(0, size_of,
                       ENET_PACKET_FLAG_RELIABLE);
    pck->data[0] = LEMNET_PLAYER_DATA;
    pck->data[1] = number;
    pck->data[2] = room;
    pck->data[3] = spec;
    pck->data[4] = ready;
    pck->data[5] = style;
    memmove(pck->data + 6, name, name_max_length + 1);
    return pck;
}



void PlayerData::read_from(ENetPacket* pck)
{
    number   = pck->data[1];
    room     = pck->data[2];
    spec     = pck->data[3];
    ready    = pck->data[4];
    style    = pck->data[5];
    memmove(name, pck->data + 6, name_max_length + 1);
}



// ############################################################################
// ################################################################# ReplayData
// ############################################################################



ReplayData::ReplayData(const char a)
:
    player(0),
    action(a),
    update(0),
    what  (0)
{
}



ENetPacket* ReplayData::create_packet() const
{
    ENetPacket* pck = enet_packet_create(0, 11, ENET_PACKET_FLAG_RELIABLE);
    pck->data[0] = LEMNET_REPLAY_DATA;
    pck->data[1] = player;
    pck->data[2] = action;
    Uint32 temp = htonl(update); memmove(pck->data + 3, &temp, 4);
           temp = htonl(what);   memmove(pck->data + 7, &temp, 4);
    return pck;
}



void ReplayData::read_from(ENetPacket* pck)
{
    player = pck->data[1];
    action = pck->data[2];
    update = ntohl(*(Uint32*) (pck->data + 3));
    what   = ntohl(*(Uint32*) (pck->data + 7));
}



// ############################################################################
// ################################################################## Functions
// ############################################################################



ENetPacket* create_packet(unsigned size)
{
    return enet_packet_create(0, size, ENET_PACKET_FLAG_RELIABLE);
}



ENetPacket* create_packet_from_uint32(Uint32 nr)
{
    // Wir nehmen immer 4 Bytes fuer einen unsigned long an, damit die
    // Pakete plattformuebergreifend immer gleich gross sind.
    ENetPacket* p = enet_packet_create(0, 6, ENET_PACKET_FLAG_RELIABLE);
    Uint32 netnr = htonl(nr);
    memmove((void*) &p->data[2], &netnr, 4);
    return p;
}



Uint32 get_uint32_from(enet_uint8* src)
{
    // Auch hier wieder die Annahme aus create_packet_from_ulong.
    Uint32 netnr;
    memmove((void*) &netnr, src, 4);
    return ntohl(netnr);
}
