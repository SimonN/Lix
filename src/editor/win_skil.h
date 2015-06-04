/*
 * ./editor/win_skill.h
 *
 * Fenster, in dem die Belegung der acht Faehigkeits-Tasten belegt weden.
 * Speziell fuer diese Klasse wird auch die Klasse SkillButton erweitert zu
 * SkillButtonWithButtons, diese Definition steht weiter unten in dieser Datei.
 *
 */

#pragma once

#include <vector>

#include "../api/button/b_bitmap.h"
#include "../api/button/b_skill.h"
#include "../api/button/b_text.h"
#include "../api/number.h"
#include "../api/window.h"
#include "../other/globals.h"
#include "../level/level.h"

namespace Api {

struct PossibleSkill;
struct SkillButtonWithButtons;

class WindowSkill : public Window {

private:

    static const int y_sbwb;
    static const int this_xl;
    static const int this_yl;

    Level* level;

    std::vector <SkillButtonWithButtons*> sbwb;

    // I have absolutely no clue why PossibleSkill works as members
    // and Sbwbs must be dynamically allocated. The game crashed whenever I
    // tried it with members for Sbwbs after the GUI rewrite in July 2010.
    typedef std::vector <SkillButtonWithButtons*> ::iterator SbwbIt;

    TextButton clear;
    TextButton classic8_to;
    TextButton all_to;
    Number     all_to_n;

    Checkbox   use_fling;
    Label      desc_use_fling;

    TextButton ok;
    TextButton cancel;

    Label debugging_1; // debugging: only set 12 skills. this will be removed
    Label debugging_2; // once the level format allows 14 skills.

    // Kopierverbot
    WindowSkill(const WindowSkill&);
    void operator = (const WindowSkill&);

public:

             WindowSkill(Level&);
    virtual ~WindowSkill();

protected:

    virtual void calc_self();

};



// ############################################################################
// ############################################################################
// ############################################################################



struct SkillButtonWithButtons : public Element {

    SkillButton  skill;

    BitmapButton   up_big;
    BitmapButton   up_med;
    BitmapButton   up_sml;
    BitmapButton down_big;
    BitmapButton down_med;
    BitmapButton down_sml;

             SkillButtonWithButtons(const unsigned = 0, const unsigned = 0);
    virtual ~SkillButtonWithButtons();

protected:

    virtual void calc_self();

}; // Ende Klasse
}  // Ende Namensraum
