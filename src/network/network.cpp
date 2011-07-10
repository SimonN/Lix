/*
 * other/network.cpp
 *
 */

#include <sstream>

#include "network.h"
#include "server.h"

#include "../other/console.h"
#include "../other/language.h"

NetServer* Network::server(0);
NetClient* Network::client(0);



void Network::initialize()
{
    enet_initialize();
}



void Network::deinitialize()
{
    shutdown();
    enet_deinitialize();
}



void Network::start_as_server()
{
    if (client || server) return;
    server = new NetServer(false,
                           gloB->server_port,
                           Help::timer_ticks_per_second,
                           &Help::timer_ticks);
    if (server) {
        server->set_version_min(gloB->version_min);
        Console::push_back(Language::net_chat_start_server);
    }
    client = new NetClient(gloB->net_ip_localhost);
}



void Network::start_as_client(const std::string& ip)
{
    if (client) return;
    client = new NetClient(ip);

    if (!server) {
        std::ostringstream s;
        s << Language::net_chat_start_client
          << " " << ip <<  ":" << gloB->server_port << "...";
        Console::push_back(s.str());
    }
}



void Network::send_chat_message(const std::string& message)
{
    if (client) client->send_chat_message(message);
}



void Network::send_replay_data(const ReplayData& data)
{
    if (client) client->send_replay_data(data);
}



void Network::calc()
{
    if (server) server->calc();
    if (client) {
        // We check first for exit and then calc. This way, a client
        // that should exit can be queried once more for player_data etc.
        if (client->get_exit()) {
            delete client;
            client = 0;
        }
        else client->calc();
    }
}



void Network::shutdown()
{
    if (client) {
        delete client;
        client = 0;
    }
    if (server) {
        delete server;
        server = 0;
    }
}
