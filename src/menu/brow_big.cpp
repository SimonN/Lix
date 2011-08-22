/*
 * menu/brow_big.cpp
 *
 */

#include "brow_big.h"

#include "../api/manager.h"
#include "../level/level.h"
#include "../other/language.h"
#include "../other/help.h" // remove root dir in window title

namespace Api {

BrowserBig::BrowserBig(const std::string& wintitle,
                       const Filename&    basedir,
                       const Filename&    lastfilename,
                       const bool checkmark_style,
                       const bool replay_style)
:
    Window(0, 0, LEMSCR_X, LEMSCR_Y, wintitle),
    exit_with  (EXIT_WITH_NOTHING),
    info_y     (200),
    file_recent(lastfilename),
    dir_list   (20, 40,
                dir_list_xl, any_list_yl, basedir, lastfilename),
    lev_list   (40+dir_list_xl, 40,
                lev_list_xl, any_list_yl),
    button_play(but_x, 40 + (but_yl + but_y_spacing) * 0,
                but_xl, but_yl),
    button_exit(but_x, LEMSCR_Y - 60,
                but_xl, but_yl),
    preview    (but_x, 20 + but_yl + 2 * but_y_spacing,
                but_xl, pre_yl)
{
    add_child(dir_list);
    add_child(lev_list);
    add_child(button_play);
    add_child(button_exit);
    add_child(preview);

    lev_list.set_activate_clicked_button();
    lev_list.set_checkmark_style(checkmark_style);
    lev_list.set_replay_style   (replay_style);
    lev_list.load_dir           (dir_list.get_current_dir());
    lev_list.highlight_file     (lastfilename);
    set_subtitle(dir_list.get_current_dir().get_dir_rootless());

    // on_level_highlight() muss von der abgeleiteten Klasse aufgerufen
    // werden, weil diese zu diesem Konstruktoraufruf-Zeitpunkt noch
    // nicht konstruiert ist.
    button_exit.set_text(Language::back);
    button_exit.set_hotkey(KEY_ESC);
}



BrowserBig::~BrowserBig()
{
}



void BrowserBig::set_current_dir_to_parent_dir()
{
    dir_list.set_current_dir_to_parent_dir();
    lev_list.load_dir(dir_list.get_current_dir());
}



void BrowserBig::calc_self()
{
    if (dir_list.get_clicked()) {
        lev_list.load_dir(dir_list.get_current_dir());
        set_subtitle(dir_list.get_current_dir().get_dir_rootless());
        lev_list.highlight_file(file_recent);
        on_file_highlight      (lev_list.get_current_file());
    }
    else if (lev_list.get_clicked()) {
        Button* b = lev_list.get_button_last_clicked();
        // Gerade zum ersten Mal angeklickt
        if (b->get_on()) {
            file_recent = lev_list.get_current_file();
            on_file_highlight(lev_list.get_current_file());
        }
        // Button schon angeklickt?
        else {
            b->set_on(); // Damit ein nichtguter Level nicht deselektiert wird
            on_file_select(lev_list.get_current_file());
        }
    }
    else if (button_play.get_clicked()) {
        on_file_select(lev_list.get_current_file());
    }
    else if (button_exit.get_clicked()) {
        set_exit_with(EXIT_WITH_EXIT);
    }
}



void BrowserBig::set_exit_with(BrowserBig::ExitWith e)
{
    Manager::remove_focus(this);
    exit_with = e;
}



void BrowserBig::reload_dir()
{
    lev_list.load_dir(lev_list.get_current_dir());
}

}
// Ende des Namensraumes Api
