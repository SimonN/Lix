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
    tck     (Help::get_timer_ticks() + 5),

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
        // afdebug: no vsync supported right now
        while (tck == Help::get_timer_ticks()) {
            rest(1);
        }
        while (tck  < Help::get_timer_ticks()) {
            ++tck;
            calc();
        }

        if (exit) break;
        draw();
    }
}



void LMain::calc()
{
    Hardware::main_loop();
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
            Replay r(useR->replay_last_dir
                   + useR->replay_last_file);
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
    if (key[ALLEGRO_KEY_ESCAPE] && key[ALLEGRO_KEY_LSHIFT]) {
        exit = true;
    }
    // Hotkey combination for fullscreen
    // Do not use key_enter_once() here, this is the time we want it with alt
    if (Hardware::get_alt()
     && (Hardware::get_key_once(ALLEGRO_KEY_ENTER) || Hardware::get_key_once(ALLEGRO_KEY_ENTER_PAD))) {
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
