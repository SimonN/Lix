// level/level_me.cpp

#include "level_me.h"
#include "level.h"
#include "../other/language.h"

LevelMetaData::LevelMetaData()
:
    format(FORMAT_NOTHING)
{
}



LevelMetaData::LevelMetaData(const Filename& fn)
{
    format = static_cast <FileFormat> (Level::get_file_format(fn));

    if      (format == FORMAT_LIX)     read_metadata_lix    (fn);
    else if (format == FORMAT_BINARY)  read_metadata_binary (fn);
    else if (format == FORMAT_LEMMINI) read_metadata_lemmini(fn);

    if (format == FORMAT_BINARY || format == FORMAT_LEMMINI)
        built = Date("0");
}



const std::string& LevelMetaData::get_name() const
{
    if (Language::get() == Language::GERMAN)
         return name_german .empty() ? name_english : name_german;
    else return name_english.empty() ? name_german  : name_english;
}



bool LevelMetaData::get_file_exists() const
{
    return format != FORMAT_NOTHING;
}



void LevelMetaData::read_metadata_lix(const Filename& fn)
{
    std::vector <IO::Line> lines;
    if (!IO::fill_vector_from_file(lines, fn.get_rootful())) {
        return;
    }
    // File exists
    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) {
        if (i->text1 == gloB->level_built)        built        = i->text2;
        if (i->text1 == gloB->level_name_german)  name_german  = i->text2;
        if (i->text1 == gloB->level_name_english) name_english = i->text2;
        if (i->text1 == gloB->level_initial)      initial      = i->nr1;
        if (i->text1 == gloB->level_required)     required     = i->nr1;
    }
}



// these functions are defined in level_bi.cpp
std::string read_level_name_bytes (std::ifstream&);
int         read_two_bytes_levelbi(std::ifstream&);


void LevelMetaData::read_metadata_binary(const Filename& fn)
{
    std::ifstream file(fn.get_rootful().c_str(), std::ios::binary);

    // see level_bi.cpp for documentation of the L1 format
    file.seekg(0x2);
    initial  = read_two_bytes_levelbi(file);
    required = read_two_bytes_levelbi(file);
    name_english = read_level_name_bytes(file);
    file.close();
}



void LevelMetaData::read_metadata_lemmini(const Filename& fn)
{
    std::ifstream file(fn.get_rootful().c_str());
    if (! file.good()) return;

    // File exists
    std::string s;
    while (file >> s) {
        if (s == "name") {
            file >> s; // parse the "=";
            s.clear();
            char c;
            while (file.get(c)) {
                if (c == ' ' && s.empty()); // discard spaces before name
                else if (c == '\n' || c == '\r') break; // done reading name
                else s += c;
            }
            name_english = s;
        }
        else if (s == "numLemmings") {
            file >> s; // parse the "="
            file >> initial;
        }
        else if (s == "numToRescue") {
            file >> s; // parse the "="
            file >> required;
        }
    }
    file.close();
}
