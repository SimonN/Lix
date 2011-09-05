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
#include <sstream> // parse main arguments

#include "os.h"

#include "../other/globals.h"
#include "../network/server.h"

volatile Uint32 timer_ticks = 0;

struct MainArgs {
    bool start;
    bool stop;
    int  port;
    bool nolock;
};

MainArgs parse_main_arguments(int, char*[]);



int main(int argc, char* argv[])
{
    MainArgs ma = parse_main_arguments(argc, argv);

    std::string lockfile = "/var/run/lixd.pid";

    if (ma.start) {
        if (ma.nolock) lockfile = "";
        int daem_ret = OS::daemonize(lockfile);
        if (daem_ret > 0) return 0;
        if (daem_ret < 0) return -1;

        enet_initialize();

        // Create server. Note that we use the number of updates per second as
        // the number of ticks per second: The network will track a timer that
        // runs slower than Lix's timer, even though server->calc() will get
        // called more often than Lix's main loop.
        NetServer* server = new NetServer(true, ma.port,
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

    else if (ma.stop) {
        OS::kill_daemon(lockfile);
        return 0;
    }

    else {
        std::cerr << "Usage:" << std::endl
         << "    lixd start [-p <port>] [--no-lock]" << std::endl
         << "    lixd stop" << std::endl
         << "See the file `doc/lixd.txt' for more information." << std::endl;
         return -1;
    }

}



// ############################################################################
// ############################################################################
// ############################################################################



MainArgs parse_main_arguments(int argc, char* argv[])
{
    MainArgs ret;
    ret.start = ret.stop = ret.nolock = false;
    ret.port  = globals_port_default;

    for (int i = 1; i < argc; ++i) {
        std::string s = argv[i];
        if      (i == 1 && s == "start") ret.start = true;
        else if (i == 1 && s == "stop" ) ret.stop  = true;
        else if (s == "-p" && ++i < argc) {
            std::istringstream portstring(argv[i]);
            if (! (portstring >> ret.port)) ret.port = globals_port_default;
        }
        else if (s == "--no-lock") ret.nolock = true;
    }

    return ret;
}


