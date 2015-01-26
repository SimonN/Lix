// other/verify.cpp

#include "verify.h"
#include "globals.h"
#include "help.h"

#include "../gameplay/gameplay.h"

#include <iostream>

Verifier::Verifier(const Verifier::Vec& v)
:
    nr_empty_dir(0),
    nr_not_found(0),
    nr_not_naming_level(0),
    nr_level_not_found(0),
    nr_level_bad(0),
    nr_level_ok_failing(0),
    nr_level_ok_solving(0)
{
    std::cout << "Lix version " << Help::version_to_string(gloB->version)
     << " replay verifier, see doc/readme.txt for details"
     << std::endl;

    std::cout << "Loading bitmaps... " << std::flush;
    load_all_bitmaps(GraLib::LOAD_WITHOUT_RECOLOR_LIX_FOR_SPEED);
    std::cout << "done." << std::endl;

    for (VecCIt itr = v.begin(); itr != v.end(); ++itr) {
        examine_string(*itr);
    }

    std::cout << std::endl << "Total results:" << std::endl;

    if (nr_empty_dir > 0) std::cout << "  " << nr_empty_dir
     << "x (EMPTY): directory given by your arguments exist, but is empty."
     << std::endl;

    if (nr_not_found > 0) std::cout << "  " << nr_not_found
     << "x (NO-REP): replay file given by your arguments doesn't exist."
     << std::endl;

    if (nr_not_naming_level > 0) std::cout << "  " << nr_not_naming_level
     << "x (IGNORE): replay ignored, it doesn't name a level file."
     << std::endl;

    if (nr_level_not_found > 0) std::cout << "  " << nr_level_not_found
     << "x (NO-LEV): replay ignored, it names a level file that doens't exist."
     << std::endl;

    if (nr_level_bad > 0) std::cout << "  " << nr_level_bad
     << "x (BADLEV): replay ignored, it names a level file with a bad level."
     << std::endl;

    if (nr_level_ok_failing > 0) std::cout << "  " << nr_level_ok_failing
     << "x (FAIL): replay names an existing level file, but doesn't solve it."
     << std::endl;

    if (nr_level_ok_solving > 0) std::cout << "  " << nr_level_ok_solving
     << "x (OK): replay names an existing level file and solves that level."
     << std::endl;

     destroy_all_bitmaps();
}



Verifier::~Verifier()
{
}



void Verifier::static_put_to_vec(const Filename& f, void* v)
{
    Verifier* ver = static_cast <Verifier*> (v);
    ver->filenames_from_find_tree.push_back(f);
}



void Verifier::examine_string(const std::string& str)
{
    Filename f(str);

    if (Help::dir_exists(f)) {
        filenames_from_find_tree.clear();
        Help::find_tree(f, gloB->mask_ext_replay, static_put_to_vec, this);
        if (filenames_from_find_tree.empty()) {
            ++nr_empty_dir;
            std::cout << "(EMPTY), " << f.get_rootless() << std::endl;
        }
        else for (FVecCIt itr = filenames_from_find_tree.begin();
            itr != filenames_from_find_tree.end(); ++itr) {
            verify_filename(*itr);
        }
    }
    else {
        // it's a normal filename, existing or not, but it's not a dir
        verify_filename(f);
    }
}



void Verifier::verify_filename(const Filename& f)
{
    const std::string f_str = f.get_rootless();
    Replay r(f);

    if (r.get_file_not_found()) {
        ++nr_not_found;
        std::cout << "(NO-REP) " << f_str << std::endl;
        return;
    }

    if (f == r.get_level_filename()) {
        ++nr_not_naming_level;
        std::cout << "(IGNORE) " << f_str << std::endl;
        return;
    }

    Level l(r.get_level_filename());
    if (l.get_status() == Level::BAD_FILE_NOT_FOUND) {
        ++nr_level_not_found;
        std::cout << "(NO-LEV) "
                  << f_str << " => " << r.get_level_filename().get_rootless()
                  << " " << r.get_player_local_name()
                  << std::endl;
        return;
    }
    else if (! l.get_good()) {
        ++nr_level_bad;
        std::cout << "(BADLEV) ";
        std::cout << f_str << " => " << r.get_level_filename().get_rootless()
                  << " " << r.get_player_local_name()
                  << std::endl;
        return;
    }

    Gameplay* gameplay = new Gameplay(Gameplay::VERIFY_MODE, &r);

    while (! gameplay->get_exit() && ! gameplay->will_run_forever()) {
        gameplay->calc();
    }

    const Result result(gameplay->get_result());
    const int saved = result.lix_saved;

    if (saved >= l.required) {
        ++nr_level_ok_solving;
        std::cout << "(OK) ";
    }
    else {
        ++nr_level_ok_failing;
        std::cout << "(FAIL) ";
    }
    std::cout << f_str << " => " << r.get_level_filename().get_rootless()
         << " " << r.get_player_local_name()
         << " " << "saved: " << saved << " / " << l.required
         << " " << "skills: " << result.skills_used
         << " " << "updates: " << result.updates_used
         << std::endl;

    delete gameplay;
}
