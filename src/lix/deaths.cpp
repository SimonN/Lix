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

    // Nicht explodieren lassen, das täte er bei 76 :-)
    // Auf jeden Fall ab Frame 6 keine Zuendschnur mehr zulassen
    // Da erst spaeter das Frame inkrementiert wird, > 3 testen.
    if (l.get_frame() > 4
     || l.get_updates_since_bomb() + 5 + 3*l.get_frame() > 74) {
        l.set_updates_since_bomb(0);
    }
    else if (l.get_updates_since_bomb() > 0) {
        l.set_updates_since_bomb(l.get_updates_since_bomb()
         + 5 + 3*l.get_frame());
    }
    if (l.is_last_frame()) l.become(LixEn::NOTHING);
    else l.next_frame();
}



void update_burner(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Nach unten bewegen, wenn kein Boden da ist und wir die Animation
    // noch nicht zu weit abgeschritten haben
    for (int i = 0; i < 5 - l.get_frame(); ++i)
     if (!l.is_solid()) l.move_down(1);

    // Zuendschnur ab Frame 6 weg scheint OK zu sein.
    update_splatter(l, ua);
}



void update_drowner(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Nicht explodieren lassen, das täte er bei 76 :-)
    // Wenn er schon fast untergegangen ist: Zündschnur entfernen
    if (l.get_updates_since_bomb() + l.get_frame() > 74
     || l.get_frame() > 9) {
        l.set_updates_since_bomb(0);
    }
    else if (l.get_updates_since_bomb() > 0) {
        l.set_updates_since_bomb(
         l.get_updates_since_bomb()+l.get_frame());
    }

    // Make it look more nicely
    // if (!l.is_solid(0, 6)) l.move_down(1);
    if (l.get_frame() < drowner_steep_frame && !l.is_solid(8, 0))
        l.move_ahead();

    // Nach Ablauf der Animation zerstören...
    if (l.is_last_frame()) l.become(LixEn::NOTHING);
    else l.next_frame();
}
