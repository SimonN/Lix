/*
 * other/network.h
 *
 * L++ networking manager class. The gameplay, the menus etc. all access
 * this class and not directly the NetClient or NetServer.
 *
 */

#pragma once

#include "client.h"

#include "../other/types.h"

class NetServer;

class Network {

private:

    typedef const std::set    <NetClient::RoomData> CoRS;
    typedef const std::list   <PlayerData>          CoLP;
    typedef const std::vector <ReplayData>          CoVR;

public:

    typedef std::list <PlayerData>          ::iterator       PlDatIt;
    typedef std::list <PlayerData>          ::const_iterator PlDatCIt;
    typedef            NetClient::RoomData                   RoomData;
    typedef std::set  <NetClient::RoomData> ::const_iterator RoomCIt;

    static void initialize();
    static void deinitialize();

    // The main loop
    static void calc();

    static void start_as_server();
    static void start_as_client(const std::string&);
    static void shutdown       ();

    static void send_chat_message(const std::string&);
    static void send_replay_data (const ReplayData&);

    static inline bool get_server()         { return server != 0; }
    static inline bool get_started()        { return client != 0; }
    static inline  int get_people_in_room()
              { return client ? client->get_people_in_room() : 0; }
    static inline  int get_active_players()
              { return client ? client->get_active_players() : 0; }

    static inline bool  get_room_data_change()
                            { return client && client->get_room_data_change();}
    static inline CoRS& get_room_data()  { return client->get_room_data();    }
    static inline CoVR& get_replay_data() {return client->get_replay_data();  }
    static inline void  clear_replay_data() {     client->clear_replay_data();}
    static inline CoLP& get_player_data() {return client->get_player_data();  }
    static inline bool  get_player_data_change()
                         { return client && client->get_player_data_change(); }

    static inline int  get_number() { return client ?  client->get_number():0;}
    static inline int  get_room()   { return client ?  client->get_room()  :0;}
    static inline char get_style()  { return client ?  client->get_style() :0;}
    static inline const Permu& get_permu() { return    client->get_permu()   ;}

    static inline void create_room()     { if (client) client->create_room(); }
    static inline void set_room (int  r) { if (client) client->set_room (r);  }
    static inline void set_style(char s) { if (client) client->set_style(s);  }

    static inline void set_level(const std::string& s) {if(client)client->set_level(s);}
    static inline bool get_level_change(){ return client && client->get_level_change();}
    static inline const Level& get_level(){return           client->get_level();       }

    static inline bool get_ready()       { return client && client->get_ready();      }
    static inline void set_ready(bool b=true) {if(client)   client->set_ready(b);     }
    static inline bool get_game_start()  { return client && client->get_game_start(); }
    static inline bool get_game_end()    { return client && client->get_game_end();   }
    static inline void set_game_end()    { if    (client)   client->set_game_end();   }
    static inline Uint get_updates()     { return client ?  client->get_updates() : 0;}
    static inline int  get_ping_millis() { return client ?  client->get_updates() : 0;}

private:

    static NetServer* server;
    static NetClient* client;

    // static singleton class
    Network();
    ~Network();
    Network         (const Network&);
    void operator = (const Network&);

};
