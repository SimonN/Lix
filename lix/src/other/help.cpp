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

#include "user.h"
#include "help.h"
#include "log.h"

#include "../graphic/glob_gfx.h" // color[COL_BLACK]

namespace Help {

// Timer-Funktionen
ALLEGRO_TIMER* timer(0);
const int      timer_ticks_per_second(60);



void timer_initialize()
{
    if (timer) return;
    timer = al_create_timer(ALLEGRO_BPS_TO_SECS(timer_ticks_per_second));
    al_start_timer(timer);
}



void timer_deinitialize()
{
    if (!timer) return;
    al_stop_timer(timer);
    al_destroy_timer(timer);
    timer = 0;
}



int get_timer_ticks()
{
    return al_get_timer_count(timer);
}



double hypot(const int x1, const int y1,
             const int x2, const int y2)
{
    return std::sqrt((x2-x1)*(x2-x1) + (y2-y1)*(y2-y1));
}

int mod(int base, int modulo)
{
    if  (modulo < 0) modulo *= -1;
    return (base % modulo + modulo) % modulo;
}

double random_double(const double min, const double max)
{
    // Umwandlung zu float reicht nicht, wenn wir durch (max - min) teilen,
    // ggf. ueberlaeuft dann der Double. Also lieber * 1.0 vorneweg.
    return (rand() * 1.0 / RAND_MAX) * (max - min) + min;
}



MainArgs parse_main_arguments(int argc, char *argv[])
{
    MainArgs main_args;
    main_args.scr_f = !useR->screen_windowed;
    main_args.scr_x = 0;
    main_args.scr_y = 0;
    main_args.sound_load_driver = gloB->sound_load_driver;

    // Argumente parsen:
    // Es kommt derzeit nur darauf an, ob in allen Argumenten ein bestimmter
    // Buchstabe ist oder nicht.
    for (int i = 1; i < argc; ++i)
     for (unsigned pos = 0; argv[i][pos] != '\0'; ++pos)
     switch (argv[i][pos]) {
    case 'w':
        main_args.scr_f = false;
        main_args.scr_x = LEMSCR_X;
        main_args.scr_y = LEMSCR_Y;
        break;

    case 'n':
        // Die globale Konfigdatei sowie die Userdatei des letzten Benutzers
        // wurden bereits geladen. Damit dennoch die Nachfrage zu Beginn
        // kommt, killen wir den User wieder.
        gloB->user_name = "";
        useR->load();
        break;

    case 'o':
        main_args.sound_load_driver = false;
        break;

    default:
        break;
    }
    return main_args;
}
// Ende von parse_main_arguments



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
void string_to_nice_case(std::string& s) {
    for (std::string::iterator i = ++s.begin(); i != s.end(); ++i) {
        if (*i >= 'A' && *i <= 'Z') *i = *i + 'a' - 'A';
    }
}



void string_remove_extension(std::string& s) {
    for (std::string::iterator i = --s.end(); i != --s.begin()
                                           && i !=   s.begin(); --i) {
        if (*i == '.') {
            s.erase(i, s.end());
            break;
        }
        else if (*i == '/') break;
}   }
void string_remove_dir(std::string& s) {
    std::string::iterator i = s.end();
    if (i != s.begin()) --i;
    if (i != s.begin() && *i == '/') --i; // Falls selbst ein Verzeichnis
    for (; i != --s.begin(); --i) {
        if (*i == '/') {
            s.erase(s.begin(), ++i);
            break;
}   }   }
void string_cut_to_dir(std::string& s) {
    for (std::string::iterator i = --s.end(); i != --s.begin(); --i) {
        if (*i == '/') {
            s.erase(++i, s.end());
            break;
}   }   }
void string_shorten(std::string& s, const ALLEGRO_FONT* ft, const int length) {
    if (al_get_text_width(ft, s.c_str()) > length) {
        while (al_get_text_width(ft, s.c_str())
         > length - al_get_text_width(ft, "...")) {
            s.resize(s.size() - 1);
        }
        s += "...";
    }
}



char string_get_pre_extension(const std::string& s)
{
    // Schaue, ob das letzte Zeichen eines endungslosen Strings gesucht ist
    std::string::const_iterator itr = --s.end();
    if (*--itr == '.') return *++itr;

    // Sonst: Durchsuche einen String mit Endung nach der Prae-Endung
    else for (itr = --s.end(); itr != --s.begin(); --itr) {
        if (*itr == '.') {
            if (--itr == --s.begin()) return '\0';
            if (--itr == --s.begin()) return '\0';
            else if (*itr == '.')     return *++itr; // "bla.A.txt" -> 'A'
            else                      return '\0';   // "bla.txt"   -> '\0'
            break;
        }
    }
    return '\0'; // Falls nicht einmal '.' enthalten
}

std::string string_get_extension(const std::string& s)
{
    std::string ext = "";
    std::string::const_iterator itr = --s.end();
    while (itr != s.begin() && itr != --s.begin() && *itr != '.') --itr;
    while (itr != s.end()   && itr != --s.begin()) {
        ext += *itr;
        ++itr;
    }
    return ext;
}

bool string_ends_with(const std::string& s, const std::string& tail)
{
    return s.size() >= tail.size()
     && std::string(s.end() - tail.size(), s.end()) == tail;
}



// ############################################################################
// ############################################################################
// ############################################################################



void draw_shadow_text(
    Torbit& bmp,
    ALLEGRO_FONT* f,
    const char* s,
    const int x,
    const int y,
    const ALLEGRO_COLOR& col
) {
    const ALLEGRO_COLOR& sha = color[COL_API_SHADOW];
    al_set_target_bitmap(bmp.get_al_bitmap());
    al_draw_text(f, sha, x+1, y+1, ALLEGRO_ALIGN_LEFT, s);
    al_draw_text(f, col, x,   y,   ALLEGRO_ALIGN_LEFT, s);
}



void draw_shadow_centered_text(
    Torbit& bmp,
    ALLEGRO_FONT* f,
    const char* s,
    const int x,
    const int y,
    const ALLEGRO_COLOR& col
) {
    const ALLEGRO_COLOR& sha = color[COL_API_SHADOW];
    al_set_target_bitmap(bmp.get_al_bitmap());
    al_draw_text(f, sha, x+1, y+1, ALLEGRO_ALIGN_CENTRE, s);
    al_draw_text(f, col, x,   y,   ALLEGRO_ALIGN_CENTRE, s);
}



void draw_shadow_fixed_number(
    Torbit& bmp,
    ALLEGRO_FONT* f,
    const int number,
    const int x,
    const int y,
    const ALLEGRO_COLOR& col,
    const bool right_to_left
) {
    std::ostringstream s;
    s << number;
    draw_shadow_fixed_text(bmp, f, s.str(), x, y, col, right_to_left);
}



void draw_shadow_fixed_text(
    Torbit& bmp,
    ALLEGRO_FONT* f,
    const std::string& s,
          int x,
    const int y,
    const ALLEGRO_COLOR& col,
    const bool right_to_left
) {
    char ch[2]; ch[1] = '\0';
    for (std::string::const_iterator i
     =  (right_to_left ? --s.end() : s.begin());
     i!=(right_to_left ? --s.begin() : s.end());
        (right_to_left ? --i : ++i))
    {
        if (right_to_left) x -= 10;
        ch[0] = *i;
        if (ch[0] >= '0' && ch[0] <= '9')
             Help::draw_shadow_text         (bmp, f, ch, x,     y, col);
        else Help::draw_shadow_centered_text(bmp, f, ch, x + 5, y, col);
        if (!right_to_left) x += 10;
    }
}



void draw_shadow_fixed_updates_used(
    Torbit& bmp,
    ALLEGRO_FONT* f,
    const int number,
    const int x,
    const int y,
    const ALLEGRO_COLOR& col,
    const bool right_left
) {
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
    draw_shadow_fixed_text(bmp, f, s.str(), x, y, col, right_left);
}
// Ende der Schattentexte



// ############################################################################
// ############################################################################
// ############################################################################



// Find files with a given extension
void find_files(
    const std::string& where,
    const std::string& what,
    DoStr              dostr,
    void*              from
) {
    ALLEGRO_FS_ENTRY* dir = al_create_fs_entry(where.c_str());
    if (al_open_directory(dir)) {
        ALLEGRO_FS_ENTRY* file;
        while (0 != (file = al_read_directory(dir))) {
            // Don't care for directories
            if ( ! (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR)) {
                std::string s = al_get_fs_entry_name(file);
                bool file_is_wanted = true;

                // Check whether ``what'' is equal the end of the found name
                if (what.empty()) file_is_wanted = true;
                else if (s.size() < what.size()) file_is_wanted = false;
                else {
                    std::string s_tail(s, s.size() - what.size(), what.size());
                    file_is_wanted = (s_tail == what);
                }
                if (file_is_wanted) dostr(s, from);
            }
            al_destroy_fs_entry(file);
        } al_close_directory(dir);
    } al_destroy_fs_entry(dir);
}



void find_dirs(
    const std::string& where,
    DoStr dostr,
    void* from
) {
    // nearly all of it is copied from ``find_files'' above.
    ALLEGRO_FS_ENTRY* dir = al_create_fs_entry(where.c_str());
    if (al_open_directory(dir)) {
        ALLEGRO_FS_ENTRY* file;
        while (0 != (file = al_read_directory(dir))) {
            if (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR) {
                std::string s = al_get_fs_entry_name(file);
                dostr(s, from);
            }
            al_destroy_fs_entry(file);
        } al_close_directory(dir);
    } al_destroy_fs_entry(dir);
}
// end of find_dirs



void find_tree(
    const std::string& where,
    const std::string& what,
    DoStr dostr,
    void* from
) {
    // Search the root dir ``where'' first. This is necessary since
    // the directory ``.'' isn't found by al_read_directory.
    find_files(where, what, dostr, from);

    // Recursively call ``find_tree'' for each subdirectory.
    ALLEGRO_FS_ENTRY* dir = al_create_fs_entry(where.c_str());
    if (al_open_directory(dir)) {
        ALLEGRO_FS_ENTRY* file;
        while (0 != (file = al_read_directory(dir))) {
            if (al_get_fs_entry_mode(file) & ALLEGRO_FILEMODE_ISDIR) {
                std::string subdir = al_get_fs_entry_name(file);
                find_tree(subdir, what, dostr, from);
                // There is no infinite recursion here. ``.'' isn't found
                // by al_read_directory.
            }
            al_destroy_fs_entry(file);
        } al_close_directory(dir);
    } al_destroy_fs_entry(dir);
}
// end of find_tree.




bool dir_exists(const std::string& s)
{
    std::string dir = s;
    if (dir[dir.size() - 1] == '/') dir.erase(--dir.end());

    bool ret = false;
    ALLEGRO_FS_ENTRY* handle = al_create_fs_entry(dir.c_str());
    if (al_open_directory(handle)) ret = true;
    al_destroy_fs_entry(handle);
    return ret;
}



bool file_exists(const std::string& s)
{
    std::string dir = s;
    if (dir[dir.size() - 1] == '/') dir.erase(--dir.end());

    bool ret = false;
    ALLEGRO_FS_ENTRY* handle = al_create_fs_entry(dir.c_str());
    if (al_fs_entry_exists(handle)) ret = true;
    al_destroy_fs_entry(handle);
    return ret;
}

}
// Ende des Namensraumes
