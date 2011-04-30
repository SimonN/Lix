#include <sstream>

#include "labeltwo.h"
#include "../other/language.h"

namespace Api {

LabelTwo::LabelTwo(
    int x,
    int y,
    int xl
) :
    Element(x, y, xl, Label::element_yl),
    desc   (0,  0),
    value  (xl, 0)
{
    add_child(desc);
    add_child(value);
    value.set_align(Label::BLOCKY);
    value.set_color(color[COL_WHITE]);
}



void LabelTwo::set_desc (const std::string& s) { desc.set_text(s);    }
void LabelTwo::set_value(const std::string& s) { value.set_text(s);   }
void LabelTwo::set_value(const int i         ) { value.set_number(i); }

void LabelTwo::set_value_seconds_as_time(const int i)
{
    std::ostringstream cl;
    if (i > 0) cl << i / 60 << ':' << i % 60 / 10 << i % 10;
    else       cl << Language::browser_info_clock_none;
    value.set_text(cl.str());
}

} // end of namespace
