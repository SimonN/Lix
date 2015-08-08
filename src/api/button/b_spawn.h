/*
 * b_spawn.h
 *
 * A pressable button for changing the spawn interval. This is not the
 * large button that only displays the unchangable spawn interval.
 *
 */

#pragma once

#include "b_bitmap.h"
#include "../label.h"

namespace Api {
class SpawnIntervalButton : public BitmapButton {

public:

             SpawnIntervalButton(int x = 0, int y = 0);
    virtual ~SpawnIntervalButton();

    inline int  get_spawnint() const { return spawnint; }
           void set_spawnint(int i);

    // Executing spawn interval buttons works differently from executing
    // normal buttons. Normal buttons execute on mouse release, this button
    // executes on a mouse downpress, and can execute repeatedly when the
    // mouse button is held down. What a joy:
    bool get_execute_left()  { return execute_left;  }
    bool get_execute_right() { return execute_right; }

    // only a shortcut for some functions that don't care what exactly happens
    virtual inline bool get_clicked() { return execute_left || execute_right; }

protected:

    virtual void check_whether_to_appear_down();
    virtual void calc_self();

private:

    int   spawnint;
    Label label;

    bool execute_left;
    bool execute_right;

    int  ticks_lmb_is_held_for; // to have left mouse button (LMB) behave like
                                // a keypress that's repeated after a while

};
}
