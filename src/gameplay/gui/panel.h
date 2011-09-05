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

    Api::Label rate_min;
    Api::Label rate;

    ///////////////////////////////////////////////////////////////////////////

    GameplayPanel();
    virtual ~GameplayPanel() {}

    void            set_gapamode(GapaMode);
    inline GapaMode get_gapamode() { return gapamode;}

    void set_like_tribe   (const Tribe*, const Tribe::Master*);
    void set_skill_numbers(const Tribe&);
    void set_skill_on     (const int);

protected:

    virtual void calc_self();
    virtual void draw_self();

private:

    GapaMode gapamode;

};
