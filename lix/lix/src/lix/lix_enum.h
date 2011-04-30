/*
 * lemming/lix_enum.h
 *
 * This outsources all Lemming enums that were previously in lemming/lix.h.
 * The idea is to not have almost everything depend on that file.
 *
 */

#pragma once

#include <string>

namespace LixEn {

    const int ex_offset = 16;
    const int ey_offset = 26;

    const int infinity     = -1;  // Fuer SkillButtons
    const int skill_nr_max = 999; // Fuer SkillButtons

    enum Ac {
        NOTHING,
        FALLER,
        TUMBLER,
        STUNNER,
        LANDER,
        SPLATTER,
        BURNER,
        DROWNER,
        EXITER,
        WALKER,

        CLIMBER,
        ASCENDER,
        FLOATER,
        EXPLODER,
        EXPLODER2,
        BLOCKER,
        BUILDER,
        SHRUGGER,
        PLATFORMER,
        SHRUGGER2,
        BASHER,
        MINER,
        DIGGER,

        JUMPER,
        ROPER,

        AC_MAX
    };

    enum Style {
        GARDEN,
        RED,
        YELLOW,
        GREEN,
        BLUE,
        CYAN,
        PURPLE,
        GREY,
        BLACK,
        STYLE_MAX
    };

    void initialize();

          Ac           string_to_ac   (const std::string&);
          Style        string_to_style(const std::string&);
    const std::string& ac_to_string   (const Ac);
    const std::string& style_to_string(const Style);

}
// end of namespace
