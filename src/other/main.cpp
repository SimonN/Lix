/*
 * other/main.cpp
 *
 * Introduction
 *
 *   This is the main source file for Lix. For build instructions, read the
 *   file ./doc/linux.txt.
 *
 * Public domain dedication
 *
 *   Lix -- a multiplayer-capable action-puzzle game
 *
 *   Written in 2006 to 2012 by Simon <eiderdaus@gmail.com> and others.
 *   See ./doc/copying.txt for the full credits for code, graphics, levels, ...
 *
 *   To the extent possible under law, the authors have dedicated all copyright
 *   and related and neighboring rights to this software to the public domain
 *   worldwide. This software is distributed without any warranty.
 *
 *   You should have received a copy of the CC0 Public Domain Dedication along
 *   with this software, see the file ./doc/copying.txt. If not, see
 *
 *     http://creativecommons.org/publicdomain/zero/1.0/
 *
 *   I believe that programming should be an entertaining hobby, not fiddling
 *   around with copyright or license restrictions. I've had and still have a
 *   lot of fun with the development and playtesting of this game. You should
 *   have as much fun as possible, too.
 *
 * About the source code itself
 *
 *   The source code consists of roughly 32,000 lines. Of these, 21,000 are
 *   actual code lines, the rest are comments and empty lines.
 *
 *   Have fun going through the source code!
 *
 * Simon
 *
 */

#include "lmain.h" // Main object to manage the different parts of the program
#include "user.h"

#include "file/log.h"
#include "../lix/lix_enum.h" // initialize strings

#include "../graphic/png/loadpng.h"

struct MainArgs {
    std::string scr_m;
    int  scr_f, scr_x, scr_y;
    bool sound_load_driver;
};
static MainArgs parse_main_arguments(int, char*[]);
static void     setenv_allegro_modules();
static void     unsetenv_allegro_modules();



int main(int argc, char* argv[])
{
    setenv_allegro_modules();
    allegro_init();
    unsetenv_allegro_modules();

    Help::timer_start();

    ::loadpng_init();

    Globals::initialize();
    User::initialize();
    Log::initialize();
    LixEn::initialize();

    // Check whether the Globals decided we're in one of the accepted
    // working directories, so all files are found. Otherwise, exit with error.
    if (! Help::dir_exists(gloB->dir_data_bitmap)) {
        allegro_message("%s", gloB->error_wrong_working_dir.c_str());
        Log::deinitialize();
        Globals::deinitialize();
        return -1;
    }

    gloB->load();
    useR->load();
    MainArgs margs = parse_main_arguments(argc, argv);

    // Allegro preparations, no graphics function are called yet
    install_keyboard();
    hardware.set_mouse_accel_on_windows(useR->mouse_acceleration);
    install_mouse();
    if (margs.sound_load_driver) Sound::initialize();

    // Allegro graphics
    set_color_depth(16);
    set_screen_mode(margs.scr_f, margs.scr_m, margs.scr_x, margs.scr_y);
    set_window_title(Language::main_name_of_the_game.c_str());

    load_all_bitmaps();
    Network::initialize();

    // Main loop. See other/lmain.cpp for this.
    LMain* l_main = new LMain;
    l_main->main_loop();
    delete l_main;

    // Clean up
    useR->save();
    gloB->save();

    Network::deinitialize();
    destroy_all_bitmaps();
    Sound::deinitialize();
    Log::deinitialize();
    Globals::deinitialize();

    // don't call allegro_exit(), doing that causes the program
    // to not terminate in rare cases
    return 0;
}
END_OF_MAIN()



// ############################################################################
// ############################################################################
// ############################################################################



MainArgs parse_main_arguments(int argc, char *argv[])
{
    MainArgs main_args;
    main_args.scr_m = "";
    main_args.scr_f = !useR->screen_windowed;
    main_args.scr_x = 0;
    main_args.scr_y = 0;
    main_args.sound_load_driver = gloB->sound_load_driver;

    // Check all arguments for any occurence of the switch-defining letters
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.substr(0, 10) == "--gfxmode=") {
            main_args.scr_m = arg.substr(10, std::string::npos);
        }
        else if (! arg.empty() && arg[0] == '-') {
            for (size_t pos = 1; pos < arg.size(); ++pos) switch (arg[pos]) {
            case 'w':
                main_args.scr_f = false;
                main_args.scr_x = LEMSCR_X;
                main_args.scr_y = LEMSCR_Y;
                break;

            case 'n':
                // The global config file and the user file have already been loaded.
                // To enable the question for the user name, we remove some data again.
                gloB->user_name = "";
                useR->load();
                break;

            case 'o':
                main_args.sound_load_driver = false;
                break;

            default:
                break;
            }
        }
    }
    return main_args;
}
// end of parse_main_arguments()



// ############################################################################
// ############################################################################
// ############################################################################



// Allegro loads a few modules as shared libraries, but not with the usual
// process of linking against these libraries (and thus putting into the
// binary the path to them) but by looking in standard places, or where
// the environment variable ALLEGRO_MODULES points. To omit the need for a
// shellscript wrapper for Lix, we set the environment variable here, and
// delete it later again (or set it back to what it was before).
//
// If you have Allegro installed into your system, and there are no shared
// Allegro libs in the Lix binary directory, then this won't set any env vars.

#ifdef __unix__
    #include <sys/stat.h>
    static std::string* oldenv = 0;
#endif



void setenv_allegro_modules()
{
#ifdef __unix__
    char* env = ::getenv("ALLEGRO_MODULES");
    if (env) oldenv = new std::string(env);
    else     oldenv = new std::string;

    struct ::stat statinfo;
    if (env && 0 == ::stat((*oldenv + "/modules.lst").c_str(), &statinfo)) {
        delete oldenv; // do nothing, user already supplied something good
        oldenv = 0;
    }
    else if (0 == ::stat("./bin/lib/allegro/modules.lst", &statinfo)) {
        ::setenv("ALLEGRO_MODULES", "bin/lib/allegro", 1);
    }
    else if (0 == ::stat("./lib/allegro/modules.lst", &statinfo)) {
        ::setenv("ALLEGRO_MODULES", "lib/allegro", 1);
    }
    else {
        delete oldenv; // we don't have to reset it
        oldenv = 0;
    }
#endif
}



void unsetenv_allegro_modules()
{
#ifdef __unix__
    if (oldenv) {
        if (oldenv->empty()) ::unsetenv("ALLEGRO_MODULES");
        else                 ::setenv  ("ALLEGRO_MODULES", oldenv->c_str(), 1);

        delete oldenv;
    }
#endif
}
