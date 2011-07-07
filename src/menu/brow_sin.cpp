/*
 * menu/browser.cpp
 *
 */

#include "browsers.h"

#include "../level/crunch.h"
#include "../other/user.h"

namespace Api {

SingleBrowser::SingleBrowser()
:
    BrowserBig(Language::browser_single_title,
                        gloB->dir_levels,
                        useR->single_last_dir,
                        useR->single_last_file,
                        true, false), // Checkmark-Stil, aber kein Replay-Stil

    button_edit(but_x, 40 + (but_yl + but_y_spacing) * 1,
                but_xl, but_yl),
    info_initial (but_x, 260, but_xl),
    info_required(but_x, 280, but_xl),
    info_clock   (but_x, 300, but_xl),
    rec_saved    (but_x, 340, but_xl),
    rec_skills   (but_x, 360, but_xl),
    rec_updates  (but_x, 380, but_xl),
    rec_altered_1(but_x, 360, Language::browser_info_result_old_1),
    rec_altered_2(but_x, 380, Language::browser_info_result_old_2)
{
    add_child(button_edit);
    add_child(info_initial);
    add_child(info_required);
    add_child(info_clock);
    add_child(rec_saved);
    add_child(rec_skills);
    add_child(rec_updates);
    add_child(rec_altered_1);
    add_child(rec_altered_2);

    info_initial .set_desc(Language::browser_info_lems);
    info_required.set_desc(Language::browser_info_required);
    info_clock   .set_desc(Language::browser_info_clock_1);
    rec_saved    .set_desc(Language::browser_info_result_saved);
    rec_skills   .set_desc(Language::browser_info_result_skills);
    rec_updates  .set_desc(Language::browser_info_result_time);

    set_button_play_text(Language::browser_play);
    button_edit.set_text(Language::browser_edit);

    set_preview_y(40 + (but_yl + but_y_spacing) * 2);

    on_file_highlight(get_current_dir() + get_current_file());
}

SingleBrowser::~SingleBrowser()
{
}



void SingleBrowser::calc_self()
{
    BrowserBig::calc_self();

    if (button_edit.get_clicked()) {
        on_file_select(get_current_dir() + get_current_file());
    }
}



void SingleBrowser::on_file_highlight(const std::string& filename)
{
    std::string cl;

    info_initial .hide();
    info_required.hide();
    info_clock   .hide();

    rec_saved    .hide();
    rec_skills   .hide();
    rec_updates  .hide();
    rec_altered_1.hide();
    rec_altered_2.hide();

    Level l(filename);
    if (l.get_status() == Level::BAD_FILE_NOT_FOUND) clear_preview();
    else set_preview(l);

    if (!l.get_good()) return;

    ////////////////////////////////////
    // Allgemeine Leveldaten anzeigen //
    ////////////////////////////////////

    info_initial .show();
    info_required.show();
    info_clock   .show();
    info_initial .set_value(l.initial);
    info_required.set_value(l.required);
    info_clock   .set_value_seconds_as_time(l.seconds);

    //////////////////////////////////////
    // Besten frueheren Erfolg anzeigen //
    //////////////////////////////////////

    const Result* result = useR->get_level_result(filename);

    if (result && result->lix_saved >= l.required) {
        rec_saved    .show();
        rec_saved    .set_value(result->lix_saved);
        // >= wird auch in api/list_lev.cpp verwendet fuer Haken-Darstellung
        if (result->built >= l.built) {
            rec_skills   .show();
            rec_updates  .show();
            rec_skills   .set_value(result->skills_used);
            rec_updates  .set_value_seconds_as_time(
                           result->updates_used / gloB->updates_per_second);
        }
        else {
            rec_altered_1.show();
            rec_altered_2.show();
        }
    }

}



void SingleBrowser::on_file_select(const std::string& filename)
{
    if (hardware.get_mrh() || button_edit.get_clicked()) {
        set_exit_with(EXIT_WITH_EDIT);
        useR->single_last_dir  = get_current_dir();
        useR->single_last_file = get_current_file();
    }
    else {
        Level l(filename);
        if (l.get_status() == Level::GOOD) {
            set_exit_with(EXIT_WITH_OKAY);
            useR->single_last_dir  = get_current_dir();
            useR->single_last_file = get_current_file();
        }
    }
}










NetworkBrowser::NetworkBrowser()
:
    BrowserBig(Language::browser_network_title,
                        gloB->dir_levels,
                        useR->network_last_dir,
                        useR->network_last_file),
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
    info_initial.set_desc(Language::browser_info_lems);
    info_skills .set_desc(Language::browser_info_skills);
    info_clock  .set_desc(Language::browser_info_clock_2);
    set_button_play_text(Language::ok);
    on_file_highlight(get_current_dir() + get_current_file());
}



NetworkBrowser::~NetworkBrowser()
{
}



void NetworkBrowser::calc_self()
{
    BrowserBig::calc_self();
    // Den normalen Dialog-Siedler-2-Rechtsklick realisieren:
    // on_file_select wird nochmal differenzieren, ob es per Rechtsklick
    // oder sonstwie aufgerufen wurde, falls der Level doof ist.
    if (hardware.get_mr()) {
        on_file_select(get_current_dir() + get_current_file());
    }
}



void NetworkBrowser::on_file_highlight(const std::string& filename)
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



void NetworkBrowser::on_file_select(const std::string& filename)
{
    Level l(filename);
    if (l.get_status() == Level::GOOD) {
        set_exit_with(EXIT_WITH_OKAY);
        useR->network_last_dir  = get_current_dir();
        useR->network_last_file = get_current_file();
    }
    else if (hardware.get_mr()) {
        set_exit_with(EXIT_WITH_EXIT);
    }
}

} // Ende Namensraum Api
