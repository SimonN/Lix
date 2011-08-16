#include "ac.h"

void update_batter(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if      (!l.is_solid())     l.become(LixEn::FALLER);
    else if (l.is_last_frame()) l.become(LixEn::WALKER);
    else                        l.next_frame();

    if (l.get_ac() == LixEn::BATTER) switch (l.get_frame()) {
    case 0:
        // transistion from frame 0 to 1
        l.play_sound(ua, Sound::BATTER_MISS);
        break;
    case 2:
        l.play_sound(ua, Sound::BATTER_HIT);
        break;
    default:
        ;
    }

}
