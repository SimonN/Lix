/*
 * level/level_io.cpp
 *
 * Loading and saving of levels
 *
 * For loading levels in the binary level format (LVL), see level_bi.cpp.
 *
 * Do not confuse this file with other/io.cpp. The latter file contains
 * functions for generic L++-style IO.
 *
 */

#include "level.h"
#include "obj_lib.h"

#include "../other/help.h"      // Prae-Extension heraussuchen
#include "../other/language.h"
#include "../other/log.h"

void Level::load_from_file(const Filename& filename)
{
    clear();
    status = GOOD;

    // load an original level from L1/ONML/...
    if (get_binary(filename)) {
        load_from_binary(filename);
    }
    // load something in the L++ text file format
    else {
        std::vector <IO::Line> lines;
        if (IO::fill_vector_from_file(lines, filename.get_rootful())) {
            load_from_vector(lines);
        }
        else status = BAD_FILE_NOT_FOUND;
    }
    load_finalize();
}



void Level::load_from_stream(std::istream& in)
{
    clear();
    status = GOOD;

    std::vector <IO::Line> lines;
    if (IO::fill_vector_from_stream(lines, in)) {
        load_from_vector(lines);
    }
    else status = BAD_FILE_NOT_FOUND;

    load_finalize();
}



void Level::load_from_vector(const std::vector <IO::Line>& lines)
{
    int sk = 0;

    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) switch(i->type) {
    // Strings setzen
    case '$':
        if      (i->text1 == gloB->level_built       )built       =i->text2;
        else if (i->text1 == gloB->level_author      )author      =i->text2;
        else if (i->text1 == gloB->level_name_german )name_german =i->text2;
        else if (i->text1 == gloB->level_name_english)name_english=i->text2;
        break;

    // Ganzzahlwert setzen
    case '#':
        if      (i->text1 == gloB->level_size_x  ) size_x   = i->nr1;
        else if (i->text1 == gloB->level_size_y  ) size_y   = i->nr1;
        else if (i->text1 == gloB->level_torus_x ) torus_x  = i->nr1;
        else if (i->text1 == gloB->level_torus_y ) torus_y  = i->nr1;
        else if (i->text1 == gloB->level_start_x ) start_x  = i->nr1;
        else if (i->text1 == gloB->level_start_y ) start_y  = i->nr1;
        else if (i->text1 == gloB->level_bg_red  ) bg_red   = i->nr1;
        else if (i->text1 == gloB->level_bg_green) bg_green = i->nr1;
        else if (i->text1 == gloB->level_bg_blue ) bg_blue  = i->nr1;
        else if (i->text1 == gloB->level_seconds ) seconds  = i->nr1;
        else if (i->text1 == gloB->level_initial ) initial  = i->nr1;
        else if (i->text1 == gloB->level_initial_legacy) initial  = i->nr1;
        else if (i->text1 == gloB->level_required) required = i->nr1;
        else if (i->text1 == gloB->level_rate    ) rate     = i->nr1;

        else if (i->text1 == gloB->level_count_neutrals_only)
                                            count_neutrals_only = i->nr1;
        else if (i->text1 == gloB->level_transfer_skills)
                                            transfer_skills     = i->nr1;

        // Auswertung der Fähigkeits-Anzahl und Erhöhung der Array-Füllzahl
        // Dies kann nicht durchlaufen werden, wenn die Fähigkeits-Slots
        // bereits voll sind.
        else if (sk < gloB->skill_max) {
            skill[sk].ac = LixEn::string_to_ac(i->text1);
            if (skill[sk].ac != LixEn::AC_MAX) {
                skill[sk].nr = i->nr1;
                ++sk;
            }
            // Nur Kaese wurde eingelesen, nicht einmal ein Faehigkeitsname?
            else skill[sk].ac = LixEn::NOTHING;
        }
        break;

    // Neues Objekt
    case ':':
        add_object_from_ascii_line(i->text1,
                                   i->nr1, i->nr2, i->text2);
        break;

    default:
        break;
    }

    // LEGACY SUPPORT: Alte Levels haben das Terrain falschherum notiert.
    // Das zuerst genannte Terrain ist dort ganz vorne, das spaetere drunter.
    // Daher einmal alles umdrehen.
    // Exclude the zero time Date("")! Saved original .LVLs have a time of 0.
    // In early 2011, the maximal number of skills was raised. Prior to that,
    // infinity was 100, and finite skill counts had to be <= 99. Afterwards,
    // infinity was -1, and the maximum skill count was 999.
    if (built != Date("")
     && built <  Date("2009-08-23 00:00:00")) pos[Object::TERRAIN].reverse();
    if (built != Date("")
     && built <  Date("2011-01-08 00:00:00")) {
        for (std::vector <Skill> ::iterator
         itr = skill.begin(); itr != skill.end(); ++itr) {
            if (itr->nr == 100) itr->nr = LixEn::infinity;
        }
    }
}



void Level::add_object_from_ascii_line(
    const std::string& text1,
    const int nr1,
    const int nr2,
    const std::string& text2
) {
    const Object* ob = ObjLib::get(text1);
    if (ob && ob->cb) {
        Level::Pos newpos;
        newpos.ob = ob;
        newpos.x  = nr1;
        newpos.y  = nr2;
        if (ob->type == Object::TERRAIN)
         for (std::string::const_iterator stritr = text2.begin();
         stritr != text2.end(); ++stritr) switch (*stritr) {
            case 'f': newpos.mirr = !newpos.mirr;         break;
            case 'r': newpos.rot  = (newpos.rot + 1) % 4; break;
            case 'd': newpos.dark = !newpos.dark;         break;
            case 'n': newpos.noow = !newpos.noow;         break;
        }
        else if (ob->type == Object::HATCH)
         for (std::string::const_iterator stritr = text2.begin();
         stritr != text2.end(); ++stritr) switch (*stritr) {
            case 'r': newpos.rot  = !newpos.rot;          break;
        }
        pos[ob->type].push_back(newpos);
    }
    // Kein Bild vorhanden
    else record_missing_image(text1);
}



void Level::record_missing_image(
    const std::string& image
) {
    status = BAD_IMAGE;
    // Fehlermeldung in die Logdatei schreiben
    std::string t;
    t = Language::log_level_unknown_bitmap + ' ' + image;
    Log::log(Log::ERROR, t);
}



void Level::load_finalize()
{

    // Einige Standards setzen, wenn die Ladewerte komisch sind
    if (size_x   < min_xl)              size_x   = min_xl;
    if (size_y   < min_yl)              size_y   = min_yl;
    if (size_x   > max_xl)              size_x   = max_xl;
    if (size_y   > max_yl)              size_y   = max_yl;
    if (initial  < 1)                   initial  = 1;
    if (initial  > 999)                 initial  = 999;
    if (required > initial)             required = initial;
    if (rate     < 1)                   rate     = 1;
    if (rate     > 99)                  rate     = 99;

    if (bg_red   < 0) bg_red   = 0; if (bg_red   > 255) bg_red   = 255;
    if (bg_green < 0) bg_green = 0; if (bg_green > 255) bg_green = 255;
    if (bg_blue  < 0) bg_blue  = 0; if (bg_blue  > 255) bg_blue  = 255;

    start_x = Help::mod(start_x, size_x);
    start_y = Help::mod(start_y, size_y);

    // Einige Fehler setzen
    // FNF wurde oben schon mit Abbrechen der Funktion gesetzt
    if (status == GOOD) {
        int count = 0;
        for (int type = Object::TERRAIN; type != Object::MAX; ++type)
         count += pos[type].size();
        if      (count == 0)                 status = BAD_EMPTY;
        else if (pos[Object::HATCH].empty()) status = BAD_HATCH;
        else if (pos[Object::GOAL ].empty()) status = BAD_GOAL;
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



void Level::save_to_file(const Filename& filename) const
{
    std::ofstream file(filename.get_rootful().c_str());
    file << *this;
    file.close();
}



std::ostream& operator << (std::ostream& out, const Level& l)
{
    Date best_built = l.built;
    // This is the date of the infinity change, see above in the load function.
    if (l.built < Date("2011-01-08 00:00:00")) best_built = Date("");

    out
     << IO::LineDollar(gloB->level_built,        best_built    )
     << IO::LineDollar(gloB->level_author,       l.author      )
     << IO::LineDollar(gloB->level_name_german,  l.name_german )
     << IO::LineDollar(gloB->level_name_english, l.name_english)

     << std::endl

     << IO::LineHash  (gloB->level_size_x,       l.size_x  )
     << IO::LineHash  (gloB->level_size_y,       l.size_y  )
     << IO::LineHash  (gloB->level_torus_x,      l.torus_x )
     << IO::LineHash  (gloB->level_torus_y,      l.torus_y )
     << IO::LineHash  (gloB->level_start_x,      l.start_x )
     << IO::LineHash  (gloB->level_start_y,      l.start_y )
     << IO::LineHash  (gloB->level_bg_red,       l.bg_red  )
     << IO::LineHash  (gloB->level_bg_green,     l.bg_green)
     << IO::LineHash  (gloB->level_bg_blue,      l.bg_blue )

     << std::endl

     << IO::LineHash  (gloB->level_seconds,      l.seconds )
     << IO::LineHash  (gloB->level_initial,      l.initial )
     << IO::LineHash  (gloB->level_required,     l.required)
     << IO::LineHash  (gloB->level_rate,         l.rate    )

//   << std::endl
//   << IO::LineHash(gloB->level_count_neutrals_only, l.count_neutrals_only)
//   << IO::LineHash(gloB->level_transfer_skills,     l.transfer_skills)

     << std::endl;

    for (int i = 0; i < gloB->skill_max; ++i) out
     << IO::LineHash  (LixEn::ac_to_string(l.skill[i].ac), l.skill[i].nr);

    // Erst Spezialobjekte, dann Terrain
    for (int type = Object::TERRAIN; type != Object::MAX; ++type) {
        if (type != Object::TERRAIN) out << l.pos[type];
    }
    out << l.pos[Object::TERRAIN];
    return out;
}



std::ostream& operator << (std::ostream& o, const Level::PosLi& li)
{
    if (!li.empty()) o << std::endl;
    for (Level::PosIt i = li.begin(); i != li.end(); ++i) {
        if (!i->ob) continue;

        std::string str = ObjLib::get_filename(i->ob);

        if (!str.empty()) {
            std::string modifiers;
            if (i->mirr) modifiers += 'f';
            for (int r = 0; r < i->rot; ++r) modifiers += 'r';
            if (i->dark) modifiers += 'd';
            if (i->noow) modifiers += 'n';
            o << IO::LineColon(str, i->x, i->y, modifiers);
        }
    }
    return o;
}



// ############################################################################
// ##################################### Statische Funktionen ausser Operatoren
// ############################################################################



bool Level::get_binary(const Filename& filename)
{
    if (! ::exists(filename.get_rootful().c_str())) return false;
    std::ifstream file(filename.get_rootful().c_str(), std::ios::binary);
    // the length check before the read() was necessary for me on Linux
    // to get the Debugger past this, it got stuck on read() when nothing
    // was wrong.
    file.seekg (0, std::ios::end);
    if (file.tellg() < 8) {
        file.close();
        return false;
    }
    file.seekg(0, std::ios::beg);
    unsigned char buf[8];
    file.read((char*) &buf, 8);
    file.close();

    // A binary file has two-byte numbers in big endian
    // for rate, lixes, required, seconds at the beginning.
    // Neither should be > 0x00FF. If all of them are,
    // this is an ASCII file which shouldn't have '\0' chars.
    if (buf[0] != '\0' && buf[2] != '\0' && buf[4] != '\0' && buf[6] != '\0')
         return false;
    else return true;
}



// A speedup of the whole loading functionn, this only extracts the name.
std::string Level::get_name(const Filename& filename)
{
    if (get_binary(filename)) return get_name_binary(filename);
    else                      return get_name_ascii (filename);
}



std::string Level::get_name_ascii(const Filename& filename)
{
    std::vector <IO::Line> lines;
    if (!IO::fill_vector_from_file(lines, filename.get_rootful())) {
        if (!filename.get_file().empty()) {
            std::string logstr = Language::log_file_not_found + ' '
                               + filename.get_rootless();
            Log::log(Log::ERROR, logstr);
        }
    }
    // File exists
    else {
        std::string g;
        std::string e;
        for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) {
            if (i->text1 == gloB->level_name_german)  g = i->text2;
            if (i->text1 == gloB->level_name_english) e = i->text2;
            if (!g.empty() && Language::get() == Language::GERMAN)  return g;
            if (!e.empty() && Language::get() == Language::ENGLISH) return e;
        }
        // String for the own language was empty. Return the other string.
        if (Language::get() == Language::GERMAN) return e; else return g;
    }
    // Nothing found at all
    return gloB->empty_string;
}



std::string Level::get_built(const Filename& filename)
{
    std::vector <IO::Line> lines;
    if (!IO::fill_vector_from_file(lines, filename.get_rootful())) {
        if (!filename.get_file().empty()) {
            std::string logstr = Language::log_file_not_found + ' '
                               + filename.get_rootless();
            Log::log(Log::ERROR, logstr);
        }
    }
    // File exists
    else for (IO::LineIt i = lines.begin(); i != lines.end(); ++i)
     if (i->text1 == gloB->level_built) return i->text2;

    // Nothing found
    return gloB->empty_string;
}
