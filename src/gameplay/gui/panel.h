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

    SkBIt button_by_skill(const LixEn::Ac);

    void set_like_tribe(const Tribe*);
    void set_skill_on  (const LixEn::Ac);

           void set_hint_cur (const int);
    inline int  get_hint_cur () const          { return hint_cur;           }
    inline bool get_nuke_doubleclicked() const { return nuke_doubleclicked; }

    inline void suggest_tooltip_scrolling()   { tooltip_scrolling   = true; }
    inline void suggest_tooltip_force_dir()   { tooltip_force_dir   = true; }
    inline void suggest_tooltip_priority()    { tooltip_priority    = true; }
    inline void suggest_tooltip_builders()    { tooltip_builders    = true; }
    inline void suggest_tooltip_platformers() { tooltip_platformers = true; }

protected:

    virtual void calc_self();
    virtual void draw_self();

private:

    GapaMode gapamode;

    bool nuke_doubleclicked;
    int  timer_tick_nuke_single;

    bool tooltip_scrolling;
    bool tooltip_force_dir;
    bool tooltip_priority;
    bool tooltip_builders;
    bool tooltip_platformers;

    int hint_size; // including the tutorial hint
    int hint_cur;

    BiB hint_big,
        hint_plus,
        hint_minus;

};
