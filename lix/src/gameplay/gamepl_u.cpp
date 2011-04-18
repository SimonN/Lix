/*
 * gameplay/gamepl_u.cpp
 *
 * Ein Update, also eine spielmechanische Zeiteinheit, in der sich
 * beispielsweise die Lemminge bewegen
 *
 */

#include "gameplay.h"

#include "../graphic/sound.h"
#include "../other/user.h"

void Gameplay::update()
{
    // Noch schnell die Replaydaten mit der eingestellten Rate fertig machen:
    // Siehe Ratenbutton-Calculate fuer Kommentar, warum dies hier passiert.
    if (!replaying && pan.rate.get_number() != trlo->rate) {
        trlo->rate        = pan.rate.get_number();
        Replay::Data data = new_replay_data();
        data.action       = Replay::RATE;
        data.what         = trlo->rate;
        replay.add(data);
        Network::send_replay_data(data);
    }

    // Im Netzwerkspiel den Netzwerk-Replaydaten-Puffer nach neuen Ereignissen
    // der anderen Spieler kontrollieren - die eigenen sind schon im Replay.
    if (Network::get_started()) {
        Replay::Vec netdata = Network::get_replay_data();
        replay.add(netdata);
        replay_recalc_from = cs.update;
        for (Replay::ConstIt i = netdata.begin(); i != netdata.end(); ++i) {
            // Das folgende <= statt <  behebt den lange quaelenden Netzwerkbug
            // und ist sogar theoretisch korrekt: Wenn wir in Update N sind und
            // es kommt Data zu Upd. N, so muss N-1 -> N neu berechnet werden.
            if (i->update <= replay_recalc_from) {
                // recalc_from ist inklusive: Es wird ein Stand geladen,
                // der dieses Update noch nicht mitberechnet hat.
                replay_recalc_need = true;
                replay_recalc_from = i->update;
            }
        }
        Network::clear_replay_data();
    }

    // Updaten, entweder mehrmals oder nur einmal
    if (replay_recalc_need) {
        State state = state_manager.load_auto(replay_recalc_from);
        if (state) {
            // Wir nutzen nicht load_state(), weil dies kein manuelles Laden
            // ist. Dies sollte das einizge nicht-manuelle Laden sein.
            unsigned long updates_to_calc_to = cs.update;
            cs = state;
            while (cs.update < updates_to_calc_to) update_cs_once();
        }
        replay_recalc_need = false;
        replay_recalc_from = 0;
    }
    else update_cs_once();





    // Diese Dinge muessen nicht mehrfach gemacht werden, selbst wenn neu
    // geladen wird, weil Netzwerkpakete eintreffen.
    for (IacIt i =  special[Object::DECO  ].begin();
               i != special[Object::DECO  ].end(); ++i) i->animate();
    for (IacIt i =  special[Object::WATER ].begin();
               i != special[Object::WATER ].end(); ++i) i->animate();
    for (IacIt i =  special[Object::ONEWAY].begin();
               i != special[Object::ONEWAY].end(); ++i) i->animate();
    for (IacIt i =  cs.fling.begin(); i != cs.fling.end(); ++i) {
        if (!(i->get_object()->subtype & 2)) i->animate(); // it's a bitfield
    }
    for (Goal::It i = goal.begin();    i != goal.end();    ++i) i->animate();
    for (HatchIt  i = hatches.begin(); i != hatches.end(); ++i)
     i->animate(effect, cs.update);

    // Gegen negative Effekte von Player::return_skills:
    // Alle Faehigkeitszahlen einmal neu schreiben.
    if (trlo) {
        pan.set_skill_numbers(*trlo);
        if (malo) pan.set_skill_on(malo->skill_sel);
    }
}
// Ende des cs.update inkl. Neuladerei und Nachberechnung



// ############################################################################
// ############################################################# update_cs_once
// ############################################################################



// upd ist, welches Update berechnet wird. Das Spiel beginnt etwa mit Update 0
// und im ersten zu errechnenden Update setzt diese Funktion das auf 1.
void Gameplay::update_cs_once()
{
    // Neues Update einleiten
    ++cs.update;
    const Ulng& upd = cs.update;
    timer_tick_last_update = Help::timer_ticks;

    Replay::Vec data = replay.get_data_for_update(upd);

    // Erst den ersten Spieler, dann den zweiten usw. pro Update,
    // damit aequivalente Replays mit verschiedener Spielerreihenfolge pro
    // Update immer gleich verlaufen.
    for (Tribe::It tritr = cs.tribes.begin(); tritr!=cs.tribes.end();++tritr) {
        for (Replay::It i = data.begin(); i != data.end(); ++i) {
            std::list <Tribe::Master>::iterator mitr = tritr->masters.begin();
            while (mitr != tritr->masters.end()
             && mitr->number != i->player) ++mitr;
            if (mitr != tritr->masters.end())
             update_cs_one_data(*tritr, &*mitr, i);
        }
    }



    // Kurzer Einschub: Uhrendinge.
    if (level.seconds > 0) {
        if (cs.clock_running && cs.clock > 0) --cs.clock;
        // Nuke durch die letzten Sekunden der Uhr. Dies loest
        // kein Netzwerk-Paket aus! Alle Spieler werden jeweils lokal genukt.
        // Dies fuehrt dennoch bei allen zum gleichen Spielverlauf, da jeder
        // Spieler das Zeitsetzungs-Paket zum gleichen Update erhalten.
        // Wir muessen dies nach dem Replayauswerten machen, um festzustellen,
        // dass noch kein Nuke-Ereignis im Replay ist.
        if (cs.clock_running &&
         cs.clock <= (unsigned long) Lixxie::updates_for_bomb)
         for (Tribe::It tr = cs.tribes.begin(); tr != cs.tribes.end(); ++tr) {
            if (!tr->nuke) {
                // Paket anfertigen
                Replay::Data  data;
                data.update = upd;
                data.player = tr->masters.begin()->number;
                data.action = Replay::NUKE;
                replay.add(data);
                // Und sofort ausfuehren: Replay wurde ja schon ausgewertet
                tr->lix_hatch = 0;
                tr->nuke           = true;
                if (&*tr == trlo) {
                    pan.nuke_single.set_on();
                    pan.nuke_multi .set_on();
                }
                effect.add_sound(upd, *tr, 0, Sound::OHNO);
            }
        }
        // Ebenfalls etwas Uhriges: Gibt es Spieler mit geretteten Lemmingen,
        // die aber keine Lemminge mehr im Spiel haben oder haben werden? Dann
        // wird die Nachspielzeit angesetzt. Falls aber alle Spieler schon
        // genukt sind, dann setzen wir die Zeit nicht an, weil sie vermutlich
        // gerade schon ausgelaufen ist.
        if (!cs.clock_running)
         for (Tribe::CIt i = cs.tribes.begin(); i != cs.tribes.end(); ++i)
         if (i->lix_saved > 0
          && i->get_lix_out() + i->lix_hatch == 0) {
            // Suche nach Ungenuktem
            for (Tribe::CIt j = cs.tribes.begin(); j != cs.tribes.end(); ++j)
             if (!j->nuke && j->get_lix_out() + j->lix_hatch > 0) {
                cs.clock_running = true;
                // Damit die Meldung nicht mehrmals kommt bei hoher Netzlast
                effect.add_overtime(upd, *i, cs.clock);
                break;
            }
            break;
        }
        // Warnsounds
        if (cs.clock_running
         && cs.clock >  0
         && cs.clock != (unsigned long) level.seconds
                                      * gloB->updates_per_second
         && cs.clock <= (unsigned long) gloB->updates_per_second * 15
         && cs.clock % gloB->updates_per_second == 0)
         for (Tribe::CIt i = cs.tribes.begin(); i != cs.tribes.end(); ++i)
         if (!i->lixvec.empty()) {
            // The 0 goes where usually a lixvec ID would go, because this
            // is one of the very few sounds that isn't attached to a lixvec.
            effect.add_sound(upd, *trlo, 0, Sound::CLOCK);
            break;
        }
    }



    // Main iteration over players:
    // Create lix, update lix, nuke
    // No evaluation of replay/network data, that has already happened
    for (Tribe::It t = cs.tribes.begin(); t != cs.tribes.end(); ++t)
    {
        const int position = replay.get_permu()[t - cs.tribes.begin()];
        // Create new Lixxie if necessary
        if (t->lix_hatch != 0 && upd >= 60 &&
         upd >= t->update_hatch + 4+(99-t->rate)/2) {
            t->update_hatch = upd;
            const EdGraphic& h = hatches[t->hatch_next];
            Lixxie& newlix = t->lixvec[level.initial - t->lix_hatch];
            newlix = Lixxie(&*t,
             (h.get_x() + h.get_object()->trigger_x) / 2 * 2,
              h.get_y() + h.get_object()->trigger_y);
            --t->lix_hatch;
            ++t->lix_out;

            // Lemmings start walking to the left instead of right?
            if (h.get_rotation()) newlix.turn();
            // This extra turning solution here is necessary to make
            // some L1 and ONML two-player levels better playable.
            if (hatches.size() < cs.tribes.size()
             && (position / hatches.size()) % 2 == 1)
             newlix.turn();

            // It's the next hatches turn
            t->hatch_next += cs.tribes.size();
            t->hatch_next %= hatches.size();
        }



        // Lemmings updaten
        // Reaktionen auf Aiming wurden bereits beim Auswerten der Replaydaten
        // umgesetzt. Fuer diese Lemminge wird also zweimal die Update-Funktion
        // aufgerufen, einmal dort und einmal hier. Die dortige Funktion
        // ruft allerdings nur Lemming::update() auf, nicht
        // Gameplay::update_lixvec(). Es werden also Sachen wie Hochzaehlen
        // des Bomben-Countdowns oder das Laufen ins Ziel nicht ueberprueft.
        UpdateArgs ua;
        ua.upd   = upd;
        ua.aim_x = map.get_mouse_x(); // Alle schauen zum Cursor, macht nix,
        ua.aim_y = map.get_mouse_y(); // weil das nur Augenzucker sein darf.
        ua.aim_c = false;

        // Zerstören, wenn die Update-Funktion "true" liefert.
        // Erster Durchlauf: Nur die Arbeitstiere bearbeiten und markieren!
        for (LixIt i = --t->lixvec.end(); i != --t->lixvec.begin(); --i) {
            if (i->get_ac() > LixEn::WALKER) {
                ua.id = i - t->lixvec.begin();
                i->mark();
                update_lix(*i, ua);
            }
            // Sonst eine vorhandene Markierung ggf. entfernen
            else i->unmark();
        }
        // Zweiter Durchlauf: Unmarkierte bearbeiten
        for (LixIt i = --t->lixvec.end(); i != --t->lixvec.begin(); --i) {
            if (!i->get_mark()) {
                ua.id = i - t->lixvec.begin();
                update_lix(*i, ua);
            }
        }
        // Dritter Durchlauf: Nuke
        // Abbruch, wenn in diesem Update ein Lemming bearbeitet wird
        // Lemmings mit verlassenden Aktivitäten werden nicht mitgenommen!
        if (t->nuke == true)
         for (LixIt i = t->lixvec.begin(); i != t->lixvec.end(); ++i) {
            if (i->get_updates_since_bomb() == 0 && i->get_nukable()) {
                i->inc_updates_since_bomb();
                // Which exploder shall be assigned?
                if (cs.tribes.size() > 1) i->set_exploder_knockback();
                else for (int j = 0; j < gloB->skill_max; ++j)
                 if (t->skill[j].ac == LixEn::EXPLODER2) {
                    i->set_exploder_knockback();
                    break;
                }
                break;
            }
        }
        // Ende Nuke
    }
    // Ende Haupt-Lemmingupdate-Geschichten
    // Dies ist aber ebenfalls sehr wichtig fuer jedes Update, egal ob
    // normal oder nachberechnet:
    for (IacIt i = cs.trap.begin(); i != cs.trap.end(); ++i) {
        if (i->get_y_frame() == 1  // Dies benachrichtigt die Falle
         || i->get_x_frame() >  0) {
            i->set_y_frame(0);
            i->set_x_frame(i->get_x_frame() + 1);
            if (i->get_x_frame() >= i->get_x_frames()) i->set_x_frame(0);
        }
    }
    // for flinging objects
    for (IacIt i = cs.fling.begin(); i != cs.fling.end(); ++i) {
        if ((i->get_object()->subtype & 2) // non-constant flingers
         && (i->get_y_frame() == 1
         || i->get_x_frame() >  0)) {
            i->set_y_frame(0);
            i->set_x_frame(i->get_x_frame() + 1);
            if (i->get_x_frame() >= i->get_x_frames()) i->set_x_frame(0);
        }
    }

    state_manager.calc_save_auto(cs);
}
// Ende update_once



// ############################################################################
// ######################################################### update_cs_one_data
// ############################################################################



void Gameplay::update_cs_one_data(Tribe& t, Tribe::Master* m, Replay::It i)
{
    const Ulng& upd = cs.update;

    if (i->action == Replay::RATE) {
        t.rate = i->what;
        if (&t == trlo) {
            pan.rate.set_number(t.rate);
        }
    }
    else if (i->action == Replay::SKILL) {
        if (!m) return;
        const int& nr = m->skill_sel = i->what;
        if (m == malo) {
            pan.set_skill_on(nr);
            effect.add_sound(upd, *trlo, nr, Sound::PANEL);
        }
    }
    else if (i->action == Replay::NUKE) {
        if (!t.nuke) {
            t.lix_hatch = 0;
            t.nuke           = true;
            if (&t == trlo) {
                pan.nuke_single.set_on();
                pan.nuke_multi .set_on();
            }
            effect.add_sound(upd, t, 0, Sound::OHNO);
        }
    }
    else if (i->action == Replay::ASSIGN) {
        if (!m) return;
        Tribe::Skill& psk = t.skill[m->skill_sel];
        if (i->what < t.lixvec.size()) {
            Lixxie& lix = t.lixvec[i->what];
            // false: Do not respect the user's options like
            // disabling the multiple builder feature
            if (lix.get_priority(psk.ac, cs.tribes.size(), goal, false) > 1
             && psk.nr != 0) {
                ++(t.skills_used);
                if (psk.nr != LixEn::infinity) --psk.nr;
                lix.evaluate_click(psk.ac);
                // Draw arrow if necessary, read arrow.h/effect.h for info
                if (useR->arrows_replay  && replaying
                 || useR->arrows_network && Network::get_started()
                                                 && m != malo) {
                    Arrow arr(map, t.style, lix.get_ex(), lix.get_ey(),
                     psk.ac, upd, i->what);
                    effect.add_arrow(upd, t, i->what, arr);
                }
                if (m == malo)
                 effect.add_sound      (upd, t, i->what, Sound::ASSIGN);
                else if (&t == trlo)
                 effect.add_sound_quiet(upd, t, i->what, Sound::ASSIGN);
            }
        }
        if (m && &t == trlo) {
            // Wird ohne Replay naemlich extra schon vorher gemacht,
            // damit es schoener aussieht bei langsamen Spieltempi.
            pan.skill[m->skill_sel].set_number(psk.nr);
        }
    }
    else if (i->action == Replay::AIM) {
        // Ob weiterhin gezielt werden muss, wird fuer die kommenden Ticks
        // am Ende von Gameplay::update() geprueft.
        size_t lem_id = (int) i->what % level.initial;
        if (lem_id < t.lixvec.size()) {
            Lixxie& lix = t.lixvec[lem_id];
            UpdateArgs ua;
            ua.upd   = upd;
            ua.id    = lem_id;
            ua.aim_x = (i->what / level.initial) % map.get_xl();
            ua.aim_y = i->what / (map.get_xl() * level.initial);
            ua.aim_c = true;
            lix.update(ua);
            effect.add_sound(upd, t, lem_id, lix.get_aim_sound());
        }
    }
    // end of AIM
}
// end of update_cs_one_data
