/*
 * lemming/lem_enum.cpp
 *
 */

#include "../other/myalleg.h" // makecol

#include <map>
#include <vector>

#include "lix_enum.h"

namespace LixEn {

std::map    <Ac,    std::string> ac_string;
std::map    <Style, std::string> style_string;



void initialize()
{
    ac_string   [NOTHING]           = "NOTHING";
    ac_string   [FALLER]            = "FALLER";
    ac_string   [TUMBLER]           = "TUMBLER";
    ac_string   [STUNNER]           = "STUNNER";
    ac_string   [LANDER]            = "LANDER";
    ac_string   [SPLATTER]          = "SPLATTER";
    ac_string   [BURNER]            = "BURNER";
    ac_string   [DROWNER]           = "DROWNER";
    ac_string   [EXITER]            = "EXITER";
    ac_string   [WALKER]            = "WALKER";

    ac_string   [CLIMBER]           = "CLIMBER";
    ac_string   [ASCENDER]          = "ASCENDER";
    ac_string   [FLOATER]           = "FLOATER";
    ac_string   [EXPLODER]          = "EXPLODER";
    ac_string   [EXPLODER2]         = "EXPLODER2";
    ac_string   [BLOCKER]           = "BLOCKER";
    ac_string   [BUILDER]           = "BUILDER";
    ac_string   [SHRUGGER]          = "SHRUGGER";
    ac_string   [PLATFORMER]        = "PLATFORMER";
    ac_string   [SHRUGGER2]         = "SHRUGGER2";
    ac_string   [BASHER]            = "BASHER";
    ac_string   [MINER]             = "MINER";
    ac_string   [DIGGER]            = "DIGGER";

    ac_string   [JUMPER]            = "JUMPER";
    ac_string   [ROPER]             = "ROPER";
    ac_string   [AC_MAX]            = "AC_MAX";

    style_string[GARDEN]            = "Garden";
    style_string[RED]               = "Red";
    style_string[YELLOW]            = "Yellow";
    style_string[GREEN]             = "Green";
    style_string[BLUE]              = "Blue";
    style_string[CYAN]              = "Cyan";
    style_string[PURPLE]            = "Purple";
    style_string[GREY]              = "Grey";
    style_string[BLACK]             = "Black";
}



Ac string_to_ac(const std::string& s) {
    for (std::map <Ac, std::string> ::iterator i = ac_string.begin();
         i != ac_string.end(); ++i) {
        if (i->second == s) return i->first;
    } return AC_MAX;
}



const std::string& ac_to_string(const Ac a) {
    for (std::map <Ac, std::string> ::iterator i = ac_string.begin();
         i != ac_string.end(); ++i) {
        if (i->first == a) return i->second;
    } return (--ac_string.end())->second;
}



Style string_to_style(const std::string& s) {
    for (std::map <Style, std::string> ::iterator i = style_string.begin();
         i != style_string.end(); ++i) {
        if (i->second == s) return i->first;
    } return GARDEN;
}



const std::string& style_to_string(const Style a) {
    for (std::map <Style, std::string> ::iterator i = style_string.begin();
         i != style_string.end(); ++i) {
        if (i->first == a) return i->second;
    } return style_string.begin()->second;
}

}
// End of namespace Lem
