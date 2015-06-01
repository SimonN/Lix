/*
 * GameplayStats::PanelTribe
 *
 */

#include <sstream>

#include "pan_info.h"

#include "../../graphic/gra_lib.h"
#include "../../other/language.h"
#include "../../other/help.h"

const int GameplayStats::PanelTribe::frame_outopp =  2;
const int GameplayStats::PanelTribe::frame_out    =  3;
const int GameplayStats::PanelTribe::frame_hatch  =  4;
const int GameplayStats::PanelTribe::frame_in     =  5;
const int GameplayStats::PanelTribe::frame_countd =  6;
const int GameplayStats::PanelTribe::frame_stopw  =  7;
const int GameplayStats::PanelTribe::frame_cup    =  8;
const int GameplayStats::PanelTribe::frame_cupall =  9;
const int GameplayStats::PanelTribe::frame_target = 10;

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
    const int*    countd,
    const int*    stopw,
    const Lixxie* tarinf,
    const int     tarcnt,
    const int     oppo_saved
) const
{
    if (!tr) return;

    const int x_p = 36; // width of a skill panel, should query the class :/

    int x_out_icon   = x +   0;
    int x_out        = x +   0 + 25;
    int x_hatch_icon = x + 2*x_p;
    int x_hatch      = x + 2*x_p + 25;
    int x_in_icon    = x + 4*x_p;
    int x_in         = x + 4*x_p + 25;
    int x_clock_icon = x + 7*x_p;
    int x_clock      = x + 7*x_p + 25;
    int x_tarinf     = x + 10*x_p;
    int xl_tarinf    =     14*x_p - x_tarinf;

    const int  in      = tr->lix_saved;
    const int  in_late = tr->lix_saved_late;
    const int  out     = tr->lix_out + tr->lix_hatch;
    const bool green   = multi ? tr->lix_saved >= oppo_saved
                               : tr->required && in >= tr->required;

    // draw out count
    GraLib::get_icon(tr->style).draw(*ground, x_out_icon, y, frame_out,
     out == 0 ? frame_gray :                       frame_color);
    draw_nr_sml(out, x_out, y);

    // draw hatch, and hatch count
    GraLib::get_icon(tr->style).draw(*ground, x_hatch_icon, y,
     frame_hatch, frame_gray);
    draw_nr_sml(tr->lix_hatch, x_hatch, y);

    // draw the house
    // possible = the highest number possible when all yet living are saved
    const int possible = out + in + in_late;
    const int required_or_opponent = (multi ? oppo_saved : tr->required);

    if (possible < required_or_opponent)
        GraLib::get_icon(tr->style).draw(*ground, x_in_icon, y,
         frame_target, frame_color);
    else if (in >= required_or_opponent)
        GraLib::get_icon(tr->style).draw(*ground, x_in_icon, y,
         frame_in, frame_green);
    else if (in > 0)
        GraLib::get_icon(tr->style).draw(*ground, x_in_icon, y,
         frame_in, frame_color);
    else
        GraLib::get_icon(tr->style).draw(*ground, x_in_icon, y,
         frame_in, frame_gray);

    // draw the saved count
    // in case of singleplayer time limit:
    //      those which have been saved too late are written on the clock,
    //      not on this counter here
    std::ostringstream str;
    str << in;
    if (required_or_opponent > 0)
        str << "/" << required_or_opponent;
    Help::draw_shadow_text(*ground, font_med, str.str().c_str(),
        x_in, y,
        green ? color[COL_TEXT_GREEN] : color[COL_TEXT]);

    // draw the clock
    // in_late should always be 0 in multiplayer
    if (countd || stopw) {
        // Showing a countdown has precedence over showing the stopwatch
        GraLib::get_icon(tr->style).draw(*ground, x_clock_icon, y,
         countd ? frame_countd : frame_stopw, frame_gray);
        std::ostringstream str;

        if (in_late == 0) {
            // display timer or stopwatch normally
            const int  c = countd ? *countd + gloB->updates_per_second - 1
                                  : *stopw;
            const int& u = gloB->updates_per_second;
            str << c / (u * 60);
            str << ':';
            str << c % (u * 60) / (u * 10);
            str << c % (u * 10) / (u     );
        }
        else {
            // display the number of lixes saved plus those saved too late
            // never draw this in green
            str << (in + in_late);
            // because space is limited, ignore tr->required with 3-digit-nrs
            if (in + in_late < 100 && tr->required < 100)
            str << "/" << tr->required;
        }
        Help::draw_shadow_text(*ground, font_med, str.str().c_str(),
         x_clock, y, color[COL_TEXT]);
    }

    // Tarinf
    if (tarinf && tarinf->get_ac() != LixEn::NOTHING) {
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

        int num_chars = ustrlen(str.c_str());
        while (text_length(font_med, (str + ab).c_str()) > xl_tarinf
         && num_chars > 1) {
            Help::remove_last_utf8_char(str);
            Help::remove_last_utf8_char(str);
            str += '.';
            num_chars--;
        }
        Help::draw_shadow_text(*ground, font_med,
         (str + ab).c_str(), x_tarinf, y);
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
    out     =     tr->lix_hatch +     tr->lix_out;
    rhs_out = rhs.tr->lix_hatch + rhs.tr->lix_out;
    if (out != rhs_out) return out > rhs_out;
    return false;
}

