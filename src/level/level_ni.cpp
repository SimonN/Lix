/*
 * Loading the Lemmini file format
 *
 */

#include <fstream>
#include <map>
#include <sstream>

#include "level.h"
#include "obj_lib.h"

#include "../other/help.h"

struct LemminiLine {
    std::string name; // what's before the = sign
    std::string str;  // everything behind the = sign
    int nr, x, y, flags; // additionally, str parsed into numerals

    LemminiLine() : nr(0), x(0), y(0), flags(0) {}

    LemminiLine(const std::string& line)
    :
        nr(0), x(0), y(0), flags(0)
    {
        size_t i = 0;
        // parse the name before the '=';
        while (i < line.size()) {
            const char c = line[i++];
            if (c == '=') {
                while (! name.empty() && name[name.size() - 1] == ' ')
                    name.resize(name.size() - 1);
                break;
            }
            else name += c;
        }
        // skip spaces after the '='
        while (i < line.size() && line[i] == ' ') ++i;

        // parse str and the numbers at the same time
        int  commas_seen = 0;
        bool minus       = false;
        while (i < line.size()) {
            const char c = line[i++];
            str += c;
            int* iptr = commas_seen == 0 ? &nr
                      : commas_seen == 1 ? &x
                      : commas_seen == 2 ? &y : &flags;
            if (c == ',') {
                if (minus) *iptr *= -1;
                minus = false;
                ++commas_seen;
            }
            else if (c == '-') minus = true;
            else if (c >= '0' && c <= '9') {
                *iptr *= 10;
                *iptr += c - '0';
            }
        }
    }
};



void Level::load_from_lemmini(const Filename& filename)
{
    typedef std::map <std::string, LemminiLine> Lines;
    Lines lines;

    std::ifstream file(filename.get_rootful().c_str());
    if (! file.good()) {
        status = BAD_FILE_NOT_FOUND;
        return;
    }

    // safely parse into lines, see other/file/io.cpp for comments.
    // ignore lines starting with #.
    std::string s;
    char        c;
    while (file.get(c)) {
        if (c != '\n' && c != '\r') s += c;
        else if (! s.empty()) {
            if (s[0] != '#') {
                LemminiLine ll(s);
                lines[ll.name] = ll;
            }
            s.clear();
        }
    }
    // No newline at end of file shall not matter
    if (! s.empty() && s[0] != '#') {
        LemminiLine ll(s);
        lines[ll.name] = ll;
    }

    file.close();

    // parse the contents of the LemminiLines

    name_english = lines["name"].str;
    size_x       = 2 * 1600;
    size_y       = 2 * 160;
    start_x      = lines["xPos"].nr;

    seconds       = std::max(lines["timeLimit"].nr * 60,
                             lines["timeLimitSeconds"].nr);
    initial       = lines["numLemmings"].nr;
    required      = lines["numToRescue"].nr;
    spawnint_slow = 4 + Help::even(99 - lines["releaseRate"].nr) / 2;
    spawnint_fast = 4;

    skills[LixEn::CLIMBER]  = lines["numClimbers"].nr;
    skills[LixEn::FLOATER]  = lines["numFloaters"].nr;
    skills[LixEn::EXPLODER] = lines["numBombers"].nr;
    skills[LixEn::BLOCKER]  = lines["numBlockers"].nr;
    skills[LixEn::BUILDER]  = lines["numBuilders"].nr;
    skills[LixEn::BASHER]   = lines["numBashers"].nr;
    skills[LixEn::MINER]    = lines["numMiners"].nr;
    skills[LixEn::DIGGER]   = lines["numDiggers"].nr;
    // skill slots with 0 skills will be culled in the finalize function

    legacy_ac_vec.push_back(LixEn::CLIMBER);
    legacy_ac_vec.push_back(LixEn::FLOATER);
    legacy_ac_vec.push_back(LixEn::EXPLODER);
    legacy_ac_vec.push_back(LixEn::BLOCKER);
    legacy_ac_vec.push_back(LixEn::BUILDER);
    legacy_ac_vec.push_back(LixEn::BASHER);
    legacy_ac_vec.push_back(LixEn::MINER);
    legacy_ac_vec.push_back(LixEn::DIGGER);

    int graphics_set = 0;
    std::string sty = lines["style"].str;
    Help::string_to_nice_case(sty);
    if      (sty == "Dirt")    graphics_set = 0;
    else if (sty == "Fire")    graphics_set = 1;
    else if (sty == "Marble")  graphics_set = 2;
    else if (sty == "Pillar")  graphics_set = 3;
    else if (sty == "Crystal") graphics_set = 4;
    else if (sty == "Brick")   graphics_set = 5;
    else if (sty == "Rock")    graphics_set = 6;
    else if (sty == "Snow")    graphics_set = 7;
    else if (sty == "Bubble")  graphics_set = 8;
    else if (sty == "Special") graphics_set = 9;

    // Parse the terrain

    int piece_nr = 0;
    while (true) {
        std::ostringstream piece_str;
        piece_str << "terrain_" << piece_nr;
        Lines::const_iterator itr = lines.find(piece_str.str());
        if (itr == lines.end()) break;
        else {
            Pos ter;
            ter.ob   = ObjLib::get_orig_terrain(graphics_set, itr->second.nr);
            ter.x    = itr->second.x;
            ter.y    = itr->second.y;
            ter.noow = itr->second.flags & 8;
            ter.mirr = itr->second.flags & 4;
            ter.dark = itr->second.flags & 2;
            if (ter.ob) pos[Object::TERRAIN].push_back(ter);
            else status = BAD_IMAGE;
        }
        ++piece_nr;
    }

    // parse the special objects

    piece_nr = 0;
    while (true) {
        std::ostringstream piece_str;
        piece_str << "object_" << piece_nr;
        Lines::const_iterator itr = lines.find(piece_str.str());
        if (itr == lines.end()) break;
        else {
            Pos ter;
            ter.ob = ObjLib::get_orig_special(graphics_set, itr->second.nr);
            ter.x  = itr->second.x;
            ter.y  = itr->second.y;
            if (ter.ob) {
                // Do not load/show the waving green and blue flags.
                if (ter.ob->type != Object::DECO || ter.ob->subtype != 1)
                    pos[ter.ob->type].push_back(ter);
            }
            else status = BAD_IMAGE;
        }
        ++piece_nr;
    }

    load_finalize_binary_or_lemmini(filename);
}
