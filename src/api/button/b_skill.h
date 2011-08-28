/*
 * ./api/button/b_skill.h
 *
 * A button of size 40 x 60 with a skill and a number.
 *
 */

#pragma once

#include "button.h"

#include "../../graphic/graphic.h"
#include "../../lix/lix_enum.h"

namespace Api {
class SkillButton : public Button {

private:

    int       number;
    LixEn::Ac skill;

    Graphic   icon;

public:

    //                   Grund,   x,                  y
             SkillButton(const unsigned = 0, const unsigned = 0,
                         const LixEn::Ac = LixEn::NOTHING,
                         const int = 0, const LixEn::Style = LixEn::GARDEN);
    virtual ~SkillButton();

    inline LixEn::Ac  get_skill()  const { return skill;  }
    inline int        get_number() const { return number; }

           void       set_skill (const LixEn::Ac a);
           void       set_number(const int);
           void       set_style (const LixEn::Style);

protected:

    virtual void draw_self();

}; // Ende Klasse
}  // Ende Namensraum
