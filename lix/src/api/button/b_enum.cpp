#include "b_enum.h"

namespace Api {

EnumButton::EnumButton(int nx, int ny, int nxl, int nyl)
:
    TextButton(nx, ny, nxl, nyl),
    number    (0)
{
}


EnumButton::~EnumButton()
{
}



void EnumButton::set_number(const int i)
{
    if (!text_vector.empty()) {
        number = i % text_vector.size();
        set_text(text_vector[number]);
    }
    else number = 0;
}



void EnumButton::add_text(const std::string& s)
{
    // Ersten Eintrag sofort anzeigen, falls Zahl nicht geregelt wird
    if (text_vector.empty()) set_text(s);
    text_vector.push_back(s);
}



void EnumButton::calc_self()
{
    TextButton::calc_self();
    if (get_clicked()) set_number(number + 1);
}

}
// Ende Namensraum
