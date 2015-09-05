/*
 * lemupd.cpp
 *
 * Was tut ein Lixxie in einem Update? Dies ist allerdings nur der grosse
 * Rahmen, der das Zusammenspiel aller Lixxiee mit den interaktiven Objekten
 * regelt. Einzel-Landbearbeitung findet in den Funktionen statt, die ueber
 * Lixxie::update() aufgerufen werden.
 *
 */

#include <cmath> // Knockback-Berechnung

#include "../gameplay/gameplay.h"
#include "../graphic/sound.h"
#include "../other/globals.h"

void Gameplay::score_lix_for_tribe(
    Lixxie& l, const UpdateArgs& ua, Tribe& tribe
) {
    // Increase amount saved, or add to the number of saved-too-late lixes.
    if (cs.goals_locked) {
        ++tribe.lix_saved_late;
    }
    else {
        ++tribe.lix_saved;
        tribe.update_saved = cs.update;
        if (! multiplayer && l.get_tribe().lix_saved == level.required) {
            l.play_sound(ua, Sound::YIPPIE);
        }
    }
}




void Gameplay::update_lix(Lixxie& l, const UpdateArgs& ua)
{
    if (l.get_ac() == LixEn::NOTHING) return;

    l.set_no_encounters();
    // record encounters at the current position. This fixes the bug where
    // you could block in front of exits after stunners got up.
    l.record_encounters();

    // Exploder-Dinge separat!
    if (l.exploder_scheduled()) {

        // updates to trigger the explosion: 76, not 75.

        // When the lix is clicked in between two updates, 0 is set to 1.
        // The current function increments upon the update before any check,
        // so 0->1->2 happens during the same update.
        // Instant exploders in singleplayer are realized by setting
        // updates_since_bomb to 75 and have this function increment it to 76.

        // exploders die instantly now.
        if (l.about_to_explode(ua)) {
            // Exploder2 a la L2 mit Knockback
            if (l.get_exploder_knockback()) {
                // The value -6 (slightly above foot) is from L2 screen shots
                make_knockback_explosion(ua.st.update, l.get_tribe(),
                      ua.id, l.get_ex(), l.get_ey() - 6);
                l.play_sound(ua, Sound::POP);
                l.set_ac(LixEn::NOTHING);
                --l.get_tribe().lix_out;
                // Death, don't do anything else now
                return;
            }
        
            // Normaler Imploder
            else {
                l.set_updates_since_bomb(0);
                // Ascenders are already inside the wall. The climber should
                // also explode inside the wall, and therefore we move it
                // inside. Without an exploder anim, this isn't necessary for
                // the visuals. We still do it to preserve compatibility with
                // older replays.
                if (l.get_ac() == LixEn::CLIMBER)  l.move_ahead(2);
                // Always explode instantly -- this is done in the exploder's
                // update function, which we enter immediately here after
                // the top-level 'if'
                l.become(LixEn::EXPLODER);
            }
        }
        // Fuse still visible -- this can only be entered when
        // upd_for_bomb is higher than 2
        else {
            l.inc_updates_since_bomb();
            if (l.get_leaving()) {
                // Make it burn down faster if the lix is going to be dead
                // anyway without exploding
                l.set_updates_since_bomb(l.get_updates_since_bomb()
                                         + l.get_frame() + 1);
                if (l.get_updates_since_bomb() >= Lixxie::updates_for_bomb)
                    l.set_updates_since_bomb(0);
            }
        }
    }



    // Haupt-Abarbeitung
    // Lixes that already did NOTHING escaped the current function at the top.
    l.update(ua);
    if (l.get_ac() == LixEn::NOTHING) {
        --l.get_tribe().lix_out;
        return;
    }



    // Blocker
    if (l.get_ac() == LixEn::BLOCKER
     ||(l.get_ac() == LixEn::EXPLODER && l.get_special_x() == 1))
     update_lix_blocker(l);



    // Ziele
    // collide all goals with the lix l
    update_lix_goals(l, ua);
    // Remove exiters that have been marked as having gone through their anim:
    // special_x == 1000 or special_x == 3000 means exactly that
    if (l.get_ac() == LixEn::EXITER && l.get_special_x() == 1000) {
        --l.get_tribe().lix_exiting;
        l.set_ac(LixEn::NOTHING);
        return;
    }



    if (! l.get_leaving()) {
        // Feuer und Wasser
        if (l.get_body_encounters() & Lookup::bit_fire ) {
            l.become(LixEn::BURNER);
            l.play_sound(ua, Sound::FIRE);
        }
        else if (l.get_foot_encounters() & Lookup::bit_water) {
            l.become(LixEn::DROWNER);
            l.play_sound(ua, Sound::WATER);
        }
        // Fallen
        else if (l.get_foot_encounters() & Lookup::bit_trap) {
            for (TrigIt i = cs.trap.begin(); i != cs.trap.end(); ++i) {
                if (l.get_in_trigger_area(*i)
                 && i->get_open_for(l.get_tribe())) {
                    i->add_tribe(l.get_tribe());
                    i->allow_animation();
                    l.play_sound(ua, i->get_object()->sound);
                    l.set_ac(LixEn::NOTHING);
                    --l.get_tribe().lix_out;
                    return;
                }
            }
        }
    }



    // Flinging
    if ((l.get_body_encounters() & Lookup::bit_fling) && ! l.get_leaving())
     for (TrigIt i = cs.fling.begin(); i != cs.fling.end(); ++i)
     if (l.get_in_trigger_area(*i)) {
        // non-constant?
        if (i->get_open_for(l.get_tribe())) {
            i->add_tribe(l.get_tribe());
            i->allow_animation();
            int specx = i->get_object()->special_x;
            if (!(i->get_object()->subtype & 1)) specx *= l.get_dir();
            l.add_fling(specx, i->get_object()->special_y);
        }
    }



    // Trampoline
    if ((l.get_body_encounters() & Lookup::bit_trampoline)
     && ! l.get_leaving())
     for (TrigIt i = cs.trampoline.begin(); i != cs.trampoline.end(); ++i)
     if (l.get_in_trigger_area(*i)) {
        const int min_y_accel = -6;

        int dir = l.get_dir();
        if (l.get_ac() == LixEn::JUMPER || l.get_ac() == LixEn::TUMBLER) {
            if (l.get_special_y() > 0) { // moving downwards
                if (l.get_special_y() <= 12) {
                    int spy = l.get_special_y();
                    // fly back up to where we came from, but
                    // get at least a certain minimum y acceleration of -4
                    l.set_special_y(std::min(min_y_accel, -spy-1));
                } else {
                    int spy = l.get_special_y();
                    // original calculation:
                    // falldist = y(y+1)/2 - 12*13/2 + 2*(6*7)/2
                    // has been amended through experimentation to:
                    double falldist = std::floor(0.5*(spy-2)*(spy-1) - 37);
                    if (falldist < 0) falldist = 0;
                    l.set_special_y(
                        (int) std::floor(0.5 - 2*std::sqrt(1+falldist)));
                }
                if (std::abs(l.get_special_x()) < 4)
                    l.set_special_x(std::abs(4*dir));
                i->allow_animation();
                if (l.get_ac() == LixEn::JUMPER) l.set_frame(5);
            }
        } else if (l.get_ac() == LixEn::FALLER) {
            int falldist = l.get_special_x();
            l.become(LixEn::TUMBLER);
            // x speed once a lix falls onto trampoline
            int spx = 4 * dir;//i->get_object()->special_x;
            l.set_special_x(std::abs(spx));
            l.set_dir(spx);
            l.set_special_y(std::min(min_y_accel,
                            (int) std::floor(-0.5 - 2*std::sqrt(1+falldist))));
            i->allow_animation();
        } // what about floaters?
    }


    // Lixxie aus dem Bild?
    if (l.get_ey() >= map.get_yl() + 23
     ||(l.get_ey() >= map.get_yl() + 15 && l.get_ac() != LixEn::FLOATER)
     || l.get_ex() >= map.get_xl() +  4
     ||(l.get_ey() < -1 && !l.get_pass_top()) // -1 because ey is 2 over floor
     || l.get_ex() < -4) {
        l.play_sound(ua, Sound::OBLIVION);
        l.set_ac(LixEn::NOTHING);
        --l.get_tribe().lix_out;
        return;
    }

    // allow a blocker to turn this lix for the next update. 
    // as all blockers are updated in one batch (see Gameplay::update_cs_once),
    // doing it here has the same effect as doing it for everyone at the
    // beginning of each update
    l.clear_blockerturned();

}



void Gameplay::update_lix_blocker(Lixxie& l)
{
    for (Tribe::It t = cs.tribes.begin(); t != cs.tribes.end(); ++t)
     for (LixIt i = t->lixvec.begin(); i != t->lixvec.end(); ++i) {
        if (! i->get_leaving() && i->get_blockable()) {
            const int dx = map.distance_x(i->get_ex(), l.get_ex());
            const int dy = map.distance_y(i->get_ey(), l.get_ey());
            // check that the lix is within the blocker force field
            if (dx > - block_s && dx < block_s
             && dy > - block_d && dy < block_u) {
                if ((i->get_dir() > 0 && dx > 0)
                      || (i->get_dir() < 0 && dx < 0))
                {
                    // don't turn if already turned
                    if (!i->get_blockerturned(Lixxie::BLOCKER_TURNED)) {
                        i->turn();
                        i->set_blockerturned(Lixxie::BLOCKER_TURNED);
                    }
                    // Platformer drehten sonst um, hoeren auf, drehen erneut
                    if (i->get_ac() == LixEn::PLATFORMER) {
                        i->become(LixEn::SHRUGGER2);
                        i->set_frame(9);
                    }
                }
                // which direction does the forcefield push towards?
                char dir_bit = dx < 0 ? 
                      Lixxie::BLOCKER_LEFT : Lixxie::BLOCKER_RIGHT;
                // mark that the lix is in such a forcefield
                // regardless of whether it actually turned the lix
                i->set_blockerturned(dir_bit);
            }
        }
    }
}



void Gameplay::update_lix_goals(Lixxie& l, const UpdateArgs& ua)
{
    // false: Keine persoenlichen Einstellungen wie !multbuilders beachten
    if (l.get_foot_encounters() & Lookup::bit_goal
     && l.get_priority(LixEn::EXITER, false) > 1)
     for (int i = 0; i < (int) goal.size(); ++i)
     if (l.get_in_trigger_area(goal[i])) {
        // become() will alter lix_out and lix_exiting accordingly
        l.become(LixEn::EXITER);
        l.set_special_y(i);
        // Mark for sidewards motion if the goal was entered near the side
        // of the trigger area. The +1 is necessary because this counts
        // pixel-wise, but the physics skip ahead 2 pixels at a time,
        // so the lixes enter the right part further to the left.
        const Object& ob = *goal[i].get_object();
        l.set_special_x(map.distance_x(goal[i].get_x()
         + ob.get_trigger_x() + ob.trigger_xl / 2, l.get_ex()));
        if (l.get_special_x() % 2 == 0) l.set_special_x(l.get_special_x() + 1);

        // count the lixes for the proper players, the called function here
        // checks whether the goals are locked
        if (goal[l.get_special_y()].has_tribe(&l.get_tribe())) {
            score_lix_for_tribe(l, ua, l.get_tribe());
        }
        else
         for (Tribe::It atr = cs.tribes.begin(); atr != cs.tribes.end(); ++atr)
         if (goal[l.get_special_y()].has_tribe(&*atr)) {
            score_lix_for_tribe(l, ua, *atr);
        }
        // play the correct sound
        if (goal[i].has_tribe(trlo))
         effect.add_sound(ua.st.update, *trlo, ua.id, Sound::GOAL);
        else if (&l.get_tribe() == trlo)
         effect.add_sound(ua.st.update, *trlo, ua.id, Sound::GOAL_BAD);
    }
}



void Gameplay::make_knockback_explosion(
    const unsigned long upd,
    const Tribe& t,
    const unsigned lem_id,
    const int x,
    const int y)
{
    // Es gibt keine Lixxie-Funktion fuer den Kreis, nur fuer Rechtecke.
    // Also machen wir's ganz geschickt: Wir nehmen das kleinste Quadrat, was
    // den Kreis enthaelt, arbeiten es per Schleife ab und entfernen alle Pixel
    // mit genügend kleinem Abstand zum Mittelpunkt.
    const int radius = 23; // Diese Zahl stammt aus L2-Screenshots
    for  (int lx = -radius; lx  <=  radius; ++lx) // lx = Loop-X
     for (int ly = -radius; ly  <=  radius; ++ly)
     if  (map.hypotsquare(lx, ly, 0, 0) <= (radius + 0.5) * (radius + 0.5))
     Lixxie::remove_pixel_absolute(x + lx, y + ly + 1); // +1, da Radius%2 = 1

    // Knockback
    // Fuer komische Werte siehe alle Kommentare zum lustigen Hochfliegen.
    const double range      = radius * 2.5 + 0.5;
    for (Tribe::It titr = cs.tribes.begin(); titr != cs.tribes.end(); ++titr)
     for (LixIt i = titr->lixvec.begin(); i != titr->lixvec.end(); ++i) {
        // Ausnahme:
        if (i->get_leaving()) continue;
        // Mehr lustiges Hochfliegen durch die 10 tiefere Explosion!
        const int dx = map.distance_x(x,      i->get_ex());
        const int dy = map.distance_y(y + 10, i->get_ey());
        const double distancesquare = map.hypotsquare(dx, dy, 0, 0);
        if (distancesquare <= range * range) {
            const double dist = std::sqrt(distancesquare);
            int sx = 0;
            int sy = 0;
            if (dist > 0) {
                const double strength_x   = 350;
                const double strength_y   = 330;
                const int    center_const =  20;
                sx = (int) (strength_x * dx / (dist * (dist + center_const)) );
                sy = (int) (strength_y * dy / (dist * (dist + center_const)) );
            }
            const bool same_tribe = (&t == &*titr);
            // the upcoming -5 are for even more jolly flying upwards!
            // don't splat too easily from flinging, degrade this bonus softly
            if      (sy > -10) sy += -5;
            else if (sy > -20) sy += (-20 - sy) / 2;
            i->add_fling(sx, sy, same_tribe);
        }
    }
    effect.add_explosion(upd, t, lem_id, x, y);
}



// defined in tumbler.cpp
void tumbler_frame_selection(Lixxie& l);

void Gameplay::finally_fling(Lixxie& l)
{
    if (! l.get_leaving()) {
        l.become(LixEn::TUMBLER);
        if (l.get_fling_x() != 0) l.set_dir(l.get_fling_x());
        l.set_special_x(std::abs(l.get_fling_x()));
        l.set_special_y(l.get_fling_y());
        l.reset_fling_new();
        tumbler_frame_selection(l);
    }
}
