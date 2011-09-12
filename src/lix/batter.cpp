#include "ac.h"

#include <algorithm>

// the rectangle is given by specifying its center point, half its width, and
// half its height. returns whether any lixes were flung.
bool fling_all_in_rectangle(
    const UpdateArgs& ua,
    const int cx, const int cy, const int blockexc, // more range for blockers
    const int wx, const int wy, // 1/2 of the xl, yl of the rectangle
    const int vx, const int vy, const Lixxie& self
) {
    bool someone_was_hit = false;
    for (Tribe::It titr = ua.st.tribes.begin();
        titr != ua.st.tribes.end(); ++titr)
        for (Tribe::LixIt i = titr->lixvec.begin();
        i != titr->lixvec.end(); ++i)
    {
        if (i->get_leaving()) continue;
        // Fling others in this rectangle.
        const bool blo = (i->get_ac() == LixEn::BLOCKER);
        const int ch = (blo ? cx + blockexc * self.get_dir() : cx);
        const int dx = std::abs(Lixxie::get_land().distance_x(ch,i->get_ex()));
        const int dy = std::abs(Lixxie::get_land().distance_y(cy,i->get_ey()));
        if (dx <= (blo ? wx + blockexc : wx)
         && dy <= wy
         && &*i != &self) {
            someone_was_hit = true;
            i->become       (LixEn::TUMBLER);
            i->set_dir      (self.get_dir());
            i->set_special_x(vx);
            i->set_special_y(vy);
        }
    }
    return someone_was_hit;
}



void update_batter(Lixxie& l, const UpdateArgs& ua)
{
    if      (!l.is_solid())     l.become(LixEn::FALLER);
    else if (l.is_last_frame()) l.become(LixEn::WALKER);
    else                        l.next_frame();

    if (l.get_ac() == LixEn::BATTER && l.get_frame() == 3) {
        // the fun batting forward
        bool someone_was_hit = fling_all_in_rectangle(ua,
            l.get_ex() + 6 * l.get_dir(), l.get_ey() - 4, 4,
            12, 12,
            10, -12, l);
        if (someone_was_hit) l.play_sound        (ua, Sound::BATTER_HIT);
        else                 l.play_sound_if_trlo(ua, Sound::BATTER_MISS);
    }

}
