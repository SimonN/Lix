/*
 * log.h
 *
 * A static (singleton) object of Log is instatiated to log things into the
 * logging file.
 *
 * void Log::set_priority(Priority p)
 *
 *   Use priority level p. This can be:
 *
 *   Log::ERROR - log errors only
 *   Log::INFO  - log errors and informational messages.
 *
 * void Log::log(Priority, std::string&)
 *
 *   Logs the given string. If the given priority is set to Log::INFO and
 *   the overall log priority is set to Log::ERROR, the string doesn't get
 *   logged.
 *
 * void Log::log(std::string)
 *
 *   Short for Log::log(Log::INFO, s).
 *
 *   This takes a std::string instead of a std::string&, so it can be used
 *   with temporary strings. This is to aid developers' bug hunts.
 *
 * void Log::log(Priority, int)
 * void Log::log(int)
 * void Log::log(std::string, int)
 *
 *   Same as above, just with ints instead of strings.
 *
 *   The third function will log both the string and the int with error
 *   level Log::INFO implicit.
 *
 */

#pragma once

#include <fstream>
#include <string>

#undef ERROR // Enet sleeps with the preprocessor, we don't want to.

class Log {

public:

    enum Priority {
        ERROR,
        INFO
    };

private:

    static Log* singl;

    std::ofstream file;
    Priority priority;
    bool something_was_logged_already_this_session;

    Log();
    ~Log();

    void operator = (const Log&);

public:

    static void initialize();
    static void deinitialize();

    inline static void     set_priority(Priority p) { singl->priority = p; }
    inline static Priority get_priority()    { return singl->priority;     }

           static void log (Priority, int);
           static void log (Priority, const std::string&);
           static void log (Priority, const std::string&, int);
    inline static void log (          int i)                       { log(INFO, i);    }
    inline static void log (          const std::string& s)        { log(INFO, s);    }
    inline static void log (          const std::string& s, int i) { log(INFO, s, i); }



};
