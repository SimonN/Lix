#include <sstream>
#include <algorithm> // find in vector, in on_dir_load() with ordering file

#include "list_lev.h"
#include "button/b_text.h"

#include "../gameplay/replay.h"
#include "../level/level_me.h"
#include "../other/file/io.h"
#include "../other/help.h"
#include "../other/user.h"
#include "../other/language.h"

namespace Api {

ListLevel::ListLevel(const int x, const int y, const int xl, const int yl)
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



bool ListLevel::search_criterion(const Filename& fn)
{
    return fn.get_file()      != gloB->file_level_dir_order
     &&    fn.get_file()      != gloB->file_level_dir_english
     &&    fn.get_file()      != gloB->file_level_dir_german
     && (  fn.get_extension() == gloB->ext_level
     ||    fn.get_extension() == gloB->ext_level_orig
     ||    fn.get_extension() == gloB->ext_level_lemmini);
}



// If there is a file determining level orders, then sort the levels by this
// before buttons are drawn.
void ListLevel::on_dir_load()
{
    // the list of files has already been sorted by name. Sort stably by
    // appearance in the order file, if such a file exists at all.
    // We're thus sorting alphabetically twice, that's an inefficient kludge.
    sort_filenames_by_order_txt_then_alpha(
     get_file_list(), get_current_dir(), false);
}



void ListLevel::add_file_button(const int nr, const int which_from_file)
{
    std::ostringstream button_text;
    const Filename& f = get_file(which_from_file);
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
        button_text << f.get_file_no_ext_no_pre_ext() << ": ";
    }

    LevelMetaData lev;

    if (replay_style) {
        Replay r(f);
        lev = LevelMetaData(r.get_level_filename());
        button_text << lev.get_name();
    }
    else {
        lev = LevelMetaData(f);
        button_text << lev.get_name();
    }

    TextButton* t = new TextButton(0, nr*20, get_xl(), 20);
    t->set_text(button_text.str());
    t->set_left();

    if (checkmark_style) {
        const Result* result = useR->get_level_result(f);
        if (result)
         // == wird auch in menu/brow_sin.cpp verwendet
         t->set_check_frame(result->built == lev.built ? 1 : 2);
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



void sort_filenames_by_order_txt_then_alpha(
    std::vector <Filename>& files,
    const Filename& dir_with_order_file,
    bool we_sort_dirs // this is a little kludge, we add slashes to the
                      // entries in the order file to work with Filename::==
) {
    // Assume that the filenames to be sorted also start with
    // dir_with_oder_file and only have appended the order file's substring.
    std::vector <std::string> orders;
    bool file_exists = IO::fill_vector_from_file_raw(orders,
        dir_with_order_file.get_dir_rootful() + gloB->file_level_dir_order);

    std::vector <Filename> ::iterator next_unsorted = files.begin();

    if (file_exists) {
        // dirs can be named as "somedir" or "somedir/", both shall work
        if (we_sort_dirs)
         for (std::vector <std::string> ::iterator itr = orders.begin();
         itr != orders.end(); ++itr) {
            if (itr->empty()) continue;
            if (*--(itr->end()) != '/') itr->push_back('/');
        }

        // Sort whatever is encountered in the order file to the beginning of
        // 'files'. What is encountered earliest shall go at the very beginning.
        for (std::vector <std::string> ::iterator orit = orders.begin();
         orit != orders.end(); ++orit) {
            Filename fn(dir_with_order_file.get_dir_rootful() + *orit);
            std::vector <Filename> ::iterator found =
                std::find(next_unsorted, files.end(), fn);
            if (found != files.end()) {
                std::swap(*found, *next_unsorted);
                ++next_unsorted;
                if (next_unsorted == files.end()) break;
            }
        }
    }
    // done processing the ordering file

    // sort the remaining items alphabetically
    std::sort(next_unsorted, files.end());
}



} // Ende Namensraum Api
