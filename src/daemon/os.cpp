/*
 * os.cpp
 *
 */

#include <iostream> // debugging
#include <fstream>  // lock file

#include "os.h"

// ############################################################################

#ifdef _WIN32
    // nothing included yet
#endif
#ifdef __unix__
    #include <csignal>
    #include <cerrno>
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <sys/time.h>
#endif



namespace OS {

static std::string lockfilename;
static bool        signal_received = false;

#ifdef __unix__
    static bool    kill_daemon(::pid_t);
#endif



int daemonize(const std::string& argument_lockfilename)
{
#ifdef _WIN32

    std::cerr << "Daemonizing on Windows isn't implemented yet." << std::endl;
    std::cerr << "Exiting this console will shut down the server."<< std::endl;

    return 0;

#endif
#ifdef __unix__

    lockfilename = argument_lockfilename;

    bool lock = ! lockfilename.empty();

    // Check whether there is an instance that is already running.
    if (lock) {
        std::ifstream lockfile_exists(lockfilename.c_str());
        if (lockfile_exists.good()) {
            std::cerr
                << "Error: There seems to be an instance running already."
                << std::endl << "(If you're sure there isn't, erase `"
                << lockfilename << "'.)" << std::endl;
            lockfile_exists.close();
            return -1;
        }
        lockfile_exists.close();
    }

    // Fork off the parent process to spawn the daemon process.
    // pid <  0 is fork error, abort parent as well.
    // pid == 0 is child process, keep this running.
    // pid >  0 is parent process, exit this cleanly.
    ::pid_t pid = ::fork();

    if (pid < 0) {
        std::cerr << "Error: Can't use fork() to create daemon." << std::endl;
        return -1;
    }
    else if (pid > 0) {

        ::umask(033);

        if (lock) {

            // This is the parent process just before exiting, but it knows the
            // pid that the child has got. Write this into the lock file.
            std::ofstream lockfile_out(lockfilename.c_str());
            lockfile_out << pid << std::endl;
            lockfile_out.close();

            // Test successful creation
            std::ifstream lockfile_test_creation(lockfilename.c_str());
            if (! lockfile_test_creation.good()) {
                std::cerr << "Error: Cannot create `"
                    << lockfilename.c_str() << "'. Are you root?"
                    << std::endl << "(You can bypass the lock file with "
                    << "`--no-lock' if you really want to," << std::endl <<
                    "but please remember to kill the daemon manually later.)"
                    << std::endl;
                lockfile_test_creation.close();

                kill_daemon(pid);

                return -1;
            }
            lockfile_test_creation.close();
        }
        // Exit the parent process (caller will check return value and exit)
        return  1;
    }

    ::umask(033);

    ::pid_t sid = ::setsid();
    if (sid < 0) {
        std::cerr << "Error: Can't aquire a session ID." << std::endl;
        return -1;
    }

    if ((::chdir("/")) < 0) {
        std::cerr << "Error: Can't set working directory to /." << std::endl;
        return -1;
    }

	::signal(SIGCHLD, SIG_IGN); // ignore child
	::signal(SIGTSTP, SIG_IGN); // ignore tty signals
	::signal(SIGTTOU, SIG_IGN);
	::signal(SIGTTIN, SIG_IGN);
	::signal(SIGHUP,  &signal_handler); // catch hangup signal
	::signal(SIGTERM, &signal_handler); // catch kill signal

    // Close all file descriptors and reopen them as /dev/null
	for (int i = ::getdtablesize(); i >= 0; --i) ::close(i);
	int i = ::open("/dev/null", O_RDWR); // stdin
	::dup(i); // stdout
	::dup(i); // stderr

    return 0;

#endif
}



void yield()
{
#ifdef _WIN32

    // not implemented

#endif
#ifdef __unix__

   ::timeval timeout;
   timeout.tv_sec = 0;
   timeout.tv_usec = 10000;
   ::select(0, NULL, NULL, NULL, &timeout);

#endif
}



void update_timer(
    volatile Uint32* const timer_ticks,
    const int              ticks_per_second
) {
#ifdef _WIN32

    // not implemented
    *timer_ticks = 0;

#endif
#ifdef __unix__

    static ::timeval epoch = { 0, 0 };
    if (epoch.tv_sec == 0) gettimeofday(&epoch, 0);

    ::timeval now;
    gettimeofday(&now, 0);
    const long passed_sec  = now.tv_sec  - epoch.tv_sec;
    const long passed_usec = now.tv_usec - epoch.tv_usec;

    *timer_ticks = passed_sec  * ticks_per_second
                 + passed_usec * ticks_per_second / 1000000;

#endif
}



void signal_handler(int sig)
{
    sig = 0; // suppress warning due to unused parameters
    signal_received = true;
}



bool no_system_signal_received()
{
    return ! signal_received;
}



void free_lockfile()
{
#ifdef _WIN32

    // not implemented

#endif
#ifdef __unix__

    if (! lockfilename.empty()) ::remove(lockfilename.c_str());

#endif
}



#ifdef __unix__
static bool kill_daemon(const ::pid_t pid)
{
    if (pid < 2) return false;

    if ( ! ::kill(pid, SIGTERM)) return true;
    else {
        if (errno == EPERM) std::cerr <<
         "Error: Don't have permission to stop the daemon. Are you root?"
         << std::endl;
        else {
            std::cerr << "Error: No daemon running with process ID "
             << pid << "." << std::endl
             << "(If you still think there are running instances of "
             "the daemon," << std::endl << "kill them manually.)" << std::endl;
            free_lockfile();
        }
        return false;
    }

}
#endif



bool kill_daemon(const std::string& lf)
{
#ifdef _WIN32

    // not implemented

#endif
#ifdef __unix__

    lockfilename = lf;

    std::ifstream pidfile(lf.c_str());
    if ( ! pidfile.good()) {
        std::cerr << "Error: Missing or unreadable lock file `" << lf << "'."
         << std::endl << "(If you still think there are running instances of "
         "the daemon," << std::endl << "kill them manually.)" << std::endl;
        return false;
    }

    ::pid_t pid = 0;
    if (! (pidfile >> pid) || pid < 2) {
        std::cerr << "Error: No valid process ID in `" << lf << "'."
         << std::endl << "(If you still think there are running instances of "
         "the daemon," << std::endl << "kill them manually.)" << std::endl;
    }

    return kill_daemon(pid);

#endif
}

}
// end namespace OS
