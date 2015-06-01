#include <sstream>

#include "b_skill.h"

#include "../../graphic/gra_lib.h"
#include "../../other/help.h"
#include "../../other/language.h" // Lix-Bildname

namespace Api {

SkillButton::SkillButton(const int nx, const int ny, const int nxl)
:
    Button (nx, ny, nxl, 60),
    replay_id(-1),
    icon(GraLib::get_lix(LixEn::GARDEN), get_ground(),
     get_x_here() + get_xl()/2 - 17, get_y_here() + 23)
{
    set_skill(LixEn::NOTHING);
}

SkillButton::~SkillButton()
{
}



void SkillButton::set_skill(const LixEn::Ac ac)
{
    set_draw_required();
    skill = ac;
    icon.set_y_frame(skill - 1);
    if (skill == LixEn::NOTHING) set_number(0);
}



void SkillButton::set_number(const int nr)
{
    set_draw_required();
    number = (skill != LixEn::NOTHING ? nr : 0);
    if (number != 0 ) {
        icon.set_x_frame(0);
    }
    else {
        set_off();
        icon.set_x_frame(1);
    }
}



void SkillButton::set_style(const LixEn::Style st)
{
    set_draw_required();
    icon.set_cutbit(GraLib::get_lix(st));
}



void SkillButton::set_hotkey_label(const std::string& s)
{
    hotkey_label = s;
    if (ustrlen(hotkey_label.c_str()) > 3) {
        hotkey_label.resize(uoffset(hotkey_label.c_str(), 3));
    }

    // note: if the hotkey label text has an initial non-ASCII
    // character we won't be able to capitalize it. Oh well.
    // Not sure that ever happens for real?
    // Anyway, this allows us to work below as if the first
    // character only takes one byte.
    if (hotkey_label.size() > 0) {
        int c = hotkey_label[0];
        if (c >= 'a' && c <= 'z') hotkey_label[0] = c + 'A' - 'a';
    }
    set_draw_required();
}



void SkillButton::draw_self()
{
    Button::draw_self();
    if (skill != LixEn::NOTHING) {
        icon.set_x(get_x_here() + get_xl()/2 - 17);
        icon.set_y(get_y_here() + 23);
        icon.draw();

        // draw the number
        // note: some calculations below assume all chars in s
        // are ASCII which is currently the case.
        std::ostringstream s;
        if      (number == LixEn::infinity) s << "*";
        else if (number == 0); // write nothing
        else if (number >= 0 && number <= LixEn::skill_nr_max) s << number;
        else     s << "?";

        // for the horizontal position, we subtract 1 in case of 3 digits
        // because they weren't properly centered otherwise.
        Help::draw_shadow_centered_text(get_ground(),
         (s.str().size() > 2 ? font_nar : font_big),
         s.str().c_str(), get_x_here() + get_xl()/2 + (s.str().size()>2?-1:0),
         get_y_here() + 4,
         color[COL_TEXT_ON], color[COL_API_SHADOW]);

        // draw the hotkey
        if (! hotkey_label.empty() && ! s.str().empty()) {
            Help::draw_shadow_text(get_ground(), font_sml,
                hotkey_label.c_str(), get_x_here() + get_xl() - 3
                    - ::text_length(font_sml, hotkey_label.c_str()),
                get_y_here() + get_yl() - 15,
                color[COL_TEXT], color[COL_API_SHADOW]
            );
        }
        // end drawing hotkey
    }
}

}  // Ende Namensraum

