#include "ac.h"

void assclk_runner(Lixxie& l)
{
    l.set_runner();
    if (l.get_ac() == LixEn::WALKER) l.become(LixEn::RUNNER);
}



// this is defined in walker.cpp
void update_walker_or_runner(Lixxie& l, const UpdateArgs& ua);

void update_runner(Lixxie& l, const UpdateArgs& ua)
{
    if (l.is_last_frame())  l.set_frame(1);
    else                    l.next_frame();

    int dir = l.get_dir();

    // Do the same as we did for the walker. If nothing of interest has
    // happened, then update another time in the same manner.
    update_walker_or_runner(l, ua);
    if (l.get_ac() == LixEn::RUNNER && l.get_dir() == dir) {
        update_walker_or_runner(l, ua);
    }
}
