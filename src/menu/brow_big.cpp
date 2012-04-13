/*
 * menu/brow_big.cpp
 *
 */

#include "brow_big.h"

#include "../api/manager.h"
#include "../level/level.h"
#include "../other/language.h"
#include "../other/help.h" // remove root dir in window title
#include "../other/user.h" // exit button hotkey

static const int freetext_y   (10);
static const int freetext_yd  (19);
static const int freetext_size((Api::BrowserBig::any_list_yl
                               - 2 * freetext_y) / freetext_yd);

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
    cover_frame(lev_list.get_x(), lev_list.get_y(), lev_list.get_xl(),
                                                    lev_list.get_yl()),
    cover_desc (freetext_size),
    button_play(but_x, 40 + (but_yl + but_y_spacing) * 0,
                but_xl, but_yl),
    button_exit(but_x, LEMSCR_Y - 60,
                but_xl, but_yl),
    preview    (but_x, 20 + but_yl + 2 * but_y_spacing,
                but_xl, pre_yl)
{
    add_child(dir_list);
    add_child(lev_list);
    add_child(cover_frame);
    add_child(button_play);
    add_child(button_exit);
    add_child(preview);

    for (size_t i = 0; i < cover_desc.size(); ++i) {
        cover_desc[i].set_align(Api::Label::CENTERED);
        cover_desc[i].set_x    (lev_list.get_x() + lev_list.get_xl() / 2);
        cover_desc[i].set_y    (lev_list.get_y() + freetext_y + i*freetext_yd);
        add_child(cover_desc[i]);
    }

    lev_list.set_activate_clicked_button();
    lev_list.set_checkmark_style(checkmark_style);
    lev_list.set_replay_style   (replay_style);

    button_play.set_hotkey(useR->key_me_okay);

    button_exit.set_text(Language::back);
    button_exit.set_hotkey(useR->key_me_exit);

    // second argument == false:
    // on_level_highlight() muss von der abgeleiteten Klasse aufgerufen
    // werden, weil diese zu diesem Konstruktoraufruf-Zeitpunkt noch
    // nicht konstruiert ist.
    load_dir(dir_list.get_current_dir(), false);
}



BrowserBig::~BrowserBig()
{
}



void BrowserBig::load_dir(const Filename& fn, bool hooks)
{
    dir_list.set_current_dir(fn);
    lev_list.load_dir(fn);

    set_subtitle(dir_list.get_current_dir().get_dir_rootless());

    lev_list.highlight_file(file_recent);
    if (hooks) on_file_highlight(lev_list.get_current_file());

    if (lev_list.get_files_total() > 0) {
        lev_list.show();
        cover_frame.hide();
        for (size_t i = 0; i < cover_desc.size(); ++i)
            cover_desc[i].hide();
    }
    else {
        lev_list.hide();
        cover_frame.show();

        std::vector <std::string> fstr;
        if (! IO::fill_vector_from_file_raw(fstr,
         dir_list.get_current_dir().get_rootful() +
         (Language::get() == Language::GERMAN ? gloB->file_level_dir_german
                                              : gloB->file_level_dir_english)))
        {
            // if file doesn't exist, load the other language
            IO::fill_vector_from_file_raw(fstr,
             dir_list.get_current_dir().get_rootful() + (Language::get()
                          == Language::GERMAN ? gloB->file_level_dir_english
                                              : gloB->file_level_dir_german));
        }
        for (size_t i = 0; i < cover_desc.size(); ++i) {
            cover_desc[i].show();
            cover_desc[i].set_text(fstr.size() > i ? fstr[i] : "");
        }
    }
}



void BrowserBig::set_current_dir_to_parent_dir()
{
    dir_list.set_current_dir_to_parent_dir();
    load_dir(dir_list.get_current_dir());
}



void BrowserBig::calc_self()
{
    if (dir_list.get_clicked()) {
        load_dir(dir_list.get_current_dir());
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



void BrowserBig::highlight_nothing()
{
    lev_list.highlight_file(dir_list.get_current_dir());
    on_file_highlight      (lev_list.get_current_file());
}

}
// Ende des Namensraumes Api
