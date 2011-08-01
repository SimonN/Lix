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
    gapamode   (GM_NONE),
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



void GameplayStats::set_tribe_local(const Tribe* t)
{
    for (std::vector <PanelTribe> ::iterator
     itr = tribes.begin(); itr != tribes.end(); ++itr) {
        if (itr->tr == t) itr->white = true;
        else              itr->white = false;
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

    const bool table = (gapamode == GM_PLAY_MULTI || gapamode==GM_SPEC_MULTI);
    const bool cups  = (gapamode == GM_REPLAY_MULTI || table);

    if (cups) {
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
         cups, show_clock ? &clock : 0, tarinf, tarcnt,
         cup_style,    cup_colored,    oppo_saved,
         cupall_style, cupall_colored, oppo_expected);
        break;
    }

    if (table) {
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
                if (**better > **best) best = better;
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
