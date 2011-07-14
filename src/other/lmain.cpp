/*
 * other/lmain.cpp
 *
 */

#include "lmain.h"
#include "user.h"

#include "../api/manager.h"
#include "../graphic/sound.h"

LMain::LMain()
:
    exit    (false),

    // We add +1 here to remedy the short pink flicker when loading the game
    // in windowed mode. And +5 is five times as good as +1 <_<;
    tck     (Help::timer_ticks + 5),

    gameplay_started_with_replay(false),

    menu    (new Menu()),
    editor  (0),
    gameplay(0)
{
}

LMain::~LMain()
{
    // Delete whatever is left
    if (editor)   delete editor;
    if (gameplay) delete gameplay;
    if (menu)     delete menu;
}

void LMain::undraw()
{
    if (editor)   editor  ->undraw();
    if (gameplay) gameplay->undraw();
    if (menu)     menu    ->undraw();
}



void LMain::main_loop()
{
    while (true) {

        undraw();

        // Depending on the vsync option, I do either frameskipping or
        // a set number of logic cycle calculations.
        if (gloB->screen_vsync) {
            const unsigned loops = Help::get_timer_ticks_per_draw() + 1;
            rest(1);
            while (++tck % loops != 0) calc();
            // To not have the program hang when the user changes the vsync
            // option, set tck here. The following "else" will start working
            // immediately in the next main_loop() call then.
            if (!gloB->screen_vsync) tck = Help::timer_ticks;
        }
        // If vsync is deactivated, do normal frameskipping: Calculate until
        // the logic is up to date, then draw. If the computer is slow, several
        // logic updates will be calcd before the draw.
        else {
            while (tck == Help::timer_ticks) {
                rest(1);
                // The mouse poll is something I've inserted here against
                // the Yassin bug. The Yassin bug is when clicks from the user
                // won't get registered by L++. I hope this helps.
                hardware.poll_mouse();
            }
            while (tck  < Help::timer_ticks) {
                ++tck;
                calc();
            }
        }

        if (exit) break;
        draw();
    }
    // This gets uncommented only whenever I want to test for exit crashes.
    // clear_to_color(screen, makecol(0, 0, 128));
    // rest(400);
}



void LMain::calc()
{
    hardware.main_loop();
    Api::Manager::calc();

    if (menu)
    {
        menu->calc();

        // Exit the menu?
        switch (menu->get_exit_with())
        {
        case Menu::START_LEVEL: {
            gameplay_started_with_replay = false;
            gameplay = new Gameplay();
            delete menu;
            menu = 0;
            break; }

        case Menu::START_REPLAY: {
            gameplay_started_with_replay = true;
            Replay r(useR->replay_last_level);
            gameplay = new Gameplay(&r);
            delete menu;
            menu = 0;
            break; }

        case Menu::START_EDITOR:
            editor = new Editor();
            delete menu;
            menu = 0;
            break;

        case Menu::EXIT_PROGRAM:
            exit = true;
            break;

        default:
            break;
        }
    }
    else if (gameplay) {
        gameplay->calc();

        if (gameplay->get_exit()) {
            SubMenu                           sub = SUBMENU_SINGLE;
            if (gameplay_started_with_replay) sub = SUBMENU_REPLAY;
            else if (Network::get_started())  sub = SUBMENU_NETWORK;

            menu = new Menu(sub);
            delete gameplay;
            gameplay = 0;
        }
    }
    else if (editor) {
        editor->calc();
        if (editor->get_exit()) {
            delete editor;
            editor = 0;
            menu = new Menu(SUBMENU_SINGLE);
        }
    }

    // Hotkey combination to terminate the program instantly from
    // everywhere. This doesn't bug the user about unsaved data.
    if (key[KEY_ESC] && key[KEY_LSHIFT]) {
        exit = true;
    }
    // Hotkey combination for fullscreen
    // Do not use key_enter_once() here, this is the time we want it with alt
    if (hardware.get_alt()
     && (hardware.key_once(KEY_ENTER) || hardware.key_once(KEY_ENTER_PAD))) {
        set_screen_mode(!gloB->screen_fullscreen_now);
    }

}



void LMain::draw()
{
    // The hereby managed classes blit the OSD and other things to the screen
    // by themselves. Only Sound and Api::manager are done from here.
    Api::Manager::draw();

    if (editor)   editor  ->draw();
    if (gameplay) gameplay->draw();
    if (menu)     menu    ->draw();

    Sound::draw();
}
