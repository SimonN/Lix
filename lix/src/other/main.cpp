/*
 * other/main.cpp
 *
 * Introduction
 *
 *   This is the main file for Lix. The game is written in C++ and uses
 *   two libraries:
 *
 *   - Allegro 5 (www.allegro.cc)  for graphics, sound and input,
 *   - enet 1.3  (enet.bespin.org) for networking.
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
 *   much fun as possible, too.
 *
 * About the source code itself
 *
 *   Class names have a capital first letter and use CamelCase.
 *
 *   Variables and functions use lower_space_names_with_underscores().
 *
 *   Some global objects are lowercasE witH capitaL lasT letteR.
 *
 *   In the rare case they're used, #define macros are in ALL CAPS. Most
 *   all-caps identifiers are Allegro types though, they start with ALLERGO_.
 *
 *   The core of the program is an object of class LMain. Usually I have
 *   commented each class in its header file. For LMain, this is other/lmain.h.
 *
 * Have fun going through the souce code!
 *
 * Simon
 *
 */

#include "lmain.h" // Main object, as commented upon above
#include "user.h"

#include "../lix/lix_enum.h" // initialize the strings in there

int main(int argc, char* argv[])
{
    al_init();
    Help::timer_initialize();

    Globals::initialize();
    User::initialize();
    Log::initialize();
    Hardware::initialize();
    LixEn::initialize();

    gloB->load();
    useR->load();
    Help::MainArgs margs = Help::parse_main_arguments(argc, argv);

    if (margs.sound_load_driver) Sound::initialize();

    // Allegro graphics
    set_screen_mode(margs.scr_f, margs.scr_x, margs.scr_y); // in glob_gfx.h

    load_all_bitmaps_and_fonts();
    Network::initialize();

    // Main loop. See other/lmain.cpp for this.
    LMain* l_main = new LMain;
    l_main->main_loop();
    delete l_main;

    // Clean up
    useR->save();
    gloB->save();

    Network::deinitialize();
    destroy_all_bitmaps_fonts_and_display();

    Sound::deinitialize();
    Hardware::deinitialize();
    Log::deinitialize();
    Globals::deinitialize();
    Help::timer_deinitialize();

    // don't call allegro_exit(), doing that causes the program
    // to not terminate in rare cases
    return 0;
}
