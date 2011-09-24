/*
 * GameplayStats::PanelTribe
 *
 */

#include <sstream>

#include "pan_info.h"

#include "../../graphic/gra_lib.h"
#include "../../other/language.h"
#include "../../other/help.h"

const int GameplayStats::PanelTribe::frame_outopp = 2;
const int GameplayStats::PanelTribe::frame_out    = 3;
const int GameplayStats::PanelTribe::frame_hatch  = 4;
const int GameplayStats::PanelTribe::frame_in     = 5;
const int GameplayStats::PanelTribe::frame_clock  = 6;
const int GameplayStats::PanelTribe::frame_cup    = 7;
const int GameplayStats::PanelTribe::frame_cupall = 8;
const int GameplayStats::PanelTribe::frame_target = 9;

const int GameplayStats::PanelTribe::frame_color  = 0;
const int GameplayStats::PanelTribe::frame_gray   = 1;
const int GameplayStats::PanelTribe::frame_green  = 2;

const int GameplayStats::PanelTribe::name_short_width  = 25;
const int GameplayStats::PanelTribe::name_medium_width = 38;



GameplayStats::PanelTribe::PanelTribe(Torbit& b)
:
    ground  (&b),
    tr      (0),
    white   (false)
{
}



GameplayStats::PanelTribe::~PanelTribe()
{
}



// Breite: 80 Pixel
void GameplayStats::PanelTribe::draw_local(
    const int  x,
    const int  y,
    const bool multi,
    const int*    clock,
    const Lixxie* tarinf,
    const int     tarcnt,
    const LixEn::Style cup_style,
    const bool         cup_colored,
    const int          oppo_saved,
    const LixEn::Style cupall_style,
    const bool         cupall_colored,
    const int          oppo_expected
) const
{
    if (!tr) return;

    int x_out_icon   = x +   0;
    int x_out        = x +   0 + 25;
    int x_hatch_icon = x +  70; // not used in singleplayer
    int x_hatch      = x +  70 + 25;
    int x_in_icon    = x +  70;
    int x_in         = x +  70 + 25;
    int x_cup_icon   = x + 140; // is the target save count
    int x_cup        = x + 140 + 25;
    int x_cupall_icon= x + 140; // doesn't appear in singleplayer
    int x_cupall     = x + 140 + 25;
    int x_clock_icon = x + 240;
    int x_clock      = x + 240 + 25;
    int x_tarinf     = x + 320;
    int xl_tarinf    = 12 * 40 - x_tarinf;
    if (multi) {
        x_hatch_icon = x +  70;
        x_hatch      = x +  70 + 25;
        x_in_icon    = x +  70;
        x_in         = x +  70 + 25;
        x_cup_icon   = x + 140;
        x_cup        = x + 140 + 25;
        x_cupall_icon= x + 210;
        x_cupall     = x + 210 + 25;
        x_clock_icon = x + 280;
        x_clock      = x + 280 + 25;
        x_tarinf     = x + 350;
        xl_tarinf    = 12 * 40 - x_tarinf;
    }

    const int  in    = tr->lix_saved;
    const int  out   = tr->get_lix_out();
    const bool green = multi ? tr->lix_saved >= oppo_saved
                             : tr->required && in >= tr->required;

    // draw out count
    GraLib::get_icon(tr->style).draw(*ground, x_out_icon, y, frame_out,
     out == 0 ? frame_gray :                       frame_color);
    draw_nr_sml(out, x_out, y);

    if (multi && tr->lix_hatch > 0) {
        // draw content of hatch
        GraLib::get_icon(tr->style).draw(*ground, x_hatch_icon, y, frame_hatch,
         frame_gray);
        // i.e. always draw a grey frame, don't do something like this:
        // tr->lix_hatch == 0 ? frame_gray : frame_color
        draw_nr_sml(tr->lix_hatch, x_hatch, y);
    }
    else {
        // out count
        GraLib::get_icon(tr->style).draw(*ground, x_in_icon,  y, frame_in,
         in  == 0 ? frame_gray : green ? frame_green : frame_color);
        draw_nr_sml(in, x_in, y, green);
    }

    if (clock) {
        GraLib::get_icon(tr->style).draw(*ground, x_clock_icon, y, frame_clock,
         frame_gray);
        std::ostringstream str;
        const int  c = *clock + gloB->updates_per_second - 1;
        const int& u = gloB->updates_per_second;
        str << c / (u * 60);
        str << ':';
        str << c % (u * 60) / (u * 10);
        str << c % (u * 10) / (u     );
        Help::draw_shadow_text(*ground, font_med, str.str().c_str(),
         x_clock, y, color[COL_TEXT]);
    }

    // draw the cup
    std::ostringstream str;
    bool               green_cup = false;
    if (multi) {
        GraLib::get_icon(cup_style).draw(*ground, x_cup_icon, y, frame_cup,
         cup_colored ? frame_color : frame_gray);
        if (in > oppo_saved) {
            str << '+';
            green_cup = true;
        }
        str << in - oppo_saved;
    }
    else {
        // draw the required count
        GraLib::get_icon(cup_style).draw(*ground, x_cup_icon, y, frame_target,
         frame_gray);
        str << tr->required << "/" << tr->initial;
    }
    Help::draw_shadow_text(*ground, font_med, str.str().c_str(),
     x_cup, y, green_cup ? color[COL_TEXT_GREEN] : color[COL_TEXT]);

    // cupall
    if (multi) {
        GraLib::get_icon(cupall_style).draw(*ground, x_cupall_icon, y,
         frame_cupall, cupall_colored ? frame_color : frame_gray);
        str.str("");
        str.clear();
        if (tr->get_score_expected() > oppo_expected) str << '+';
        str << tr->get_score_expected() - oppo_expected;
        Help::draw_shadow_text(*ground, font_med, str.str().c_str(),
         x_cupall, y, color[COL_TEXT]);
    }

    // Tarinf
    if (tarinf) {
        std::ostringstream cnt;
        std::string str;
        std::string ab;
        cnt << tarcnt;
        cnt << ' ';
        str += LixEn::ac_to_string(tarinf->get_ac());
        Help::string_to_nice_case(str);
        if (tarcnt > 1 && Language::get() != Language::GERMAN) str += 's';
        str = cnt.str() + str;
        if (tarinf->get_runner()
         || tarinf->get_floater()
         || tarinf->get_climber()) {
            ab += " (";
            if (tarinf->get_runner() ) ab += 'R';
            if (tarinf->get_climber()) ab += 'C';
            if (tarinf->get_floater()) ab += 'F';
            ab += ')';
        }
        while (text_length(font_med, (str + ab).c_str()) > xl_tarinf
         && str.size() > 1) {
            str.resize(str.size() - 2);
            str += '.';
        }
        Help::draw_shadow_text(*ground, font_med,
         (str + ab).c_str(), x_tarinf, y);
    }

}



// Breite: 40 Pixel
void GameplayStats::PanelTribe::draw_med
 (const int x, const int y, const int required) const
{
    if (!tr) return;
    const int  in    = tr->lix_saved;
    const int  out   = tr->get_lix_out() + tr->lix_hatch;
    const bool green = required && in >= required;
    if (tr->initial >= 100) {
        GraLib::get_icon(tr->style).draw(*ground, x, y, frame_outopp,
         out == 0 ? frame_gray : frame_color);
        Help::draw_shadow_text(*ground, font_med,
         name_short.c_str(), x + 15, y);
        draw_nr_ctr(out, x + 20, y + 20);
        draw_nr_ctr(in,  x + 20, y + 40, green);
    }
    else {
        Help::draw_shadow_centered_text(*ground, font_med,
         name_medium.c_str(), x + 20, y);
        GraLib::get_icon(tr->style).draw(*ground, x, y + 20, frame_outopp,
         out == 0 ? frame_gray : frame_color);
        // the following +16 looks nice above the centered ``in'' count below.
        draw_nr_sml(out, x + 16, y + 20);
        draw_nr_ctr(in,  x + 20, y + 40, green);
    }
}



void GameplayStats::PanelTribe::draw_nr_sml(const int first,
 const int x, const int y, const bool green) const
{
    std::ostringstream s;
    const int col_zero =         color[COL_TEXT];
    const int col_full = green ? color[COL_TEXT_GREEN]
                       : white ? color[COL_TEXT] : col_zero;
    s << first;
    Help::draw_shadow_text(*ground, font_med,
     s.str().c_str(), x, y, first > 0 ? col_full : col_zero);
}



void GameplayStats::PanelTribe::draw_nr_ctr(const int first,
 const int x, const int y, const bool green) const
{
    std::ostringstream s;
    const int col_zero =         color[COL_TEXT];
    const int col_full = green ? color[COL_TEXT_GREEN]
                       : white ? color[COL_TEXT] : col_zero;
    s << first;
    Help::draw_shadow_centered_text(*ground, font_med,
     s.str().c_str(), x, y, first > 0 ? col_full : col_zero);
}



bool GameplayStats::PanelTribe::operator > (const PanelTribe& rhs) const
{
    // It's fine if in the beginning, all tribes compare as equal.
    // Because they will be inserted into GameplayStats's vector according
    // to permutation, the first opponents shown will be random in every game.
    int out, rhs_out;
    if    (tr->lix_saved != rhs.tr->lix_saved)
    return tr->lix_saved > rhs.tr->lix_saved;
    out     =     tr->lix_hatch +     tr->get_lix_out();
    rhs_out = rhs.tr->lix_hatch + rhs.tr->get_lix_out();
    if (out != rhs_out) return out > rhs_out;
    return false;
}

