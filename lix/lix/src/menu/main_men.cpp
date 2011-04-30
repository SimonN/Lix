/*
 * menu/main_men.cpp
 *
 */

#include "main_men.h"

#include "../api/manager.h"
#include "../other/help.h"
#include "../other/language.h"



MainMenu::MainMenu()
:
    window(new Api::MainMenuWindow())
{
    Api::Manager::add_elder(window);
}

MainMenu::~MainMenu()
{
    Api::Manager::remove_elder(window);
    delete window;
}



// ############################################################################
// ############################################################################
// ############################################################################



namespace Api {

const unsigned MainMenuWindow::button_xl  = 200;
const unsigned MainMenuWindow::button_yl  =  40;
const unsigned MainMenuWindow::button_y_spacing =  20;

const unsigned MainMenuWindow::window_xl  = button_xl
                                                  + 2 * button_y_spacing;
const unsigned MainMenuWindow::window_yl  =   4 * button_yl
                                                  + 6 * button_y_spacing
                                                  + 60; // Fuer Infotexte

MainMenuWindow::MainMenuWindow()
:
    Window(LEMSCR_X/2 - window_xl/2, LEMSCR_Y/2 - window_yl/2,
     window_xl, window_yl, Language::main_name_of_the_game),

    exit_with(SUBMENU_NOTHING),

    single (20, 40 + 0 * (button_yl + button_y_spacing),
     button_xl, button_yl, Language::browser_single_title),
    network(20, 40 + 1 * (button_yl + button_y_spacing),
     button_xl, button_yl, Language::win_lobby_title),
    replay (20, 40 + 2 * (button_yl + button_y_spacing),
     button_xl/2 - 10, button_yl, Language::browser_replay_title),
    options(130, 40 + 2 * (button_yl + button_y_spacing),
     button_xl/2 - 10, button_yl, Language::option_title),
    exit   (20, 40 + 3 * (button_yl + button_y_spacing),
     button_xl, button_yl, Language::exit),
    version(window_xl / 2, window_yl - 60, "", Label::CENTERED),
    website(window_xl / 2, window_yl - 40, "", Label::CENTERED)
{
    add_child(single);
    add_child(network);
    add_child(replay);
    add_child(options);
    add_child(exit);
    add_child(version);
    add_child(website);

    exit.set_hotkey(KEY_ESC);
    const std::string  v = Language::main_version + " "
                         + Help::version_to_string(gloB->version);
    version.set_text(v);
    website.set_text(Language::main_website);
}

MainMenuWindow::~MainMenuWindow()
{
}



void MainMenuWindow::calc_self()
{
    if      (single .get_clicked()) exit_with = SUBMENU_SINGLE;
    else if (network.get_clicked()) exit_with = SUBMENU_NETWORK;
    else if (replay .get_clicked()) exit_with = SUBMENU_REPLAY;
    else if (options.get_clicked()) exit_with = SUBMENU_OPTIONS;
    else if (exit   .get_clicked()) exit_with = SUBMENU_EXIT;
}



void MainMenuWindow::draw_self()
{
    get_ground().clear_to_color(::color[COL_BLACK]);
    Window::draw_self();
}

} // Ende Namensraum Api
