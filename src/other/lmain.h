/*
 * lmain.h
 *
 * This is the main object for the program. It calls the timing things
 * from other/help.h and then the undraw, calc and draw functions from
 * the game's main parts.
 *
 * It is a bit strange that I use these undraw routines, yes. Many of them
 * do nothing at all, and some do curious things that'll make "it all" work.
 *
 */

#pragma once

#include <string> // Level-Liste
#include <vector> // Level-Liste

#include "../editor/editor.h"
#include "../gameplay/gameplay.h"
#include "../menu/menu.h"

class LMain {

private:

    bool     exit;
    unsigned tck; // Ticks, are compared with Help::timer_ticks

    bool     gameplay_started_with_replay;

    // Modules
    Menu*     menu;
    Editor*   editor;
    Gameplay* gameplay;

    // Noncopyable
    LMain(const LMain&);
    void operator = (const LMain&);

    void undraw();
    void calc();
    void draw();

public:

    LMain();
    ~LMain();

    void main_loop();

};
