/*
 * server/s_main.cpp
 *
 * This is the main source file for the L++ server.
 *
 * This program runs non-interactively and realizes the functionalities a
 * hosting normal L++ player does to his clients.
 *
 * The L++ server shares some source files with L++ itself. The only distinct
 * sources are those in the src/server/ subdirectory like this file.
 *
 * Simon
 *
 */

#include <iostream>

#include "os.h"

#include "../other/globals.h"
#include "../network/server.h"

volatile Uint32 timer_ticks = 0;



int main()
{
    int daem_ret = OS::daemonize("/var/run/lixd.pid");
    if (daem_ret > 0) return 0;
    if (daem_ret < 0) return -1;

    Globals::initialize();
    enet_initialize();

    // Create server. Note that we use the number of updates per second as
    // the number of ticks per second: Our timer is slower than L++'s.
    NetServer* server = new NetServer(true, gloB->server_port,
     gloB->updates_per_second, &timer_ticks);

    // Main loop
    while (OS::no_system_signal_received()) {
        OS::update_timer(&timer_ticks, gloB->updates_per_second);
        server->calc();
        OS::yield();
    }
    delete server;

    enet_deinitialize();
    Globals::deinitialize();

    OS::free_lockfile();

    return 0;
}

