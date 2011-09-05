/*
 * os.h
 *
 * Wrappers around platform specific code. This is mainly used in the server
 * daemon for yielding and timing.
 *
 */

#pragma once

#include <ostream>

#include "../network/net_t.h" // Uint32

namespace OS {

    // Fork the process so it keeps running in the background.
    // returns -1 on error, 0 on child, 1 on parent. You should exit
    // the caller when this returns != 0.
    // 1st argument: name of the lock file
    //               If empty string is supplied, no lock file is used,
    //               and the daemon may start in any case.
    int  daemonize(const std::string&);

    void yield();

    // First argument is the address of the timer ticks variable. That variable
    // will get updated to represent the number of passed ticks since the
    // first invocation of update_timer().
    // The second argument is the number of ticks per second. This defines
    // how long a tick is.
    void update_timer(volatile Uint32*, int);

    void signal_handler(int);

    // Use this to check in the main loop whether we got any system signals
    // Returns true when none have arrived, false otherwise. You should cleanly
    // exit your main loop if you got a signal.
    bool no_system_signal_received();

    void free_lockfile();

    // Returns true on a successful kill, false if nothing to kill or error.
    bool kill_daemon(const std::string&);

}
