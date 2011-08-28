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

    std::vector <PossibleSkill>           possible;
    std::vector <SkillButtonWithButtons*> sbwb;

    // I have absolutely no clue why PossibleSkill works as members
    // and Sbwbs must be dynamically allocated. The game crashed whenever I
    // tried it with members for Sbwbs after the GUI rewrite in July 2010.
    typedef std::vector <PossibleSkill>           ::iterator PosIt;
    typedef std::vector <SkillButtonWithButtons*> ::iterator SbwbIt;

    PossibleSkill*          cur_pos;
    SkillButtonWithButtons* cur_sbwb;

    TextButton clear;
    TextButton classic8;
    TextButton classic12;
    TextButton all_to;
    Number     all_to_n;
    TextButton ok;
    TextButton cancel;

    // Kopierverbot
    WindowSkill(const WindowSkill&);
    void operator = (const WindowSkill&);

    // Helfer fuer die schnelle Positionierung der Possible-Buttons
    inline unsigned int px(unsigned int i) { return 20 + i%8 * 60; }
    inline unsigned int py(unsigned int i) { return 40 + i/8 * 50; }

    void swap_sbwbs(SkillButtonWithButtons*, SkillButtonWithButtons*);
    void assign_sbwb_pos(SkillButtonWithButtons*, PossibleSkill*);

public:

             WindowSkill(Level&);
    virtual ~WindowSkill();

protected:

    virtual void calc_self();

};



// ############################################################################
// ############################################################################
// ############################################################################



struct PossibleSkill : public Button {

    static const unsigned this_length;

    Graphic icon;
    LixEn::Ac ac;

    PossibleSkill (const int = 0, const int = 0,
                   const LixEn::Ac    = LixEn::NOTHING,
                   const LixEn::Style = LixEn::GARDEN);
    virtual ~PossibleSkill();

    bool get_color();
    void set_color(const bool b);

protected:

    virtual void draw_self();

};



struct SkillButtonWithButtons : public Element {

    SkillButton  skill;

    BitmapButton   up_big;
    BitmapButton   up_med;
    BitmapButton   up_sml;
    BitmapButton down_big;
    BitmapButton down_med;
    BitmapButton down_sml;

             SkillButtonWithButtons(const unsigned = 0, const unsigned = 0,
                                    const LixEn::Ac = LixEn::NOTHING,
                                    const int = 0);
    virtual ~SkillButtonWithButtons();

protected:

    virtual void calc_self();

}; // Ende Klasse
}  // Ende Namensraum
