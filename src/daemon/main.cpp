/*
 * daemon/main.cpp
 *
 * This is the main source file for the Lix standalone server, lixd.
 *
 * On Unix-like environments, lixd runs as a daemon, i.e. it'll detach
 * itself from the running session and operates in the background.
 *
 * On other systems, e.g. Windows, lixd will run non-interactively as a
 * process in a console. It'll be shut down when the console gets closed.
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

    enet_initialize();

    // Create server. Note that we use the number of updates per second as
    // the number of ticks per second: The network will track a timer that
    // runs slower than Lix's timer, even though server->calc() will get
    // called more often than Lix's main loop.
    NetServer* server = new NetServer(true, globals_port_default,
     globals_updates_per_second, &timer_ticks);

    // Main loop
    while (OS::no_system_signal_received()) {
        OS::update_timer(&timer_ticks, globals_updates_per_second);
        server->calc();
        OS::yield();
    }
    delete server;

    enet_deinitialize();

    OS::free_lockfile();

    return 0;
}

