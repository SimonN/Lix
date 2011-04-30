/*
 * /menu/main_men.h
 *
 * Hauptmenue
 *
 * Der Konstruktor dieses Menues stellt einige tolle Dinge mit dem OSD an,
 * weil das Fenster nicht komplett den Bildschirm ausfuellt. Daher braucht
 * die Klasse Menu nichts mehr dergleichen zu tun!
 *
 * Ein Objekt dieser Klasse ist eine Instanz von MainMenuWindow.
 *
 */

#pragma once

#include "../api/button/b_text.h"
#include "../api/label.h"
#include "../api/window.h"

enum SubMenu {
    SUBMENU_NOTHING,
    SUBMENU_SINGLE,
    SUBMENU_NETWORK,
    SUBMENU_REPLAY,
    SUBMENU_OPTIONS,
    SUBMENU_EXIT
};

// Dies ist nur das Fenster,
// die "richtige" Klasse kommt unten.
namespace Api {
class MainMenuWindow : public Window {

private:

    static const unsigned button_xl;
    static const unsigned button_yl;
    static const unsigned button_y_spacing;

    static const unsigned window_xl;
    static const unsigned window_yl;

    SubMenu exit_with;

    TextButton single;
    TextButton network;
    TextButton replay;
    TextButton options;
    TextButton exit;

    Label      version;
    Label      website;

    // Kopieren verboten
    MainMenuWindow(const MainMenuWindow&);
    void operator = (const MainMenuWindow&);

public:

             MainMenuWindow();
    virtual ~MainMenuWindow();

    inline SubMenu get_exit_with() { return exit_with; }

protected:

    virtual void calc_self();
    virtual void draw_self();

};
}



// ############################################################################
// ############################################################################
// ############################################################################



// Die Hauptklasse, die auch das OSD kontrolliert, die Effekte, etc.
namespace Api { class MainMenuWindow; }
class MainMenu {

private:

    Api::MainMenuWindow* window;

    MainMenu(const MainMenu&);
    void operator = (const MainMenu&);

public:

    MainMenu();
    ~MainMenu();

    inline SubMenu get_exit_with() { return window->get_exit_with(); }

};

