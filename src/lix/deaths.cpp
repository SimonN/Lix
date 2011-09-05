/*
 * lix/deaths.cpp
 *
 * Enthaelt Splatter, Drowner, Burner und derlgeichen.
 *
 * Am Ende der Animationen wird diesen Lixen die Aktivitaet NOTHING
 * zugewiesen. Die Gameplay-Klasse erkennt das.
 *
 */

#include "ac.h"

const int tumbler_frame_steep_drown = 7;
const int drowner_steep_frame = 6;



void become_drowner(Lixxie& l)
{
    int drowner_frame = 0;
    if (l.get_ac() == LixEn::TUMBLER
        && l.get_frame() > tumbler_frame_steep_drown) {
        drowner_frame = drowner_steep_frame;
    }
    l.become_default(LixEn::DROWNER);
    l.set_frame(drowner_frame);
}



void update_splatter(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    if (l.is_last_frame()) l.become(LixEn::NOTHING);
    else l.next_frame();
}



void update_burner(Lixxie& l, const UpdateArgs& ua)
{
    // Nach unten bewegen, wenn kein Boden da ist und wir die Animation
    // noch nicht zu weit abgeschritten haben
    for (int i = 0; i < 5 - l.get_frame(); ++i)
     if (!l.is_solid()) l.move_down(1);

    update_splatter(l, ua);
}



void update_drowner(Lixxie& l, const UpdateArgs& ua)
{
    // Make it look more nicely
    // if (!l.is_solid(0, 6)) l.move_down(1);
    if (l.get_frame() < drowner_steep_frame && !l.is_solid(8, 0))
        l.move_ahead();

    update_splatter(l, ua);
}
