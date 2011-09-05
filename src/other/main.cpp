/*
 * other/main.cpp
 *
 * Introduction
 *
 *   This is the main file for Lix. The game is written in C++ and uses
 *   two libraries:
 *
 *   - Allegro (www.allegro.cc)  for graphics, sound and input,
 *   - enet    (enet.bespin.org) for networking.
 *
 * License
 *
 *   Lix is free software.
 *
 *   You may do anything you'd like with this game. Download, play, use, alter
 *   and spread Lix as you wish. There are no restrictions. I am just not
 *   responsible for any bugs, loss of data, or the like.
 *
 *   I believe that programming should be an entertaining hobby, not fiddling
 *   around with license restrictions. I've had and still have a lot of fun
 *   with the development and playtesting of this game. You should have as
 *   as possible, too.
 *
 * About the source code itself
 *
 *   The source code consists of about 850 KB. It consists of roughly 30,000
 *   lines. 20,000 of those are code lines, the rest are comments and empty
 *   lines.
 *
 *   The core of the program is an object of class LMain. Usually I have
 *   commented each class in its header file. For LMain, this is other/lmain.h.
 *
 * Have fun going through the souce code!
 *
 * Simon
 *
 */

#include "lmain.h" // Main object, as commented on above
#include "user.h"

#include "../lix/lix_enum.h" // initialize the strings in there

struct MainArgs {
    int  scr_f, scr_x, scr_y;
    bool sound_load_driver;
};
MainArgs parse_main_arguments(int, char*[]);



int main(int argc, char* argv[])
{
    allegro_init();
    Help::timer_start();

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
    install_mouse();
    if (margs.sound_load_driver) Sound::initialize();

    // Allegro graphics
    set_color_depth(16);
    set_screen_mode(margs.scr_f, margs.scr_x, margs.scr_y); // in glob_gfx.h
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
    main_args.scr_f = !useR->screen_windowed;
    main_args.scr_x = 0;
    main_args.scr_y = 0;
    main_args.sound_load_driver = gloB->sound_load_driver;

    // Check all arguments for any occurence of the switch-defining letters
    for (int i = 1; i < argc; ++i)
     for (unsigned pos = 0; argv[i][pos] != '\0'; ++pos)
     switch (argv[i][pos]) {
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
    return main_args;
}
// end of parse_main_arguments()
