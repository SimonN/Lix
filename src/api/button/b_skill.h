/*
 * ./api/button/b_skill.h
 *
 * A button of size 40 x 60 with a skill and a number.
 *
 *  int number
 *
 *      Amount of usages left of the skill. Since replay_id is also an int,
 *      the name number is bad.
 *
 *  int replay_id;
 *
 *      We sort skills before displaying them in the panel.
 *      To not screw up the replay format yet, we don't sort them in the
 *      replay file. During game, both single- and multiplayer, each skill
 *      button therefore has a different replay_id, to. When the first skill
 *      button has replay_id 6, then whenever it's selected, it generates
 *      SKILL 6. Conversely, such actions are mapped to the first button.
 *
 *      In the editor, we don't care about replay_id.
 *
 */

#pragma once

#include "button.h"

#include "../../graphic/graphic.h"
#include "../../lix/lix_enum.h"

namespace Api {
class SkillButton : public Button {

private:

    int         number;
    LixEn::Ac   skill;

    Graphic     icon;
    std::string hotkey_label;

public:

    //                   x,             y,             xl
             SkillButton(const int = 0, const int = 0, const int = 36);
    virtual ~SkillButton();

    inline LixEn::Ac get_skill()     const { return skill;     }
    inline int       get_number()    const { return number;    }

           void set_skill       (const LixEn::Ac a);
           void set_number      (const int);
           void set_style       (const LixEn::Style);
           void set_hotkey_label(const std::string& s);

protected:

    virtual void draw_self();

}; // Ende Klasse
}  // Ende Namensraum
