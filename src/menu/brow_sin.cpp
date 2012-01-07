/*
 * menu/browser.cpp
 *
 */

#include <sstream>

#include "browsers.h"

#include "../api/manager.h"
#include "../level/crunch.h"
#include "../other/user.h"

namespace Api {

static int stats_y = 240;

SingleBrowser::SingleBrowser()
:
    BrowserBig(Language::browser_single_title,
                        gloB->dir_levels,
                        useR->single_last_level,
                        true, false), // Checkmark-Stil, aber kein Replay-Stil

    button_edit  (but_x, 40+but_yl, but_xl, but_yl),
    button_export(but_x, 360,       but_xl),
    button_delete(but_x, 380,       but_xl),
    info_author  (but_x, stats_y,      but_xl),
    info_initial (but_x, stats_y + 20, but_xl),
    rec_saved    (but_x, stats_y + 40, but_xl),
    rec_skills   (but_x, stats_y + 60, but_xl),
    rec_updates  (but_x, stats_y + 80, but_xl),
    rec_altered_1(but_x, stats_y + 60, Language::browser_info_result_old_1),
    rec_altered_2(but_x, stats_y + 80, Language::browser_info_result_old_2),
    browser_save(0),
    box_delete  (0)
{
    add_child(button_edit);
    add_child(button_export);
    add_child(button_delete);
    add_child(info_author);
    add_child(info_initial);
    add_child(rec_saved);
    add_child(rec_skills);
    add_child(rec_updates);
    add_child(rec_altered_1);
    add_child(rec_altered_2);

    info_author  .set_desc(Language::browser_info_author);
    info_author  .set_value_not_blocky();
    info_initial .set_desc(Language::browser_info_initgoal);
    rec_saved    .set_desc(Language::browser_info_result_saved);
    rec_skills   .set_desc(Language::browser_info_result_skills);
    rec_updates  .set_desc(Language::browser_info_result_time);
    info_author  .set_undraw_color(color[COL_API_M]);
    info_initial .set_undraw_color(color[COL_API_M]);
    rec_saved    .set_undraw_color(color[COL_API_M]);
    rec_skills   .set_undraw_color(color[COL_API_M]);
    rec_updates  .set_undraw_color(color[COL_API_M]);
    button_export.set_undraw_color(color[COL_API_M]);
    button_delete.set_undraw_color(color[COL_API_M]);

    set_button_play_text    (Language::browser_play);
    button_edit  .set_text  (Language::browser_edit);
    button_edit  .set_hotkey(useR->key_me_edit);
    button_export.set_hotkey(useR->key_me_export);
    button_delete.set_text  (Language::browser_delete);
    button_delete.set_hotkey(useR->key_me_delete);

    set_preview_y (40 + but_yl * 2 + but_y_spacing);
    set_preview_yl(88);

    on_file_highlight(get_current_file());
}

SingleBrowser::~SingleBrowser()
{
}



void SingleBrowser::calc_self()
{
    BrowserBig::calc_self();

    if (box_delete) {
        box_delete->set_draw_required(); // Damit Buttons eingedrueckt werd.
        switch (box_delete->get_button_clicked()) {
        case 1:
            ::delete_file(get_current_file().get_rootful().c_str());
            reload_dir();
            highlight_nothing();
            // falls through
        case 2:
            delete box_delete;
            box_delete = 0;
            set_draw_required();
            break;
        default: ;
        }
    }
    else {
        // No additional windows open
        if (button_edit.get_clicked()) {
            on_file_select(get_current_file());
        }
        else if (button_export.get_clicked()) {
            Level l(get_current_file());
            std::string s = get_current_file().get_rootless();
            for (size_t i = 0; i < s.size(); ++i) if (s[i] == '/') s[i] = '-';
            s += ".bmp";
            l.export_image(Filename(s));
            button_export.set_text(Language::browser_export_image_done);
        }
        else if (button_delete.get_clicked()) {
            if (exists(get_current_file().get_rootful().c_str())) {
                std::string s1 = Language::editor_file_name
                               + ' ' + get_current_file().get_rootless();
                std::string s2 = Language::editor_level_name
                               + ' ' + Level::get_name(get_current_file());
                box_delete = new Api::BoxMessage(500, 3,
                                       Language::browser_box_delete_tit_lev);
                box_delete->add_text(Language::browser_box_delete_que_lev);
                box_delete->add_text(s1);
                box_delete->add_text(s2);
                box_delete->add_button(Language::yes, useR->key_me_okay);
                box_delete->add_button(Language::no,  useR->key_me_exit);
                Manager::add_focus(box_delete);
            }
        }
    }
    // end of windows open/not open
}



void SingleBrowser::on_file_highlight(const Filename& filename)
{
    std::string cl;

    info_author  .hide();
    info_initial .hide();

    rec_saved    .hide();
    rec_skills   .hide();
    rec_updates  .hide();
    rec_altered_1.hide();
    rec_altered_2.hide();

    button_export.hide();
    button_export.set_text(Language::browser_export_image);
    button_delete.hide();

    Level l(filename);
    if (l.get_status() == Level::BAD_FILE_NOT_FOUND) clear_preview();
    else set_preview(l);

    if (!l.get_good()) return;

    ////////////////////////////////////
    // Allgemeine Leveldaten anzeigen //
    ////////////////////////////////////

    std::ostringstream initgoalval;
    initgoalval << l.required << "/" << l.initial;

    info_author  .show();
    info_initial .show();
    info_author  .set_value(l.author.empty()
                            ? Language::browser_info_author_none : l.author);
    info_initial .set_value(initgoalval.str());

    button_export.show();
    button_export.set_text(Language::browser_export_image);
    button_delete.show();

    //////////////////////////////////////
    // Besten frueheren Erfolg anzeigen //
    //////////////////////////////////////

    const Result* result = useR->get_level_result(filename);

    if (result && result->lix_saved >= l.required) {
        rec_saved    .show();
        rec_saved    .set_value(result->lix_saved);
        // == wird auch in api/list_lev.cpp verwendet fuer Haken-Darstellung
        if (result->built == l.built) {
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



void SingleBrowser::on_file_select(const Filename& filename)
{
    if (hardware.get_mrh() || button_edit.get_clicked()) {
        set_exit_with(EXIT_WITH_EDIT);
        useR->single_last_level = get_current_file();
    }
    else {
        Level l(filename);
        if (l.get_status() == Level::GOOD) {
            set_exit_with(EXIT_WITH_OKAY);
            useR->single_last_level = get_current_file();
        }
    }
}

} // Ende Namensraum Api
