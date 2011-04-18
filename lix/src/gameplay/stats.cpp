/*
 * gameplay/panel.cpp
 *
 */

#include <sstream>

#include "stats.h"

#include "../graphic/gra_lib.h"
#include "../other/language.h"
#include "../other/help.h"

const int GameplayStats::PanelTribe::frame_outopp = 0;
const int GameplayStats::PanelTribe::frame_out    = 1;
const int GameplayStats::PanelTribe::frame_in     = 2;
const int GameplayStats::PanelTribe::frame_clock  = 3;
const int GameplayStats::PanelTribe::frame_cup    = 4;
const int GameplayStats::PanelTribe::frame_cupall = 5;
const int GameplayStats::PanelTribe::frame_target = 6;

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
    int x_out        = x +  25;
    int x_in_icon    = x +  80;
    int x_in         = x + 105;
    int x_cup_icon   = x + 160; // is the target save count
    int x_cup        = x + 185;
    int x_cupall_icon= x + 160; // doesn't appear in singleplayer
    int x_cupall     = x + 185;
    int x_clock_icon = x + 240;
    int x_clock      = x + 265;
    int x_tarinf     = x + 320;
    int xl_tarinf    = 12 * 40 - x_tarinf;
    if (multi) {
        x_in_icon    = x +  70;
        x_in         = x +  95;
        x_cup_icon   = x + 140;
        x_cup        = x + 165;
        x_cupall_icon= x + 210;
        x_cupall     = x + 235;
        x_clock_icon = x + 280;
        x_clock      = x + 305;
        x_tarinf     = x + 350;
        xl_tarinf    = 12 * 40 - x_tarinf;
    }

    const int  in    = tr->lix_saved;
    const int  out   = tr->get_lix_out() + tr->lix_hatch;
    const bool green = multi ? tr->lix_saved >= oppo_saved
                             : tr->required && in >= tr->required;

    GraLib::get_icon(tr->style).draw(*ground, x_out_icon, y, frame_out,
     out == 0 ? frame_gray :                       frame_color);
    GraLib::get_icon(tr->style).draw(*ground, x_in_icon,  y, frame_in,
     in  == 0 ? frame_gray : green ? frame_green : frame_color);
    draw_nr_sml(out, x_out, y);
    draw_nr_sml(in,  x_in, y, green);

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
        if (in >= oppo_saved) {
            if (in > oppo_saved) str << '+';
            if (in > 0)          green_cup = true;
        }
        str << in - oppo_saved;
    }
    else {
        GraLib::get_icon(cup_style).draw(*ground, x_cup_icon, y, frame_target,
         frame_gray);
        str << tr->required;
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
        if (tarinf->get_climber() || tarinf->get_floater()) {
            ab += " (";
            if (tarinf->get_climber() && tarinf->get_floater()) ab += 'A';
            else if (tarinf->get_climber()) ab += 'C';
            else if (tarinf->get_floater()) ab += 'F';
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



// ############################################################################
// ############################################################################
// ############################################################################



GameplayStats::GameplayStats()
:
    Element    (0, 0, 0, 20),
    stats_bar  (0, 0, 20, 20),
    stats_multi(0, 0, 20, 60),
    show_clock (false),
    tarcnt     (0),
    tarinf     (0),
    clock      (0)
{
    add_child(stats_bar);
    add_child(stats_multi);
}

GameplayStats::~GameplayStats()
{
}



void GameplayStats::add_tribe(const Tribe& t)
{
    tribes.push_back(PanelTribe(get_ground()));
    tribes.back().tr          = &t;
    tribes.back().name_short  = t.get_name();
    tribes.back().name_medium = t.get_name();

    // Shorten names whenever someone new is added
    if (tribes.size() > 1) // Against division by 0
     for (int i = 0; i < (int) tribes.size(); ++i) {
        std::string& str = tribes[i].name_short;
        std::string& med = tribes[i].name_medium;
        while (text_length(font_med, str.c_str())
         > PanelTribe::name_short_width
         && str.size() > 1) {
            str.resize(str.size() - 1);
        }
        while (text_length(font_med, med.c_str())
         > PanelTribe::name_medium_width
         && med.size() > 2) {
            med.resize(med.size() - 2);
            med += '.';
        }
    }
    set_draw_required();
}



void GameplayStats::set_tribe_local(const Tribe& t)
{
    for (std::vector <PanelTribe> ::iterator
     itr = tribes.begin(); itr != tribes.end(); ++itr)
     if (itr->tr == &t) {
        itr->white = true;
        break;
    }
}



void GameplayStats::calc_self()
{
    stats_bar.set_down  (false);
    stats_multi.set_down(false);
}



void GameplayStats::draw_self()
{
    // Zunaechst alle alten Aufschriften loeschen: Das Setzen der X-Koordinate
    // und der Laenge setzt draw_required per Element-Basisfunktionen.
    stats_bar.set_x(0);
    bool multi = tribes.size() > 1;
    stats_bar.set_xl(12 * 40);

    stats_bar.set_draw_required();
    stats_bar.draw();

    if (tribes.empty()) return;

    bool         cup_colored     = false;
    LixEn::Style cup_style       = tribes[0].tr->style; // maybe for 1-player
    int          oppo_saved      = 0;
    int          leader_saved    = 0;

    bool         cupall_colored  = false;
    LixEn::Style cupall_style    = tribes[0].tr->style; // maybe for 1-player
    int          oppo_expected   = 0;
    int          leader_expected = 0;

    if (multi) {
        // How many lix did the best opponent save? --> oppo_saved.
        // ``required'' remembers which scores should be green, which are
        // those of the leading players among all players.
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr) {
            if (itr->tr->get_score() > leader_saved)
             leader_saved = itr->tr->get_score();
            if (itr->tr->get_score_expected() > leader_expected)
             leader_expected = itr->tr->get_score_expected();
            if (!itr->white) {
                if (itr->tr->get_score() > oppo_saved)
                 oppo_saved = itr->tr->get_score();
                if (itr->tr->get_score_expected() > oppo_expected)
                 oppo_expected = itr->tr->get_score_expected();
            }
        }
        // Color the cup if there is exactly one leading player
        cup_colored    = true;
        cupall_colored = true;
        int leader_saved_count    = 0;
        int leader_expected_count = 0;
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr) {
            if (itr->tr->get_score() == leader_saved) {
                ++leader_saved_count;
                cup_style = itr->tr->style;
            }
            if (itr->tr->get_score_expected() == leader_expected) {
                ++leader_expected_count;
                cupall_style = itr->tr->style;
            }
        }
        if (leader_saved_count    > 1) cup_colored    = false;
        if (leader_expected_count > 1) cupall_colored = false;
    }

    // Zunaechst den Spieler mit weisser Schrift nach links malen
    for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
     itr != tribes.end(); ++itr)
     if (itr->white) {
        itr->draw_local(get_x_here(), get_y_here(),
         multi, show_clock ? &clock : 0, tarinf, tarcnt,
         cup_style,    cup_colored,    oppo_saved,
         cupall_style, cupall_colored, oppo_expected);
        break;
    }

    if (multi) {
        stats_multi.set_x (12 * 40);
        stats_multi.set_xl( 4 * 40);
        stats_multi.show  ();
        stats_multi.set_draw_required();
        stats_multi.draw();

        int x = stats_multi.get_x_here();
        int plus_x = 40;

        if (tribes.size() == 2) {
            x      += 30;
            plus_x =  60;
        }
        else if (tribes.size() == 3) {
            x      += 10;
            plus_x =  50;
        }

        // Draw the local player first for reference.
        // He will be drawn twice in multiplyaer, because his main stats
        // are also in the panel's top bar.
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr)
         if (itr->white) {
            itr->draw_med(x, stats_multi.get_y_here(), leader_saved);
            x += plus_x;
            break;
        }

        // Sort the other players with a stable sort.
        typedef std::list <const PanelTribe*>            PtrList;
        typedef std::list <const PanelTribe*> ::iterator PtrItr;

        PtrList sorted;
        PtrList candidates;
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr)
         if (!itr->white) candidates.push_back(&*itr);
        while (!candidates.empty() && sorted.size() < 3) {
            PtrItr best = candidates.begin();
            for (PtrItr better = ++candidates.begin();
             better != candidates.end(); ++better) {
                if (*better > *best) best = better;
            }
            sorted.push_back(*best);
            candidates.erase(best);
        }
        // end of sort

        for (PtrItr itr = sorted.begin(); itr != sorted.end(); ++itr) {
            (**itr).draw_med(x, stats_multi.get_y_here(), leader_saved);
            x += plus_x;
        }
        draw_button_connection();
    }
    else stats_multi.hide();
}



void GameplayStats::draw_button_connection()
{
    // Code taken from api/button.cpp
    BITMAP* g    = get_ground().get_al_bitmap();
    const int x1 = stats_multi.get_x_here() - 2;
    const int y1 = stats_multi.get_y_here();
    const int x2 = stats_multi.get_x_here() + 1;
    const int y2 = stats_multi.get_y_here() + 19;

    const int color_1 = color[COL_API_L   ];
    const int color_2 = color[COL_API_M   ];
    const int color_3 = color[COL_API_D   ];

    acquire_bitmap(g);

    rectfill(g, x1,   y1,   x2,   y1+1, color_1); // top
    rectfill(g, x1,   y1+2, x2,   y2-2, color_2); // center
    hline   (g, x1,   y2-1, x2-1,       color_3); // close bottom left
    hline   (g, x1,   y2,   x2-2,       color_3); // close bottom left
    putpixel(g, x2,   y2-1,             color_2); // bottom right corner
    putpixel(g, x2-1, y2,               color_2); // bottom right corner
    putpixel(g, x2,   y1,               color_1); // bottom right corner

    release_bitmap(g);
}
