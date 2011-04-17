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
    ALLEGRO_EVENT_QUEUE* lmain_queue = al_create_event_queue();

    al_register_event_source(lmain_queue,
                             al_get_timer_event_source(Help::timer));

    while ( ! exit) {
        ALLEGRO_EVENT event;
        al_wait_for_event(lmain_queue, &event);

        if (event.type == ALLEGRO_EVENT_TIMER) {
            calc();
        }
        if (al_is_event_queue_empty(lmain_queue)) {
            draw();
            undraw();
        }
    }

    al_destroy_event_queue(lmain_queue);
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
    if ((Hardware::get_shift() && Hardware::get_key_hold(ALLEGRO_KEY_ESCAPE))
     || Hardware::get_display_was_closed()) {
        exit = true;
    }
    // Hotkey combination for fullscreen
    // Do not use key_enter_once() here, this is the time we want it with alt
    if (Hardware::get_alt() && Hardware::get_key_enter_once()) {
        // afdebug: machen wir derzeit nicht
        // set_screen_mode(!gloB->screen_fullscreen_now);
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
