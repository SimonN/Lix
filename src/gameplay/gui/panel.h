/*
 * gameplay/panel.h
 *
 * All the control panels and the stats area.
 *
 */

#pragma once

#include "pan_info.h"

#include "../tribe.h"

#include "../../api/element.h"
#include "../../api/label.h"
#include "../../api/button/b_bitmap.h"
#include "../../api/button/b_text.h"
#include "../../api/button/b_skill.h"

struct GameplayPanel : public Api::Element {

public:

    typedef Api::BitmapButton              BiB;
    typedef Api::SkillButton               SkB;
    typedef std::vector <Api::SkillButton> SkBVec;
    typedef SkBVec::iterator               SkBIt;

    SkBVec skill;

    BiB rate_minus,
        rate_plus,
        rate_fixed,
        pause,
        zoom,
        speed_slow,
        speed_fast,
        speed_turbo,
        state_save,
        state_load,
        restart,
        nuke_single,
        nuke_multi;

    Api::TextButton spec_tribe;

    GameplayStats stats;

    Api::Label rate_slow;
    Api::Label rate_cur;
    Api::Label rate_fast;

    void (*on_hint_change)(void*, const int); // int = new hint number
    void* on_hint_change_where; // first arg to give to above function

    ///////////////////////////////////////////////////////////////////////////

    GameplayPanel();
    virtual ~GameplayPanel() {}

    void set_gapamode_and_hints(GapaMode, const int); // hint size

    inline GapaMode get_gapamode() { return gapamode;}

    void set_like_tribe   (const Tribe*, const Tribe::Master*);
    void set_skill_numbers(const Tribe&);
    void set_skill_on     (const int);

           void set_hint_cur (const int);
    inline int  get_hint_cur () { return hint_cur; }



protected:

    virtual void calc_self();
    virtual void draw_self();

private:

    GapaMode gapamode;

    int hint_size; // including the tutorial hint
    int hint_cur;

    BiB hint_big,
        hint_plus,
        hint_minus;

};
