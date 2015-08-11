/*
 * b_spawn.h
 *
 * A pressable button for changing the spawn interval. This is not the
 * large button that only displays the unchangable spawn interval.
 *
 * A TwoTasksButton is an intermediate form: It supports the left/right-click
 * behavior of the SpawnIntervalButton, and has two hotkeys for these two
 * modes, but does not have the spawn interval printed on it.
 *
 * warm/hot have different meanings from Button::. Right now, both mean:
 * Don't execute continuously on LMB hold after a while.
 *
 */

#pragma once

#include "b_bitmap.h"
#include "../label.h"

namespace Api {
class TwoTasksButton : public BitmapButton {

public:

    //                                     x    y    xl   yl
             TwoTasksButton(const Cutbit&, int, int, int, int);
    virtual ~TwoTasksButton();

    // Executing spawn interval buttons works differently from executing
    // normal buttons. Normal buttons execute on mouse release, this button
    // executes on a mouse downpress, and can execute repeatedly when the
    // mouse button is held down. What a joy:
    bool get_execute_left()  { return execute_left;  }
    bool get_execute_right() { return execute_right; }

    int  get_hotkey_right()      { return hotkey_right; }
    void set_hotkey_right(int i) { hotkey_right = i;    }

    // only a shortcut for some functions that don't care what exactly happens
    virtual inline bool get_clicked() { return execute_left || execute_right; }

protected:

    virtual void check_whether_to_appear_down();
    virtual void calc_self();

private:

    int hotkey_right;
    int ticks_lmb_is_held_for; // to have left mouse button (LMB) behave like
                               // a keypress that's repeated after a while

    bool execute_left;
    bool execute_right;

};



class SpawnIntervalButton : public TwoTasksButton {

public:

             SpawnIntervalButton(int x = 0, int y = 0);
    virtual ~SpawnIntervalButton();

    inline int  get_spawnint() const { return spawnint; }
           void set_spawnint(int i);

private:

    int   spawnint;
    Label label;

};

}
// end namespace Api
