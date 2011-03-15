#include <sstream>

#include "b_skill.h"

#include "../../graphic/gra_lib.h"
#include "../../other/help.h"
#include "../../other/language.h" // Lix-Bildname

namespace Api {

SkillButton::SkillButton(const unsigned nx, const unsigned ny,
 const LixEn::Ac ac, const int nr, const LixEn::Style style)
:
    Button (nx, ny, 40, 60),
    lem(GraLib::get_lix(style), get_ground(),
     get_x_here() + 3, get_y_here() + 23)
{
    set_skill(ac);
    set_number(nr);
}

SkillButton::~SkillButton()
{
}



void SkillButton::set_skill(const LixEn::Ac ac)
{
    set_draw_required();
    skill = ac;
    lem.set_y_frame(skill - 1);
    if (skill == LixEn::NOTHING) set_number(0);
}



void SkillButton::set_number(const int nr)
{
    set_draw_required();
    number = (skill != LixEn::NOTHING ? nr : 0);
    if (number != 0 ) {
        lem.set_x_frame(0);
    }
    else {
        set_off();
        lem.set_x_frame(1);
    }
}



void SkillButton::set_style(const LixEn::Style st)
{
    lem.set_cutbit(GraLib::get_lix(st));
}



void SkillButton::draw_self()
{
    Button::draw_self();
    if (skill != LixEn::NOTHING) {
        lem.set_x(get_x_here() +  3);
        lem.set_y(get_y_here() + 23);
        lem.draw();

        std::ostringstream s;
        if      (number == LixEn::infinity) s << "*";
        else if (number == 0); // write nothing
        else if (number >= 0 && number <= LixEn::skill_nr_max) s << number;
        else     s << "?";

        // for the horizontal position, we subtract 1 in case of 3 digits
        // because they weren't properly centered otherwise.
        Help::draw_shadow_centered_text(get_ground(),
         (s.str().size() > 2 ? font_nar : font_big),
         s.str().c_str(), get_x_here() + 20 + (s.str().size() > 2 ? -1 : 0),
         get_y_here() + 4,
         color[COL_TEXT_ON], color[COL_API_SHADOW]);
    }
}

}  // Ende Namensraum

