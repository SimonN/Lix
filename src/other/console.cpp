/*
 * other/console.cpp
 *
 */

#include "console.h"

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
            std::string::size_type old_size_in_bytes = line.size();
            std::string::const_iterator old_i = i;
            int numBytesInUtf8Char = uwidth(&*i);
            for(int n = 0; n < numBytesInUtf8Char && i != t.end() ; ++n, ++i) {
                line += *i;
            }
            if (text_length(font_med, line.c_str()) > con.lines_maxl) {
                line.resize(old_size_in_bytes);
                i = old_i;
                con.line.push_back(Line(line, w));
                line = "  ";
            }
        }
        con.line.push_back(Line(line, w));
    }
    else con.line.push_back(Line(t, w));
    // Log::log(Log::INFO, t);
}



void Console::break_lines(
    std::vector <std::string>& vec,
    const std::string& source,
    FONT* fo,
    const int max_line_length
) {
    std::string::const_iterator itr        = source.begin();
    std::string::const_iterator line_start = itr;
    std::string::const_iterator word_start = itr; // first letter of new word

    while (itr < source.end() + 1) {

        const std::string cur_line (line_start, itr);
        const std::string next_four(itr,        itr + 4);
        const int cur_length = ::text_length(fo, cur_line.c_str());

        // put any final line which didn't reach any other linebreaking
        // criterion into the vector. Then end the breaking algorithm.
        if (itr == source.end()) {
            vec.push_back(cur_line);
            break;
        }
        // hard line breaks wanted by the writer
        else if (next_four == "<br>") {
            vec.push_back(cur_line);
            itr += 4;
            line_start = itr;
            word_start = itr;
        }
        // linebreaks after a word, in extreme cases after a letter
        else if (cur_length > max_line_length) {
            // break after a word
            if (word_start != line_start) {
                vec.push_back(std::string(line_start, word_start));
                itr        = word_start; // go back to beginning of cur word
                line_start = itr;
            }
            // break after the last letter before the current word of
            // line-length has reached its end
            else if (itr > (line_start + 1)) {
                Help::move_iterator_utf8(source, itr, -1);
                vec.push_back(std::string(line_start, itr));
                line_start = itr;
                word_start = itr;
            }
            // for some reason, we are stuck, itr doesn't advance. This might
            // happen if there is a letter wider than the allowed line length.
            // Kill off the word-breaking process entirely.
            else {
                break;
            }
        }
        // no break here, but maybe a new word begins
        // increment the iterator in any case
        else {
            bool cur_was_space = (*itr == ' ');
            Help::move_iterator_utf8(source, itr, +1);
            if (itr < source.end() && cur_was_space && *itr != ' ') {
                word_start = itr;
            }
        }
    }

    // remove empty trailing lines
    while ( ! vec.empty() && vec.back().empty()) vec.pop_back();
}



