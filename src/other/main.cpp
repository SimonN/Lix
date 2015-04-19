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
 *   Written in 2006 to right now by Simon <eiderdaus@gmail.com> and others.
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

#include <iostream>

#include "lmain.h" // Main object to manage the different parts of the program
#include "user.h"
#include "verify.h"
#include "file/log.h"
#include "language.h"

#include "../lix/lix_enum.h" // initialize strings
#include "../graphic/png/loadpng.h"

struct MainArgs {
    bool print_version_and_exit;
    bool print_help_and_exit;
    bool input_user_name;
    std::string graphics_driver;
    bool resolution_or_fullscreen_suggested_via_args;
    bool fullscreen;
    int  screen_xl;
    int  screen_yl;
    bool suppress_sound_driver;
    std::vector <std::string> replays_to_verify;
};
static MainArgs parse_main_arguments(int, char*[]);
static void     setenv_allegro_modules();
static void     unsetenv_allegro_modules();
static void     print_usage();



int main(int argc, char* argv[])
{
    MainArgs margs = parse_main_arguments(argc, argv);

    if (margs.print_version_and_exit) {
        std::cout << Help::version_to_string(Globals::version) << std::endl;
        return 0;
    }
    else if (margs.print_help_and_exit) {
        print_usage();
        return 0;
    }

    // From here, load either the game or the interactive replay checker.
    // This will depend on existing dirs later on.

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
    if (margs.input_user_name) {
        gloB->user_name = "";
    }
    useR->load();

    // Graphics are needed both by interactive and noninteractive mode
    ::set_color_depth(16);

    if (margs.replays_to_verify.empty()) {
        // interactive mode, i.e., no replay checking mode

        install_keyboard();
        hardware.set_mouse_accel_on_windows(useR->mouse_acceleration);
        install_mouse();
        if (! margs.suppress_sound_driver && gloB->sound_load_driver)
            Sound::initialize();

        if (margs.resolution_or_fullscreen_suggested_via_args)
            set_screen_mode(margs.fullscreen, margs.graphics_driver,
                            margs.screen_xl,  margs.screen_yl);
        else
            set_screen_mode(! useR->screen_windowed,
                            margs.graphics_driver, 0, 0);

        // BUG: in Windows, setting the window title with non-ASCII UTF-8
        // string does not seem to work (Allegro bug?)--non-ASCII characters
        // turn into ^.  Workaround by forcing use of English string here.
        //
        // (should main_name_of_the_game even be allowed for translation?)
        set_window_title(Language::main_name_of_game_English);

        if (useR->language == Language::NONE
         || useR->language >= Language::MAX) {
            // This is for the "initial use" case where there is no
            // user profile and therefore no language set.
            //
            // It is preferrable in that case to load the custom language
            // if it exists, so that we display the "loading..." texts (inside
            // load_all_bitmaps() below) text in the language the user probably
            // wants (the custom one if it exists, otherwise English)
            static_cast<void>(Language::try_load_custom_language(false));
        }

        load_all_bitmaps(GraLib::LOAD_WITH_RECOLOR_LIX);
        Network::initialize();

        // Main loop. See other/lmain.cpp for this.
        LMain* l_main = new LMain;
        l_main->main_loop();
        delete l_main;

        // Clean up
        useR->save();
        gloB->save();

        destroy_all_bitmaps();

        Network::deinitialize();
        Sound::deinitialize();
    }

    else {
        // noninteractive mode that checks replays
        Help::interactive_mode = false;
        Verifier(margs.replays_to_verify);
    }

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
    main_args.print_version_and_exit = false;
    main_args.print_help_and_exit = false;
    main_args.graphics_driver = "";
    main_args.input_user_name = false;
    main_args.resolution_or_fullscreen_suggested_via_args = false;
    main_args.fullscreen = true;
    main_args.screen_xl = 0;
    main_args.screen_yl = 0;
    main_args.suppress_sound_driver = false;
    main_args.replays_to_verify.clear();

    // Check all arguments for any occurence of the switch-defining letters
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.substr(0, 10) == "--gfxmode=") {
            main_args.graphics_driver = arg.substr(10, std::string::npos);
        }
        else if (arg.substr(0, 6) == "--help") {
            main_args.print_help_and_exit = true;
        }
        else if (arg.substr(0, 9) == "--verify=") {
            main_args.replays_to_verify.push_back(
             arg.substr(9, std::string::npos));
        }
        else if (arg.substr(0, 9) == "--version") {
            main_args.print_version_and_exit = true;
        }
        else if (arg.size() >= 2 && arg[0] == '-' && arg[1] != '-') {
            for (size_t pos = 1; pos < arg.size(); ++pos) switch (arg[pos]) {
            case 'w':
                main_args.resolution_or_fullscreen_suggested_via_args = true;
                main_args.fullscreen = false;
                main_args.screen_xl = LEMSCR_X;
                main_args.screen_yl = LEMSCR_Y;
                break;

            case 'h':
                main_args.print_help_and_exit = true;
                break;

            case 'n':
                main_args.input_user_name = true;
                break;

            case 'o':
                main_args.suppress_sound_driver = false;
                break;

            case 'v':
                main_args.print_version_and_exit = true;
                break;

            default:
                break;
            }
        }
    }
    return main_args;
}
// end of parse_main_arguments()



void print_usage()
{
    // this is always in English, because we don't load the user language
    // until much later.
    std::cout << "Lix version " << Help::version_to_string(Globals::version)
<< " command line arguments:" << std::endl
<< "-v or --version     print version and exit\n"
<< "-h or --help        print this help and exit\n"
<< "-w                  force 640x480 windowed mode\n"
<< "-n                  always ask for player's name at startup\n"
<< "-o                  suppress loading the sound driver\n"
<< "--gfxmode=MODE      on Windows, suggest a gfx driver, see doc/readme.txt\n"
<< "--verify=file.txt   verify a single replay file, see doc/readme.txt\n"
<< "--verify=directory  verify all replay files recursively in directory"
<< std::endl;
}



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
