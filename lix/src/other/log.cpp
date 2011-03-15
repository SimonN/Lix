/*
 * log.cpp
 *
 */

#include "help.h"     // timer_ticks
#include "log.h"
#include "globals.h"
#include "language.h" // Error and Info text

#include <cstdio>     // sprintf

Log* Log::singl(0);



void Log::initialize()
{
    if (!singl) singl = new Log;
}



void Log::deinitialize()
{
    if (singl) {
        delete singl;
        singl = 0;
    }
}



Log::Log()
:
    file(gloB->file_log.c_str(), std::ios::app)
{
    priority = INFO;
    something_was_logged_already_this_session = false;
}



Log::~Log()
{
    file.close();
}



void Log::log(Priority p, const std::string& s) {
    if ((singl->priority == ERROR && p == ERROR)
     || (singl->priority == INFO && p == ERROR)
     || (singl->priority == INFO && p == INFO)) {

        char time[10]; // Time string, will be written later
        sprintf(time, "%9.2f", (float) Help::timer_ticks
                                     / Help::timer_ticks_per_second);
        // Free line between to L++ programm sessions
        if (!singl->something_was_logged_already_this_session) {
             singl->something_was_logged_already_this_session = true;
             singl->file << std::endl;
        }
        // Output the line
        singl->file << time << ' ';
        if (p == ERROR) singl->file << Language::log_error;
        else            singl->file << Language::log_info;
        singl->file << ' ' << s << std::endl;
    }
}

void Log::log(Priority p, int i) {
    if ((singl->priority == ERROR && p == ERROR)
     || (singl->priority == INFO  && p == ERROR)
     || (singl->priority == INFO  && p == INFO)) {

        char time[10];
        sprintf(time, "%9.2f", (float) Help::timer_ticks
                                     / Help::timer_ticks_per_second);
        if (!singl->something_was_logged_already_this_session) {
             singl->something_was_logged_already_this_session = true;
             singl->file << std::endl;
        }
        singl->file << time << ' ';
        if (p == ERROR) singl->file << Language::log_error;
        else            singl->file << Language::log_info;
        singl->file << ' ' << i << std::endl;
    }
}

void Log::log(Priority p, const std::string& s, int i) {
    if ((singl->priority == ERROR && p == ERROR)
     || (singl->priority == INFO  && p == ERROR)
     || (singl->priority == INFO  && p == INFO)) {

        char time[10];
        sprintf(time, "%9.2f", (float) Help::timer_ticks
                                     / Help::timer_ticks_per_second);
        if (!singl->something_was_logged_already_this_session) {
             singl->something_was_logged_already_this_session = true;
             singl->file << std::endl;
        }
        singl->file << time << ' ';
        if (p == ERROR) singl->file << Language::log_error;
        else            singl->file << Language::log_info;
        singl->file << ' ' << s << ' ' << i << std::endl;
    }
}
