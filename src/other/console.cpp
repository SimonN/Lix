/*
 * other/console.cpp
 *
 */

#include "console.h"
#include "log.h"

const int Console::lines_max_number(8);
const int Console::lines_max_ticks (10 * Help::timer_ticks_per_second);

Console::Console()
:
    lines_maxl(LEMSCR_X - 40 - 5)
{
}

Console::~Console()
{
}

// Einziges Objekt erschaffen
Console Console::con;



const std::list <Console::Line>& Console::get_lines_all()
{
    while ((int) con.line.size() > con.lines_max_number) con.line.pop_front();
    return con.line;
}



const std::list <Console::Line>& Console::get_lines_recent()
{
    while (!con.line.empty()
     && ((int) con.line.size() > con.lines_max_number
     ||  con.line.begin()->time + con.lines_max_ticks < Help::timer_ticks))
     con.line.pop_front();
    return con.line;
}



// Zeilenumbruch-Regeln sind hier hartkodiert, siehe Headerkommentar
void Console::push_back(const std::string& t, const bool w)
{
    if (text_length(font_med, t.c_str()) > con.lines_maxl) {
        std::string line;
        std::string::const_iterator i = t.begin();
        while (i != t.end()) {
            line += *i;
            if (text_length(font_med, line.c_str()) > con.lines_maxl) {
                line.erase(--line.end());
                con.line.push_back(Line(line, w));
                line = "  ";
            }
            else ++i;
        }
        con.line.push_back(Line(line, w));
    }
    else con.line.push_back(Line(t, w));
    // Log::log(Log::INFO, t);
}
