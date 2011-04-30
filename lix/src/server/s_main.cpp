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

#include "../other/myalleg.h"

#include <iostream> // initial msg: this is the server, end it with keypress

#include "../other/globals.h"
#include "../network/server.h"

// Timer
volatile Uint32 timer_ticks = 0;

void timer_increment_ticks() {
    ++timer_ticks;
}
void timer_start() {
    LOCK_VARIABLE (timer_ticks);
    LOCK_FUNCTION (timer_increment_ticks);
    install_int_ex(timer_increment_ticks,
                   BPS_TO_TIMER(gloB->updates_per_second));
}



int main()
{
    allegro_init();
    install_keyboard(); // ESC terminates the server
    set_window_title("LixServ, [ESC] to quit");

    Globals::initialize();
    enet_initialize();

    timer_start();

    // Create server. Note that we use the number of updates per second as
    // the number of ticks per second: Our timer is slower than L++'s.
    NetServer* server = new NetServer(true, gloB->server_port,
     gloB->updates_per_second, &timer_ticks);
    std::cout << "This is the dedicated Lix server. Press [ESC] to exit."
              << std::endl;

    // Main loop
    while (!key[KEY_ESC]) {
        server->calc();
        rest(1);
    }
    delete server;

    enet_deinitialize();
    Globals::deinitialize();
    allegro_exit();

    return 0;
}
END_OF_MAIN()
