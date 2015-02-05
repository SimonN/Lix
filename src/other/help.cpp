/*
 * libhelp.cpp
 *
 */

#include "../other/myalleg.h"
#include <cmath>   // Hypot: Brauche std::sqrt
#include <iomanip> // Fuer stringstream
#include <string>
#include <sstream>
#include <vector>

#include "help.h"

#include "../graphic/glob_gfx.h" // color[COL_BLACK]



namespace Help {

// Timer-Funktionen
volatile Uint32 timer_ticks            =  0;
const    int    timer_ticks_per_second = 60;

void timer_increment_ticks() {
    ++timer_ticks;
}
END_OF_FUNCTION(increment_ticks)

void timer_start() {
    LOCK_VARIABLE (timer_ticks);
    LOCK_FUNCTION (timer_increment_ticks);
    install_int_ex(timer_increment_ticks,
     BPS_TO_TIMER (timer_ticks_per_second));
}

unsigned int get_timer_ticks_per_draw() {
    if (get_refresh_rate() != 0) {
        const unsigned i = timer_ticks_per_second / get_refresh_rate();
        if (i > 0) return i;
        else       return 1;
    }
    // Dies nimmt notfalls an, dass der Monitor 60 Hz hat bei 120 Ticks/Sek.
    else return gloB->screen_vsync + 1;
}
// Ende der Timer-Funktionen



double hypot(const int x1, const int y1,
             const int x2, const int y2)
{
    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}



double random_double(const double min, const double max)
{
    // Umwandlung zu float reicht nicht, wenn wir durch (max - min) teilen,
    // ggf. ueberlaeuft dann der Double. Also lieber * 1.0 vorneweg.
    return (rand() * 1.0 / RAND_MAX) * (max - min) + min;
}



std::string version_to_string(const unsigned long nr) {
    std::ostringstream sstr;
    sstr << nr;
    std::string s = sstr.str();
    if (s.size() < 10) return s;
    s.insert( 4, "-");
    s.insert( 7, "-");
    s.insert(10, "-");
    if (nr % 100 == 0) s.resize(s.size() - 3);
    return s;
}



std::string scancode_to_string(const int sc)
{
    std::string ret = ::scancode_to_name(sc);
    string_to_nice_case(ret);
    for (size_t i = 0; i < ret.size(); ++i) {
        if (ret[i] == '_' && i + 1 < ret.size()) {
            ret.erase(i, 1);
            if (ret[i] >= 'a' && ret[i] <= 'z') ret[i] = ret[i] + 'A' - 'a';
        }
    }
    return ret;
}



static char single_hex_char(int i)
{
    if (i < 0xA) return '0' + i;
    else         return 'A' + i - 0xA;
}



std::string int_to_hex(int i)
{
    std::string ret;
    bool negative = false;
    if (i < 0) {
        negative = true;
        i *= -1;
    }
    if (i == 0) ret += '0';
    else while (i > 0) {
        const int digit = i % 0x10;
        ret.insert(ret.begin(), single_hex_char(digit));
        i /= 0x10;
    }
    ret = (negative ? "-0x" : "0x") + ret;
    return ret;
}



void string_to_nice_case(std::string& s) {
    if (s.empty()) return;
    if (s[0] >= 'a' && s[0] <= 'z') s[0] = s[0] + 'A' - 'a';
    for (std::string::iterator i = ++s.begin(); i != s.end(); ++i) {
        if (*i >= 'A' && *i <= 'Z') *i = *i + 'a' - 'A';
    }
}



void string_shorten(std::string& s, const FONT* ft, const int length) {
    if (text_length(ft, s.c_str()) > length) {
        while (text_length(ft, s.c_str()) > length - text_length(ft, "..."))
         Help::remove_last_utf8_char(s);
        s += "...";
    }
}



bool string_ends_with(const std::string& s, const std::string& tail)
{
    return s.size() >= tail.size()
     && std::string(s.end() - tail.size(), s.end()) == tail;
}



void move_iterator_utf8(std::string const& str,
                        std::string::const_iterator& iter,
                        int num_chars)
{
    if (num_chars > 0) {
        std::string::difference_type num_bytes_to_end = str.end() - iter;
        while (num_chars > 0 && iter != str.end()) {
            int num_bytes = uwidth(&*iter);
            if (num_bytes <= num_bytes_to_end) {
                iter += num_bytes;
                num_bytes_to_end -= num_bytes;
            } else {
                iter = str.end();
            }
            num_chars--;
        }
    }
    else if (num_chars < 0) {
        // Allegro's Unicode-handling functions do not efficiently
        // handle iterating backwards in UTF-8, which is rather
        // common for our needs (eg. truncating text being displayed)
        // so we'll do our own thing here.
        while (num_chars < 0 && iter != str.begin()) {
            --iter;
            char byte = *iter;
            // a valid UTF-8 encoding of a character always start with
            // bit-pattern 0xxxxxxx or 11xxxxxx in first byte
            if ((byte & 0x80) == 0 || (byte & 0xC0) == 0xC0) {
                num_chars++;
            }
        }
    }
}



void remove_last_utf8_char(std::string& str) {
    std::string::const_iterator iter = str.end();
    move_iterator_utf8(str, iter, -1);
    str.resize(iter - str.begin());
}



std::string make_utf8_seq(int codepoint) {
    int num_bytes = ucwidth(codepoint);
    char byte_sequence[num_bytes + 1]; // +1 for terminating 0
    usetc(byte_sequence, codepoint);
    byte_sequence[num_bytes] = 0;
    return std::string(byte_sequence);
}



std::string escape_utf8_with_ascii(const std::string& str)
{
    std::string ret;
    for (std::string::const_iterator itr = str.begin();
     itr != str.end(); move_iterator_utf8(str, itr, 1))
    {
        // we escape everything that is not ASCII, and even some ASCII that
        // doesn't fall into our range of acceptable filename characters
        const bool nice = (*itr >= 'a' && *itr <= 'z')
                       || (*itr >= 'A' && *itr <= 'Z')
                       || (*itr >= '0' && *itr <= '9');
        if (nice) {
            // add the ASCII char
            ret += *itr;
        }
        else {
            ret += '_';
            // print bytes in little endian, as they appear in the string
            const int len = ::uwidth(&*itr);
            if (len <= str.end() - itr)
             for (int i = 0; i < len; ++i) {
                // print each byte as a big-endian two-letter string
                const unsigned char byte = *(itr + i);
                ret += single_hex_char(byte / 0x10);
                ret += single_hex_char(byte % 0x10);
            }
        }
        // end else (! nice)
    }
    // end for itr, move_iterator_utf8
    return ret;
}



void draw_shaded_text(Torbit& bmp, FONT* f, const char* s,
 int x, int y, int r, int g, int b) {
    textout_ex(bmp.get_al_bitmap(), f, s, x+2, y+2, makecol(r/4, g/4, b/4),-1);
    textout_ex(bmp.get_al_bitmap(), f, s, x+1, y+1, makecol(r/2, g/2, b/2),-1);
    textout_ex(bmp.get_al_bitmap(), f, s, x  , y  , makecol(r  , g  , b  ),-1);
}
void draw_shadow_text(
 Torbit& bmp, FONT* f, const char* s, int x, int y, int c, int sc) {
    textout_ex(bmp.get_al_bitmap(), f, s, x+1, y+1, sc, -1);
    textout_ex(bmp.get_al_bitmap(), f, s, x  , y  , c,  -1);
}
void draw_shaded_centered_text(BITMAP *bmp, FONT* f, const char* s,
 int x, int y, int r, int g, int b) {
    textout_centre_ex(bmp, f, s, x+1, y+2, makecol(r/4, g/4, b/4), -1);
    textout_centre_ex(bmp, f, s, x  , y+1, makecol(r/2, g/2, b/2), -1);
    textout_centre_ex(bmp, f, s, x-1, y  , makecol(r  , g  , b  ), -1);
}
void draw_shadow_centered_text(
 Torbit& bmp, FONT* f, const char* s, int x, int y, int c, int sc) {
    textout_centre_ex(bmp.get_al_bitmap(), f, s, x+1, y+1, sc, -1);
    textout_centre_ex(bmp.get_al_bitmap(), f, s, x  , y  , c,  -1);
}
void draw_shadow_fixed_number(
 Torbit& bmp, FONT* f, int number, int x, int y,
 int c, bool right_to_left, int sc) {
    std::ostringstream s;
    s << number;
    draw_shadow_fixed_text(bmp,
     f, s.str(), x, y, c, right_to_left, sc);
}
// TODO: proper handling of UTF-8 string
// Currently no callers of draw_shadow_fixed_text() pass in
// non-ASCII string, so TODO can be deferred.
void draw_shadow_fixed_text(
 Torbit& bmp, FONT* f, const std::string& s, int x, int y,
 int c, bool right_to_left, int sc) {
    char ch[2]; ch[1] = '\0';
    for (std::string::const_iterator i
     =  (right_to_left ? --s.end() : s.begin());
     i!=(right_to_left ? --s.begin() : s.end());
        (right_to_left ? --i : ++i))
    {
        if (right_to_left) x -= 10;
        ch[0] = *i;
        if (ch[0] >= '0' && ch[0] <= '9')
             Help::draw_shadow_text         (bmp, f, ch, x,     y, c, sc);
        else Help::draw_shadow_centered_text(bmp, f, ch, x + 5, y, c, sc);
        if (!right_to_left) x += 10;
    }
}
void draw_shadow_fixed_updates_used(
 Torbit& bmp, FONT* f, int number, int x, int y,
 int c, bool right_left, int sc) {
    // Minuten
    std::ostringstream s;
    s << number / (gloB->updates_per_second * 60);
    s << ':';
    // Sekunden
    if  ((number / gloB->updates_per_second) % 60 < 10) s << '0';
    s << (number / gloB->updates_per_second) % 60;
    // s += useR->language == Language::GERMAN ? ',' : '.';
    // Sekundenbruchteile mit zwei Stellen
    // ...schreiben wir nicht. Wird zu unuebersichtlich.
    // int frac = number % Gameplay::updates_per_second
    //            * 100  / Gameplay::updates_per_second;
    // if (frac < 10) s += '0';
    // s += frac;
    draw_shadow_fixed_text(bmp, f, s.str(), x, y, c, right_left, sc);
}
// Ende der Schattentexte



// Dateisuchfunktionen in verschiedenen Variationen
void find_files(
    const Filename& fn_where,
    const std::string& what,
    DoStr              dostr,
    void*              from
) {
    std::string where = fn_where.get_rootful();
    al_ffblk info;
    std::string search_for = where + what;
    if (al_findfirst(search_for.c_str(), &info,
     FA_RDONLY | FA_HIDDEN | FA_LABEL | FA_ARCH) == 0) {
        do {
            // Gefundene Datei zum Vektor hinzufügen
            Filename fn_result(where + info.name);
            dostr(fn_result, from);
        } while (al_findnext(&info) == 0);
        al_findclose(&info);
    }
}
// Ende der Dateisuche



void find_dirs(const Filename& fn_where, DoStr dostr, void* from)
{
    al_ffblk info;
    std::string where = fn_where.get_rootful();
    if (where[where.size()-1] != '/') where += '/';
    if (where[where.size()-1] != '*') where += '*';
    if (al_findfirst(where.c_str(), &info,
     FA_RDONLY | FA_HIDDEN | FA_LABEL | FA_DIREC | FA_ARCH) == 0) {
        do {
            // Gefundenes Verzeichnis hinzufügen
            if ((info.attrib & FA_DIREC) == FA_DIREC && info.name[0] != '.') {
                std::string s = where;
                s.resize(s.size() -1 ); // * von der Maske abschnibbeln
                s += info.name;
                s += '/';
                Filename fn_result(s);
                dostr(fn_result, from);
            }
        } while (al_findnext(&info) == 0);
        al_findclose(&info);
    }
}
// Ende der Pfadsuche



void find_tree
(const Filename& fn_where, const std::string& what, DoStr dostr, void* from)
{
    std::string where = fn_where.get_rootful();
    // Nach Verzeichnissen suchen
    al_ffblk info;
    if (where[where.size()-1] != '/') where += '/';
    std::string search_for = where + '*';
    if (al_findfirst(search_for.c_str(), &info,
     FA_RDONLY | FA_HIDDEN | FA_LABEL | FA_DIREC | FA_ARCH) == 0) {
        do {
            // Dies nur für jedes Verzeichnis außer . und .. ausführen:
            // Neue Suche mit gleichem Vektor im gefundenen Verzeichnis
            if ((info.attrib & FA_DIREC) == FA_DIREC && info.name[0] != '.') {
                Filename fn_recurs(where + info.name + '/');
                find_tree(fn_recurs, what, dostr, from);
            }
        } while (al_findnext(&info) == 0);
        al_findclose(&info);
    }
    // Nach Dateien suchen, die dem Suchkriterium entsprechen
    Filename fn_where_with_slash(where);
    find_files(fn_where_with_slash, what, dostr, from);
}
// Ende der Unterverzeichnis-einschließenden Dateisuche



bool dir_exists(const Filename& fn)
{
    std::string dir = fn.get_rootful();
    if (dir.size() > 0 && dir[dir.size() - 1] == '/')
        dir.erase(--dir.end());
    // Allegro's file_exits sucks, you have to check twice like the following
    // if you want to find all directories, no matter whether FA_ARCH or not
    const bool a = file_exists(dir.c_str(), FA_ALL, 0);
    const bool b = file_exists(dir.c_str(), FA_ALL & ~ FA_DIREC, 0);
    return a && ! b;
}

}
// Ende des Namensraumes
