/*
 * menu.cpp
 *
 */

#include "menu.h"

#include "../api/manager.h"
#include "../graphic/gra_lib.h"
#include "../other/user.h"

Menu::Menu(SubMenu sm)
:
    exit_with          (NOTHING),
    main               (0),
    single             (sm == SUBMENU_SINGLE  ? new Api::SingleBrowser() : 0),
    lobby              (sm == SUBMENU_NETWORK ? new Api::Lobby()         : 0),
    replay             (sm == SUBMENU_REPLAY  ? new Api::ReplayBrowser() : 0),
    options            (0),
    new_player         (0),
    language           (0),

    mouse_cursor_offset(15),
    mouse_cursor(GraLib::get(gloB->file_bitmap_mouse),
     Api::Manager::get_torbit())
{
    if (single) Api::Manager::add_elder(single);
    if (lobby)  Api::Manager::add_elder(lobby);
    if (replay) Api::Manager::add_elder(replay);
}

Menu::~Menu() {
    if (main)       delete main;
    if (single)     delete single;
    if (lobby)      delete lobby;
    if (replay)     delete replay;
    if (options)    delete options;
    if (new_player) delete new_player;
    if (language)   delete language;
}



// ############################################################################
// ############################################################################
// ############################################################################



void Menu::undraw()
{
    mouse_cursor.undraw();
}



void Menu::calc()
{
    if (main) {
        switch (main->get_exit_with()) {

        case SUBMENU_NOTHING:
            break;

        case SUBMENU_SINGLE:
            delete main;
            main = 0;
            single = new Api::SingleBrowser();
            Api::Manager::add_elder(single);
            break;

        case SUBMENU_NETWORK:
            delete main;
            main = 0;
            lobby = new Api::Lobby();
            Api::Manager::add_elder(lobby);
            break;

        case SUBMENU_REPLAY:
            delete main;
            main = 0;
            replay = new Api::ReplayBrowser();
            Api::Manager::add_elder(replay);
            break;

        case SUBMENU_OPTIONS:
            delete main;
            main = 0;
            options = new Api::OptionMenu();
            Api::Manager::add_elder(options);
            break;

        case SUBMENU_EXIT:
            delete main;
            main = 0;
            exit_with = EXIT_PROGRAM;
            break;
        }
    }

    else if (single) {
        if (single->get_exit_with() != Api::SingleBrowser::EXIT_WITH_NOTHING) {
            switch (single->get_exit_with()) {
            case Api::SingleBrowser::EXIT_WITH_NOTHING:
                break;

            case Api::SingleBrowser::EXIT_WITH_OKAY:
                exit_with = START_LEVEL;
                break;

            case Api::SingleBrowser::EXIT_WITH_EDIT:
                exit_with = START_EDITOR;
                break;

            case Api::SingleBrowser::EXIT_WITH_EXIT:
                delete single;
                single = 0;
                main = new MainMenu();
                break;
            }
        }
    }

    else if (lobby) {
        Network::calc();
        if (lobby->get_exit_with() == Api::Lobby::EXIT_WITH_PLAY) {
            delete lobby;
            lobby = 0;
            exit_with = START_LEVEL;
        }
        else if (lobby->get_exit_with() == Api::Lobby::EXIT_WITH_EXIT) {
            delete lobby;
            lobby = 0;
            main = new MainMenu();
        }
    }

    else if (replay) {
        if (replay->get_exit_with() == Api::ReplayBrowser::EXIT_WITH_OKAY) {
            delete replay;
            replay = 0;
            exit_with = START_REPLAY;
            // Replay-Last-Dir und -File hat der Replaybrowser gesetzt
        }
        else if (replay->get_exit_with()==Api::ReplayBrowser::EXIT_WITH_EXIT) {
            delete replay;
            replay = 0;
            main = new MainMenu();
        }
    }

    else if (options) {
        if (options->get_exit()) {
            delete options;
            options = 0;
            main = new MainMenu();
        }
    }

    else if (new_player) {
        if (new_player->get_exit()) {
            delete new_player;
            new_player = 0;
            main = new MainMenu();
        }
    }

    else if (language) {
        language->calc();
        if (language->get_exit()) {
            Language::set(language->get_exit_with());
            delete language;
            language = 0;
            if (gloB->user_name_ask
             || gloB->user_name.empty()) {
                 new_player = new Api::NewPlayerMenu();
                 Api::Manager::add_elder(new_player);
            }
            else main = new MainMenu();
        }
    }



    // Kein Fenster offen?
    else {
        if (useR->language == Language::NONE
         || useR->language >= Language::MAX) {
            language   = new Api::LanguageMenu();
            Api::Manager::add_elder(language);
        }
        else if (gloB->user_name_ask
         ||      gloB->user_name.empty()) {
            new_player = new Api::NewPlayerMenu();
            Api::Manager::add_elder(new_player);
        }
        else main      = new MainMenu();
    }
}



void Menu::draw()
{
    if (!new_player) {
        mouse_cursor.set_x(hardware.get_mx()-mouse_cursor_offset);
        mouse_cursor.set_y(hardware.get_my()-mouse_cursor_offset);
        mouse_cursor.draw();
    }
    blit_to_screen(Api::Manager::get_torbit().get_al_bitmap());
}
