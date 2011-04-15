#include <sstream>

#include "number.h"

#include "../graphic/gra_lib.h" // Buttonbilder
#include "../other/help.h"
#include "../other/globals.h"

namespace Api {

Number::Number(int nx, int ny, const unsigned xl,
 const unsigned new_digits, const int nmin, const int nmax, const int nval,
 const bool new_six_buttons)
:
    Element(nx, ny, xl, 20),

    digits     (new_digits),
    six_buttons(new_six_buttons),

    number    (nval),
    step_sml  (  1),
    step_med  ( 10),
    step_big  (100),

    down_big(GraLib::get(gloB->file_bitmap_api_number), 0),
    down_med(GraLib::get(gloB->file_bitmap_api_number), 20),
    down_sml(GraLib::get(gloB->file_bitmap_api_number), 40),
    up_sml  (GraLib::get(gloB->file_bitmap_api_number), xl - 60),
    up_med  (GraLib::get(gloB->file_bitmap_api_number), xl - 40),
    up_big  (GraLib::get(gloB->file_bitmap_api_number), xl - 20)
{
    minimum        = nmin;
    maximum        = nmax;
    white_zero     = false;
    format_time    = false;
    show_sign      = false;
    minus_one_char = 0;

    down_med.set_x_frame(1);
    down_sml.set_x_frame(2);
    up_sml  .set_x_frame(3);
    up_med  .set_x_frame(4);
    up_big  .set_x_frame(5);

    add_child(down_big);
    add_child(up_big);
    add_child(down_med);
    add_child(down_sml);
    add_child(up_med);
    add_child(up_sml);

    // Aeussere Buttons nicht anzeigen?
    if (!six_buttons) {
        down_sml.set_x(down_sml.get_x() - 20);
        down_med.set_x(down_med.get_x() - 20);
        up_med  .set_x(up_med  .get_x() + 20);
        up_sml  .set_x(up_sml  .get_x() + 20);
        remove_child(down_big);
        remove_child(up_big);
    }
}



Number::~Number()
{
}



void Number::set_minimum(const int i)
{
    minimum = i;
    set_number(number);
}

void Number::set_maximum(const int i)
{
    maximum = i;
    set_number(number);
}

void Number::set_number(const int nval)
{
    number = nval;
    if      (number > maximum) number = maximum;
    else if (number < minimum) number = minimum;
    set_draw_required();
}



void Number::calc_self() {
    if (down_big.get_clicked()) {
        if (number == minimum) number = maximum;
        else {
            number -= step_big;
            if (number < minimum || number > maximum) number = minimum;
        }
    }
    if (down_med.get_clicked()) {
        if (number == minimum) number = maximum;
        else {
            number -= step_med;
            if (number < minimum || number > maximum) number = minimum;
        }
    }
    if (down_sml.get_clicked()) {
        if (number == minimum) number = maximum;
        else {
            number -= step_sml;
            if (number < minimum || number > maximum) number = minimum;
        }
    }
    if (up_sml.get_clicked()) {
        if (number == maximum) number = minimum;
        else {
            number += step_sml;
            if (number > maximum) number = maximum;
        }
    }
    if (up_med.get_clicked()) {
        if (number == maximum) number = minimum;
        else {
            number += step_med;
            if (number > maximum) number = maximum;
        }
    }
    if (up_big.get_clicked()) {
        if (number == maximum) number = minimum;
        else {
            number += step_big;
            if (number > maximum) number = maximum;
        }
    }

    if (down_big.get_draw_required() && six_buttons
     || down_med.get_draw_required()
     || down_sml.get_draw_required()
     || up_sml  .get_draw_required()
     || up_med  .get_draw_required()
     || up_big  .get_draw_required() && six_buttons) set_draw_required();
}



void Number::draw_self()
{
    // Wenn wir sowieso hier sind, dann koennen wir auch alles sofort
    // zeichnen. Ein bisschen erhoehter Aufwand, der gerechtfertigtigt wird
    // von einer kuerzeren Formulierung im Optionsmenue, in dem man das
    // Number-Element nicht immer sieht.
    down_med.set_draw_required();
    down_sml.set_draw_required();
    up_sml  .set_draw_required();
    up_med  .set_draw_required();
    down_med.draw();
    down_sml.draw();
    up_sml  .draw();
    up_med  .draw();

    if (six_buttons) {
        down_big.set_draw_required();
        up_big  .set_draw_required();
        down_big.draw();
        up_big  .draw();
    }

    // Alte Zahl wegputzen
    rectfill(get_ground().get_al_bitmap(),
     get_x_here()            + 40 + 20 * six_buttons,   get_y_here(),
     get_x_here() + get_xl() - 40 - 20 * six_buttons-1, get_y_here() + 20-1,
     color[COL_API_M]);

    // Negativ?
    bool negative = (number < 0);
    int  posval   = number;
    if (negative) posval *= -1;

    // String mit (digits)-vielen Ziffern befuellen
    // und fuer jede Ziffer eine Farbe festlegen
    std::string       digit(digits, '0');
    std::vector <int> digit_color(digits, color[COL_TEXT]);

    if (white_zero) digit_color[digits - 1] = color[COL_TEXT_ON];

    // Im seltenen Fall, dass -1 und minus_one_char zutreffen
    if (number == -1 && minus_one_char != 0) {
        for (int i = digits - 1; i >= 0; --i) {
            digit[i]       = minus_one_char;
            digit_color[i] = color[COL_TEXT_ON];
        }
    }
    // Normale Zahlen, also keine Zeiten
    else if (!format_time) {
        for (int i = digits - 1; i >= 0; --i) {
            digit[i] = '0' + posval%10;
            if (posval > 0) digit_color[i] = color[COL_TEXT_ON];
            posval /= 10;
            if (show_sign && posval == 0 && i > 0) {
                digit[0] = negative ? '-' : '+';
                if (number != 0) digit_color[0] = color[COL_TEXT_ON];
                break;
            }
        }
    }
    // Nun format_time
    else {
        std::ostringstream digitstream;
        digitstream << posval / 60 << ':' << (posval % 60) / 10 << posval % 10;
        digit = digitstream.str();
        while (digit.length() > digits - negative) digit.erase(digit.begin());
        while (digit.length() < digits - negative) digit = '0' + digit;
        if (negative) {
            digit = '-' + digit;
            digit_color[0] = color[COL_TEXT_ON];
        }
        // Ab der ersten interessanten Stelle alles weiss zeichnen
        for (unsigned i = 0; i < digit.length(); ++i)
         if (digit[i] != '0' && digit[i] != ':' && digit[i] != '-') {
            for (unsigned j = i; j < digit.length(); ++j)
             digit_color[j] = color[COL_TEXT_ON];
            break;
        }
    }

    // Ziffern malen
    for (unsigned i = 0; i < digits; ++i) {
        std::string substring(digit, i, 1); // 1 Buchstaben lang
        // Zeichenmethode fuer - anders waehlen
//        if (substring[0] == '-' || substring[0] == '+') {
//            Help::draw_shadow_centered_text(get_ground(), font_med,
//             substring.c_str(), get_x() + get_xl()/2 - digits*5 +5+i*10,
//             get_y(), digit_color[i], color[COL_API_SHADOW]);
//        }
//        else {
//            Help::draw_shadow_text(get_ground(), font_med,
//             substring.c_str(), get_x() + get_xl()/2 - digits*5 + i*10,
//             get_y(), digit_color[i], color[COL_API_SHADOW]);
//        }
        Help::draw_shadow_fixed_text(get_ground(), font_med,
         substring.c_str(), get_x_here() + get_xl()/2 - digits*5 + i*10,
         get_y_here(), digit_color[i]);
    }
}

} // Ende Namensraum Api
