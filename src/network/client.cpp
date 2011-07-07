/*
 * other/network.cpp
 *
 */

#include "client.h"

#include "../other/console.h"
#include "../other/language.h"

NetClient::NetClient(const std::string& ip)
:
    me                (0),
    server            (0),
    exit              (false),
    is_game           (false),
    game_start        (false),
    game_end          (false),
    room_data_change  (false),
    player_data_change(false),
    level_change      (false),
    updates_change    (0),
    ourself           (players.begin()) // will crash if dereferenced if empty
{
    // 0 = Client, 1 = one connection at most (to server), 0, 0 = unlim. bandw.
    //
    // #############################################################
    // # NOTE: If you get a compiler error for the following line, #
    // #  check your enet version. Lix assumes you have enet 1.3.  #
    // #############################################################
    me = enet_host_create(0, 1, LEMNET_CHANNEL_MAX, 0, 0);

    ENetAddress address;
    enet_address_set_host(&address, ip.c_str());
    address.port = gloB->server_port;

    server = enet_host_connect(me, &address, LEMNET_CHANNEL_MAX, 0);
}



NetClient::~NetClient()
{
    if (me) {
        // maybe use disconnect_now instead of disconnect
        if (server) enet_peer_disconnect(server, 0);
        enet_host_flush  (me);
        enet_host_destroy(me);
        me     = 0;
        server = 0;
    }
}



NetClient::PlDatIt NetClient::get_player(char number)
{
    PlDatIt itr = players.begin();
    while (itr != players.end() && itr->number != number) ++itr;
    return itr;
}



void NetClient::set_nobody_ready()
{
    for (PlDatIt i = players.begin(); i != players.end(); ++i)
     i->ready = false;
    player_data_change = true;
}



int NetClient::get_people_in_room()
{
    if (!me) return 0;
    return players.size();
}



int NetClient::get_active_players()
{
    if (!me) return 0;
    int count = 0;
    for (PlDatIt itr = players.begin(); itr != players.end(); ++itr)
     if (!itr->spec) ++count;
    return count;
}



void NetClient::send_chat_message(const std::string& message)
{
    if (!me || !server) return;
    std::string str = gloB->user_name;
    str += ": ";
    str += message;

    ENetPacket* p = create_packet(NETWORK_TEXT_LENGTH + 3);
    p->data[0]    = LEMNET_CHAT_MESSAGE;
    p->data[1]    = ourself->number;
    int i         = 2;
    while (i < NETWORK_TEXT_LENGTH + 2 && i < (int) str.length() + 2) {
        p->data[i] = str[i - 2];
        ++i;
    }
    p->data[i] = '\0';
    enet_peer_send(server, LEMNET_CHANNEL_CHAT, p);
}



const std::set <NetClient::RoomData>& NetClient::get_room_data() {
    return rooms;
}



void NetClient::create_room()
{
    if (!me || !server || ourself == players.end()) return;
    ENetPacket* pck = create_packet(2);
    pck->data[0] = LEMNET_ROOM_CREATE;
    pck->data[1] = ourself->number;
    enet_peer_send(server, LEMNET_CHANNEL_REPLAY, pck);
}



int  NetClient::get_room()
{
    if (!me || !server || ourself == players.end()) return 0;
    else return ourself->room;
}



void NetClient::set_room(const char target_room)
{
    if (!me || !server || ourself == players.end()) return;
    ENetPacket* pck = create_packet(3);
    pck->data[0] = LEMNET_ROOM_CHANGE;
    pck->data[1] = ourself->number;
    pck->data[2] = target_room;
    enet_peer_send(server, LEMNET_CHANNEL_REPLAY, pck);

    // Put ourself directly into the new room, so that we cannot change
    // several times because of the lobby screen not updating.
    ourself->room = target_room;
}



void NetClient::send_replay_data(const ReplayData& data)
{
    if (!me || !server) return;
    ENetPacket* pck = data.create_packet();
    enet_peer_send(server, LEMNET_CHANNEL_REPLAY, pck);
}



void NetClient::set_style_and_spec(const char st, const bool sp)
{
    if (!me || !server || ourself == players.end()) return;
    PlayerData copy = *ourself;
    copy.style = st;
    copy.spec  = sp;
    ENetPacket* pck = copy.create_packet();
    enet_peer_send(server, LEMNET_CHANNEL_MAIN, pck);
}



bool NetClient::get_room_data_change()
{
    bool b = room_data_change;
    room_data_change = false;
    return b;
}



bool NetClient::get_level_change()
{
    bool b = level_change;
    level_change = false;
    return b;
}



bool NetClient::get_player_data_change()
{
    return player_data_change;
    // will be set to false in get_client_data()
}



const std::list <PlayerData>& NetClient::get_player_data()
{
    player_data_change = false;
    return players;
}



const std::vector <ReplayData>& NetClient::get_replay_data()
{
    return replay_data;
}



void NetClient::clear_replay_data()
{
    replay_data.clear();
}



bool NetClient::get_ready()
{
    if (!me || !server || ourself == players.end()) return false;
    return ourself->ready;
}



void NetClient::set_ready(const bool b)
{
    if (!me || !server || ourself == players.end()) return;
    PlayerData copy = *ourself;
    copy.ready = (char) b;
    ENetPacket* pck = copy.create_packet();
    enet_peer_send(server, LEMNET_CHANNEL_MAIN, pck);
}



bool NetClient::get_game_start()
{
    bool b = game_start;
    game_start = false;
    return b;
}



bool NetClient::get_game_end()
{
    bool b = game_end;
    game_end = false;
    return b;
}



void NetClient::set_game_end()
{
    if (!me ||!server || ourself == players.end()) return;
    ENetPacket* p = create_packet(2);
    p->data[0] = LEMNET_GAME_END;
    p->data[1] = ourself->number;
    enet_peer_send(server, LEMNET_CHANNEL_MAIN, p);
}



Uint32 NetClient::get_updates()
{
    Uint32 ret = updates_change;
    updates_change = 0;
    return ret;
}



int NetClient::get_ping_millis()
{
    if (!me || !server) return 0;
    return server->roundTripTime;
}
