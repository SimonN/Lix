/*
 * gameplay/panel.cpp
 *
 * The GameplayStats::PanelTribe is in a different file altogether.
 *
 */

#include "pan_info.h"

GameplayStats::GameplayStats()
:
    Element    (0, 0, 0, 20),
    stats_bar  (0, 0, 20, 20),
    stats_multi(0, 0, 20, 60),
    frame_around_scores(0, 0, 20, 60),
    gapamode   (GM_NONE),
    show_countd(false),
    tarcnt     (0),
    tarinf     (0),
    countd     (0),
    stopw      (0),
    help       (3, 0)
{
    add_child(stats_bar);
    add_child(stats_multi);
    add_child(frame_around_scores);
    add_child(help);
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
        int len = ustrlen(str.c_str());
        while (text_length(font_med, str.c_str())
         > PanelTribe::name_short_width
         && len > 1) {
            Help::remove_last_utf8_char(str);
            len--;
        }
        len = ustrlen(med.c_str());
        while (text_length(font_med, med.c_str())
         > PanelTribe::name_medium_width
         && len > 2) {
            Help::remove_last_utf8_char(med);
            Help::remove_last_utf8_char(med);
            med += '.';
            len--;
        }
    }
    set_draw_required();
}



void GameplayStats::set_tribe_local(const Tribe* t)
{
    for (std::vector <PanelTribe> ::iterator
     itr = tribes.begin(); itr != tribes.end(); ++itr) {
        if (itr->tr == t) itr->white = true;
        else              itr->white = false;
    }
}



void GameplayStats::set_help(const std::string& s)
{
    if (help.get_text() != s) {
        help.set_text(s);
        set_draw_required();
    }
}



void GameplayStats::calc_self()
{
    stats_bar.set_down  (false);
    stats_multi.set_down(false);
}



void GameplayStats::draw_self()
{
    // Force clear of the bar
    stats_bar.set_x(0);
    stats_bar.set_xl(14 * 36);

    stats_bar.set_draw_required();
    stats_bar.draw();

    if (tribes.empty()) return;

    int oppo_saved = 0;

    const bool print_horiz_bars = gapamode == GM_PLAY_MULTI
                               || gapamode == GM_SPEC_MULTI;
                                       // not GM_REPLAY_MULTI!

    // How many lix did the best opponent save? --> oppo_saved.
    // ``required'' remembers which scores should be green, which are
    // those of the leading players among all players.
    for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
     itr != tribes.end(); ++itr) {
        if (!itr->white) {
            if (itr->tr->get_score() > oppo_saved)
             oppo_saved = itr->tr->get_score();
        }
    }

    // draw the local player
    if (help.get_text().empty()) {
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr)
         if (itr->white) {
            itr->draw_local(get_x_here(), get_y_here(), tribes.size() > 1,
               show_countd ? &countd : 0,
             ! show_countd ? &stopw  : 0,
             tarinf, tarcnt, oppo_saved);
            break;
        }
    }

    if (print_horiz_bars) {
        stats_multi.set_x (stats_bar.get_xl());
        stats_multi.set_xl(4 * 34);
        stats_multi.show  ();
        stats_multi.set_draw_required();
        stats_multi.draw();

        // all the horiz-bar drawing occurs in the following rectangle
        // we're computing real screen drawing coordinates here
        const int bar_x  = stats_multi.get_x_here() + 4;
        const int bar_xl = stats_multi.get_xl() - 8;
        const int bar_y  = stats_multi.get_y_here() + 4;
        const int bar_yl = stats_multi.get_yl() - 8;

        frame_around_scores.set_x (bar_x - this->get_x_here());
        frame_around_scores.set_xl(bar_xl);
        frame_around_scores.set_y (bar_y - this->get_y_here());
        frame_around_scores.set_yl(bar_yl);
        frame_around_scores.show  ();
        frame_around_scores.set_draw_required();
        frame_around_scores.draw();

        BITMAP* g = get_ground().get_al_bitmap();
        rectfill(g, bar_x, bar_y, bar_x+bar_xl-1, bar_y+bar_yl-1,
            color[COL_BLACK]);

        // Sort all players, including the local (white) one,
        // with a stable sort. While doing so, collect the info about the
        // highest potential (expected score) among all players
        int highest_expected = 1; // don't crash when dividing by this

        typedef std::list   <const PanelTribe*>            PtrList;
        typedef std::list   <const PanelTribe*> ::iterator PtrItr;
        typedef std::vector <const PanelTribe*>            PtrVec;

        PtrVec  sorted;
        PtrList candidates;
        for (std::vector <PanelTribe> ::const_iterator itr = tribes.begin();
         itr != tribes.end(); ++itr) {
            candidates.push_back(&*itr);
            if (itr->tr->get_score_expected() > highest_expected) {
                highest_expected = itr->tr->get_score_expected();
            }
        }
        while (!candidates.empty()) {
            PtrItr best = candidates.begin();
            for (PtrItr better = ++candidates.begin();
             better != candidates.end(); ++better) {
                if (**better > **best) best = better;
            }
            sorted.push_back(*best);
            candidates.erase(best);
        }
        // end of sort

        // draw a bar for each player found
        const int ssize = static_cast <int> (sorted.size());
        for (int i = 0; i < ssize; ++i) {
            const int saved = sorted[i]->tr->get_score();
            const int expec = sorted[i]->tr->get_score_expected();
            const int x  = bar_x;
            const int y  = bar_y + i * bar_yl / ssize;
            const int yl =     (i+1) * bar_yl / ssize - i * bar_yl / ssize;
            const int xl =     saved * bar_xl / highest_expected;
            // p stands for potential
            const int py  = y + (yl+1)/3;
            const int pyl = yl - 2 * (py - y);
            const int px  = x; // really (x + xl), but with (x), it comes out
            const int pxl = (expec * bar_xl / highest_expected); // - xl;
            if (expec > saved) {
                draw_clones_bar(px, py, pxl, pyl, sorted[i]->tr->style);
            }
            if (saved) {
                // draw this after the first bar, to partially overwrite it
                draw_clones_bar(x, y, xl, yl, sorted[i]->tr->style);
            }
        }

        draw_button_connection();
    }
    else {
        stats_multi.hide();
        frame_around_scores.hide();
    }
}



// this takes values in absolute screen coordinates already
void GameplayStats::draw_clones_bar(
    int x, int y, int xl, int yl,
    LixEn::Style style)
{
    BITMAP* g     = get_ground().get_al_bitmap();
    const int& x1 = x;
    const int& y1 = y;
    const int  x2 = x + xl - 1;
    const int  y2 = y + yl - 1;

    // bad: manual retrieval of the colors from the recoloring pallette
    // when porting to D, cache the colors instead in GraLib and offer
    // an interface to query colors by style
    const Cutbit& cb = GraLib::get(gloB->file_bitmap_lix_recol);
    const int col_l = cb.get_pixel(6, style + 1);
    const int col_m = cb.get_pixel(7, style + 1);
    const int col_d = cb.get_pixel(8, style + 1);

    rectfill(g, x1, y1, x2, y2, col_m);

    // drawing the border until we don't have room anymore, or we have
    // drawn the usual border thickness of 2;
    for (int bor = 0; bor < xl/2 && bor < yl/2 && bor < 2; ++bor) {
        // +/-0 instead of +/-1 means: this draws the corner pixels too
        hline(g, x1 + bor + 0, y1 + bor,     x2 - bor - 1, col_l);
        hline(g, x1 + bor + 1, y2 - bor,     x2 - bor - 0, col_d);
        vline(g, x1 + bor,     y1 + bor + 1, y2 - bor - 1, col_l);
        vline(g, x2 - bor,     y1 + bor + 1, y2 - bor - 1, col_d);
    }
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
