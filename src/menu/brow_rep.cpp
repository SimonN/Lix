/*E sw
 * menu/brow_rep.cpp
 *
 */

#include "browsers.h"

#include "../api/manager.h"
#include "../gameplay/replay.h"
#include "../level/level_me.h"
#include "../other/user.h"

namespace Api {

ReplayBrowser::ReplayBrowser()
:
    BrowserBig(Language::browser_replay_title,
               gloB->dir_replay,
               useR->replay_last_level,
               false, true), // kein Checkmark-Stil, aber Replay-Stil

    button_extract(but_x, LEMSCR_Y - 180,
                   but_xl, but_yl),
    button_delete (but_x, LEMSCR_Y - 120,
                   but_xl, but_yl),
    label_info_player (but_x, get_info_y()),
    desc_info_player  (but_x, get_info_y()),
    label_info_version(but_x, get_info_y() + 40),
    desc_info_version (but_x, get_info_y() + 40),
    label_info_built  (but_x, get_info_y() + 60),
    desc_info_built   (but_x, get_info_y() + 60),

    browser_save(0),
    box_delete  (0)
{
    add_child(button_extract);
    add_child(button_delete);

    add_child(desc_info_player);
    add_child(label_info_player);
    add_child(desc_info_version);
    add_child(label_info_version);
    add_child(desc_info_built);
    add_child(label_info_built);

    label_info_player.set_color(::color[COL_WHITE]);
    label_info_version.set_color(::color[COL_WHITE]);
    label_info_built.set_color(::color[COL_WHITE]);

    label_info_player .set_undraw_color(color[COL_API_M]);
    desc_info_player  .set_undraw_color(color[COL_API_M]);
    label_info_version.set_undraw_color(color[COL_API_M]);
    desc_info_version .set_undraw_color(color[COL_API_M]);
    label_info_built  .set_undraw_color(color[COL_API_M]);
    desc_info_built   .set_undraw_color(color[COL_API_M]);

    set_button_play_text(Language::browser_replay);
    button_extract.set_text(Language::browser_extract);
    button_extract.set_hotkey(useR->key_me_export);
    button_extract.set_undraw_color(color[COL_API_M]);
    button_extract.hide();
    button_delete .set_text(Language::browser_delete);
    button_delete .set_hotkey(useR->key_me_delete);
    button_delete .set_undraw_color(color[COL_API_M]);
    button_delete .hide();

    on_file_highlight(get_current_file());
}



ReplayBrowser::~ReplayBrowser()
{
    if (browser_save) delete browser_save;
    if (box_delete  ) delete box_delete;
}



void ReplayBrowser::calc_self()
{
    BrowserBig::calc_self();

    if (browser_save) {
        browser_save->set_draw_required();
        if (browser_save->get_exit()) {
            if (browser_save->get_exit_with()) {
                useR->single_last_level = browser_save->get_current_file();
                // Replay-Verzeichnis und -Datei wurden aktualisiert, als
                // auf den Extraktions-Button gedrueckt wurde
                // Extract exactly the replay, not the level it points to!
                Level  l(get_current_file());
                l.save_to_file(browser_save->get_current_file());
                Sound::play_loud(Sound::DISKSAVE);

                load_dir(get_current_file());
            }
            delete browser_save;
            browser_save = 0;
            set_draw_required();
        }
    }
    else if (box_delete) {
        box_delete->set_draw_required(); // Damit Buttons eingedrueckt werd.
        switch (box_delete->get_button_clicked()) {
        case 1:
            delete_file(get_current_file().get_rootful().c_str());
            load_dir(get_current_file());
            clear_preview();

            desc_info_player  .set_text();
            label_info_player .set_text();
            desc_info_version .set_text();
            label_info_version.set_text();
            desc_info_built   .set_text();
            label_info_built  .set_text();

            button_delete.hide();
            button_extract.hide();
            // faellt durch
        case 2:
            delete box_delete;
            box_delete = 0;
            set_draw_required();
            break;
        default: ;
        }
    }
    // Replaybrowser normal, also ohne Extraktionsdialog
    else {
        if (!button_extract.get_hidden() && button_extract.get_clicked()) {
            useR->replay_last_level = get_current_file();
            browser_save = new Api::SaveBrowser(gloB->dir_levels,
                           gloB->ext_level,
                           useR->single_last_level,
                           Api::SaveBrowser::search_criterion_level,
                           Api::SaveBrowser::new_box_overwrite_level);
            browser_save->set_y(50); // mittig, nicht der Editor-Standard
            browser_save->set_info_level_name(LevelMetaData(
             useR->replay_last_level).get_name());
            browser_save->set_info_filename(useR->replay_last_level);
            Manager::add_focus(browser_save);
        }
        else if (!button_delete.get_hidden() && button_delete.get_clicked()) {
            if (exists(get_current_file().get_rootful().c_str())) {
                Replay r(get_current_file());
                std::string s1 = Language::editor_file_name
                 + ' ' + get_current_file().get_rootless();
                std::string s2 = Language::editor_level_name
                 + ' ' + LevelMetaData(r.get_level_filename()).get_name();
                box_delete = new Api::BoxMessage(500, 3,
                                       Language::browser_box_delete_tit_rep);
                box_delete->add_text(Language::browser_box_delete_que_rep);
                box_delete->add_text(s1);
                box_delete->add_text(s2);
                box_delete->add_button(Language::common_yes,useR->key_me_okay);
                box_delete->add_button(Language::common_no, useR->key_me_exit);
                Manager::add_focus(box_delete);
            }
        }
        // Den normalen Dialog-Siedler-2-Rechtsklick realisieren:
        else if (hardware.get_mr()) {
            set_exit_with(EXIT_WITH_EXIT);
        }
    }
}



void ReplayBrowser::on_file_highlight(const Filename& filename)
{
    Replay r(filename);

    if (r.get_file_not_found()) {
        button_delete.hide();
        button_extract.hide();
        clear_preview();
        return;
    }

    // from here, the replay file exists.
    button_delete.show();

    // Try both the contained level l and the pointed-to level.
    // Use the contained level if possible.
    Level l(filename);
    if (l.get_good()) {
        button_extract.show();
    }
    else {
        button_extract.hide();
        l = Level(r.get_level_filename());
        if (! l.get_good()) {
            clear_preview();
            return;
        }
    }

    // From here, at least one level is good.
    // If both levels are good, we have preferred the contained one.
    // l is the pointed-to level only if the contained one is bad.
    set_preview(l);

    const unsigned long over = gloB->version;
    const unsigned long omin = gloB->version_min;
    const unsigned long rmin = r.get_version_min();
    const Date&         dlev = l.built;
    const Date&         drep = r.get_built_required();
    const bool   showcontain = ! button_extract.get_hidden();

    std::string pl = Language::browser_info_player  + ' ';
    std::string ti = Language::browser_info_version + ' ';
    std::string bu = Language::browser_info_built   + ' ';
    desc_info_player .set_text(pl);
    desc_info_version.set_text(ti);
    desc_info_built  .set_text(bu);

    pl +=               r.get_player_local_name();
    ti += rmin > over ? Language::browser_info_old
        : rmin < omin ? Language::browser_info_new
        :               Language::browser_info_same;
    bu += showcontain ? Language::browser_info_holds_level
        : drep > dlev ? Language::browser_info_old
        : drep < dlev ? Language::browser_info_new
        :               Language::browser_info_same;

    label_info_player.set_text(pl);
    label_info_version.set_text(ti);
    label_info_built.set_text(bu);
}



void ReplayBrowser::on_file_select(const Filename& filename)
{
    Replay r(filename);
    Level  l(filename);

    // like in on_file_highlight(), use l if good, use pointed only if l bad
    if (! l.get_good()) {
        l = Level(r.get_level_filename());
    }
    if (l.get_good()) {
        set_exit_with(EXIT_WITH_OKAY);
        useR->replay_last_level = filename;
    }
}

} // Ende Namensraum Api
