/*
 * menu.h
 *
 * Das Menü des Spiels
 *
 * Ein Menüobjekt wird von LMain erschaffen, wenn das Programm startet oder
 * ein Level beendet wird. Das Menü lädt alle Levels eines Verzeichnisses und
 * zeigt sie an, ebenso alle erreichbaren Unterverzeichnisse. Dieser Ladevorgang
 * geschieht einmal bei Menüerschaffung und zudem bei jedem Verzeichniswechsel.
 *
 * Zum Kommunizieren mit anderen Ebenen werden unter anderem die globalen
 * Variablen fuer den aktuellen Einzelspielerlevel sowie fuer den Netzwerklevel
 * verwendet.
 *
 * Dem Konstruktor dieses Objektes kann der zuletzt verwendete Pfad mitgegeben
 * werden, damit der Explorer direkt hier startet.
 * Zweites Argument: Die Datei (mit vollst. Pfad) fuer sofortiges Highlight.
 *
 * Das Laden des Hauptmenues bewirkt per dessen Konstruktor ein Ueberzeichnen
 * des OSDs, damit kein magisches Pink verbleibt.
 *
 */

#pragma once

#include "../other/myalleg.h"
#include <string>
#include <vector> // Level-Liste wird im Konstruktor übergeben

#include "browsers.h"
#include "lobby.h"
#include "main_men.h"
#include "options.h"
#include "little.h"

#include "../graphic/glob_gfx.h" // Löschen des OSDs im Konstruktor und Destruktor
#include "../graphic/graph_bg.h" // Mauscursor
#include "../level/level.h"

// Da gegenseitig eingebunden, Deklaration
class MainMenu;

class Menu {

public:

    enum ExitWith {
        NOTHING,
        START_LEVEL,
        START_REPLAY,
        START_EDITOR,
        EXIT_PROGRAM
    };

private:

    ExitWith  exit_with;

    MainMenu* main;

    Api::SingleBrowser* single;
    Api::Lobby*         lobby;
    Api::ReplayBrowser* replay;
    Api::OptionMenu*    options;
    Api::NewPlayerMenu* new_player;
    Api::LanguageMenu*  language;

    const int mouse_cursor_offset;
    GraphicBG mouse_cursor;

    // Kopierverbot
    Menu(const Menu&);
    void operator = (const Menu&);

public:

    // Dem Konstruktor kann das ExitWith des letzten Menuebeendens mitgegeben
    // werden. Dann kann das korrekte Untermenue sofort geoeffnet werden.
    Menu(SubMenu = SUBMENU_NOTHING);
    ~Menu();

    void undraw();
    void calc();
    void draw();

    inline ExitWith     get_exit_with()   { return exit_with;      }

};
