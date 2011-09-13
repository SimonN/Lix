/*
 * Network level browser
 *
 */

#include <sstream>

#include "browsers.h"

#include "../level/crunch.h"
#include "../other/user.h"

namespace Api {

NetworkBrowser::NetworkBrowser()
:
    BrowserBig(Language::browser_network_title,
                        gloB->dir_levels,
                        useR->network_last_level),
    info_hatches(but_x, get_info_y(),       but_xl),
    info_goals  (but_x, get_info_y() +  20, but_xl),
    info_initial(but_x, get_info_y() +  40, but_xl),
    info_skills (but_x, get_info_y() +  60, but_xl),
    info_clock  (but_x, get_info_y() + 100, but_xl)
{
    add_child(info_hatches);
    add_child(info_goals);
    add_child(info_initial);
    add_child(info_skills);
    add_child(info_clock);
    info_hatches.set_desc(Language::browser_info_hatches);
    info_goals  .set_desc(Language::browser_info_goals);
    info_initial.set_desc(Language::browser_info_initial);
    info_skills .set_desc(Language::browser_info_skills);
    info_clock  .set_desc(Language::browser_info_clock_2);
    info_hatches.set_undraw_color(color[COL_API_M]);
    info_goals  .set_undraw_color(color[COL_API_M]);
    info_initial.set_undraw_color(color[COL_API_M]);
    info_skills .set_undraw_color(color[COL_API_M]);
    info_clock  .set_undraw_color(color[COL_API_M]);

    set_button_play_text(Language::ok);
    on_file_highlight(get_current_file());
}



NetworkBrowser::~NetworkBrowser()
{
}



void NetworkBrowser::calc_self()
{
    BrowserBig::calc_self();

    if (hardware.get_mr() && get_exit_with() == EXIT_WITH_NOTHING)
        set_exit_with(EXIT_WITH_EXIT);

    if (get_exit_with() == EXIT_WITH_OKAY
     || get_exit_with() == EXIT_WITH_EXIT)
        useR->network_last_level = get_current_file();
}



void NetworkBrowser::on_file_highlight(const Filename& filename)
{
    Level l(filename);
    info_hatches.hide();
    info_goals  .hide();
    info_initial.hide();
    info_skills .hide();
    info_clock  .hide();

    // Wir gucken dies hier nach und nicht in BrowserBig, weil dort nicht
    // sicher ist, ob ein Level da sein soll oder ein Replay, etwa. Nur auf
    // beliebige Dateien koennen wir nicht kotnrollieren, weil sonst auch
    // das Basisverzeichnis eine gueltige Datei ist.
    if (l.get_status() == Level::BAD_FILE_NOT_FOUND) clear_preview();
    else set_preview(l);

    if (!l.get_good()) return;

    int hatches = l.pos[Object::HATCH].size();
    int goals   = l.pos[Object::GOAL] .size();
    int skills  = 0;
    for (unsigned i = 0; i != 8; ++i) skills += l.skill[i].nr;

    info_hatches.show();
    info_goals  .show();
    info_initial.show();
    info_skills .show();
    info_clock  .show();
    info_hatches.set_value(hatches);
    info_goals  .set_value(goals);
    info_initial.set_value(l.initial);
    info_skills .set_value(skills);
    info_clock  .set_value_seconds_as_time(l.seconds);
}



void NetworkBrowser::on_file_select(const Filename& filename)
{
    Level l(filename);
    if (l.get_status() == Level::GOOD) set_exit_with(EXIT_WITH_OKAY);
}

} // end namespace Api
