#include <sstream>

#include "list_lev.h"
#include "button/b_text.h"

#include "../gameplay/replay.h"
#include "../level/level.h"
#include "../other/help.h"
#include "../other/user.h"
#include "../other/language.h"

namespace Api {

ListLevel::ListLevel(const int x,  const int y, const int xl, const int yl)
:
    ListFile(x, y, xl, yl, search_criterion),
    write_file_names(false),
    replay_style    (false),
    checkmark_style (false)
{
}



ListLevel::~ListLevel()
{
}



bool ListLevel::search_criterion(const std::string& s)
{
    return Help::string_ends_with(s, gloB->ext_level)
     ||    Help::string_ends_with(s, gloB->ext_level_orig);
}



void ListLevel::add_file_button(const int nr, const int which_from_file)
{
    std::ostringstream button_text;
    std::string f = get_current_dir() + get_file(which_from_file);
    // Breite Leerzeichen (so breit wie eine Ziffer) schreiben, damit
    // alle Nummern gescheit untereinander liegen. '@' ist leergemalt.
    if (!write_file_names && !replay_style) {
        unsigned max = get_files_total();
        unsigned cur = which_from_file + 1;
        int      leading_spaces = 0;
        while (max /= 10) ++leading_spaces;
        while (cur /= 10) --leading_spaces;
        for (int i = 0; i < leading_spaces; ++i) button_text << '@';
        button_text << which_from_file + 1 << ". ";
    }
    else {
        std::string f_no_path = get_file(which_from_file);
        Help::string_remove_extension(f_no_path);
        button_text << f_no_path << ": ";
    }
    if (replay_style) {
        Replay r(f);
        button_text << Level::get_name(r.get_level_filename());
    }
    else {
        button_text << Level::get_name(f);
    }

    TextButton* t = new TextButton(0, nr*20, get_xl(), 20);
    t->set_text(button_text.str());
    t->set_left();

    if (checkmark_style) {
        const Result* result = useR->get_level_result(f);
        if (result)
         // >= wird auch in menu/brow_sin.cpp verwendet
         t->set_check_frame(result->built >= Level::get_built(f) ? 1 : 2);
    }
    button_push_back(t);
}



void ListLevel::add_flip_button()
{
    TextButton* t = new TextButton(0,
     get_bottom_button()*20, // 20 ist die Hoehe eines Buttons
     get_xl(), 20);
    t->set_text(Language::dir_flip_page);
    button_push_back(t);
}

} // Ende Namensraum Api
