/*
 * gameplay/gamepl_a.cpp
 *
 * Dinge, die bei einem aktiven Tick des Gameplays kontrolliert werden.
 * "Aktiv" heisst hierbei, dass kein Replay abgespielt wird, sondern spiel-
 * mechanisch relevante Dinge der Steuerung bearbeitet werden.
 *
 */

#include "gameplay.h"
#include "../other/user.h"

Replay::Data Gameplay::new_replay_data()
{
    Replay::Data data;
    data.player = malo->number;
    data.update = cs.update + 1;
    return data;
}

void Gameplay::calc_active()
{
    if (!malo) return;

    // Wo ist die Maus auf dem Land?
    int mx = map.get_mouse_x();
    int my = map.get_mouse_y();

    const bool mouse_on_panel = hardware.get_my() > map.get_screen_yl();

    // mouse on the playing field, lixes are selectable
    if (!mouse_on_panel && malo->aiming != 2 && trlo) {
        // Bestimmte Richtung anwählen?
        bool only_dir_l = false;
        bool only_dir_r = false;
        if (  key[useR->key_force_left]
         && ! key[useR->key_force_right]) {
            only_dir_l = true;
            mouse_cursor.set_x_frame(1);
        }
        else if (! key[useR->key_force_left]
         &&        key[useR->key_force_right]) {
            only_dir_r = true;
            mouse_cursor.set_x_frame(2);
        }
        // Decide which lix the cursor points at
        // Die Liste der Lixen wird durchlaufen und die Priorität jeder
        // Lix errechnet. Wird eine höhere Priorität als die derzeitig
        // höchste gefunden, wechselt LixIt target. Bei gleicher Prioritaet
        // haben Lixen, die naeher am Mauscursor liegen, Vorrang! Mit rechter
        // Maustaste (selectable in the options) waehlt man dagegen die letzte
        // Lix mit der niedrigsten Priorität. Auch hier haben naeher liegende
        // Lixen Vorrang.
        LixIt  target = trlo->lixvec.end(); // Klickbar mit Prioritaet
        LixIt  tarinf = trlo->lixvec.end(); // Nicht unb. klickbar mit Prior.
        int    tarcnt = 0; // Anzahl Lixen unter Cursor
        int    target_priority = 0;
        int    tarinf_priority = 0;
        double target_hypot = 1000;
        double tarinf_hypot = 1000;

        // Bei Zoom diese Variablen veraendern
        int zoom   = map.get_zoom();
        int mmld_x = mouse_max_lix_distance_x - mouse_cursor_offset/2*zoom;
        int mmld_u = mouse_max_lix_distance_u - mouse_cursor_offset/2*zoom;
        int mmld_d = mouse_max_lix_distance_d - mouse_cursor_offset/2*zoom;

        // trlo->skill_sel wird erst beim naechsten Update gesetzt.
        // Also suchen wir manuell, welche Faehigkeit der Spieler durch
        // Anklicken gewaehlt hat.
        int skill_visible;
        for (skill_visible = 0; skill_visible < gloB->skill_max
         && ( !pan.skill[skill_visible].get_on()
          || trlo->skill[skill_visible].nr == 0
          ||   pan.skill[skill_visible].get_number() == 0); ++skill_visible);

        // Dies fuer den Notfall, um sinniges Cursoroeffnen zu produzieren
        if (skill_visible == gloB->skill_max) skill_visible = malo->skill_sel;

        for (LixIt i = --trlo->lixvec.end(); i != --trlo->lixvec.begin();
         --i) {
            if (map.distance_x(i->get_ex(), mx) <=  mmld_x
             && map.distance_x(i->get_ex(), mx) >= -mmld_x
             && map.distance_y(i->get_ey(), my) <=  mmld_d
             && map.distance_y(i->get_ey(), my) >= -mmld_u) {

                // Hypot geht von (ex|ey+etwas) aus
                // true = Beachte persoenliche Einschraenkungen wie !MultBuild
                int priority = i->get_priority(trlo->skill[skill_visible].ac,
                 cs.tribes.size(), true);

                // Invert priority if a corresponding mouse button is held
                if ((hardware.get_mrh() && useR->prioinv_right)
                 || (hardware.get_mmh() && useR->prioinv_middle)) {
                    priority = 100000 - priority;
                }
                double hypot = map.hypot(mx, my, i->get_ex(),
                                          i->get_ey() + ((mmld_d - mmld_u)/2)
                                          );
                if (priority > 0 && priority < 100000) {
                    // Die Anforderungen den offenen Mauscursur
                    // und das Schreiben des Strings auf die Info...
                    ++tarcnt;
                    if (priority >  tarinf_priority
                     ||(priority == tarinf_priority && hypot < tarinf_hypot)) {
                        tarinf = i;
                        tarinf_priority = priority;
                        tarinf_hypot    = hypot;
                    }
                    // ...sind geringer als die für Anklick-Inbetrachtnahme!
                    if ((priority > 1 && priority < 99999)
                     && (priority >  target_priority
                     || (priority == target_priority && hypot < target_hypot))
                     && !(only_dir_l && i->get_dir() ==  1)
                     && !(only_dir_r && i->get_dir() == -1)) {
                        target          = i;
                        target_priority = priority;
                        target_hypot    = hypot;
                    }
                }
            }
        }

        // Auswertung von tarinf
        if (tarinf != trlo->lixvec.end()) {
            mouse_cursor.set_y_frame(1);
        }
        pan.stats.set_tarinf(tarinf == trlo->lixvec.end() ? 0 : &*tarinf);
        pan.stats.set_tarcnt(tarcnt);

        // Auswertung von target
        // Wir kontrollieren auch die angezeigte Zahl, siehe Kommentar zur
        // sichtbaren Zahl wegen Schokolade fuer's Auge
        if (target != trlo->lixvec.end() && hardware.get_ml()
         && pan.skill[skill_visible].get_number() != 0) {
            const int lem_id = target - trlo->lixvec.begin();
            pan.pause.set_off();

            Replay::Data data = new_replay_data();
            data.action       = Replay::ASSIGN;
            data.what         = lem_id;
            replay.add(data);
            Network::send_replay_data(data);

            // Die sichtbare Zahl hinabsetzen geschieht nur fuer's Auge,
            // in Wirklichkeit geschieht dies erst beim Update. Das Augen-
            // spielzeug verabreichen wir allerdings nur, wenn nicht z.B.
            // zweimal auf dieselbe Lix mit derselben Faehigkeit
            // geklickt wurde. Den unwahrscheinlichen Fall, dass man
            // zweimal mit beide Male anwendbaren Faehigkeiten auf dieselbe
            // Lix geklickt hat, koennen wir vernachlaessigen - dann
            // erscheint die Nummernaenderung eben erst beim kommenden Update.
            // Auch der Sound (s.u.) wird dann nicht gespielt.
            if (!replay.get_on_update_lix_clicked(cs.update + 1, lem_id)
             && pan.skill[skill_visible].get_number() != LixEn::infinity) {
                pan.skill[skill_visible].set_number(
                pan.skill[skill_visible].get_number() - 1);
            }
            // Sound in der Effektliste speichern, damit er nicht beim Update
            // nochmal ertoent, und zusatzlich wird er hier nochmals gespielt,
            // damit wir sicher gehen, dass er auf jeden Fall beim Klick kommt.
            Sound::Id snd = Lixxie::get_ac_func(pan.skill[skill_visible]
                            .get_skill()).sound_assign;
            effect.add_sound(cs.update + 1, *trlo, lem_id, snd);
            Sound::play_loud(snd);
        }

    }
    // Ende von: Maus im Spielfeld ohne Zielen

    // Zielen
    else if (malo->aiming == 2) {
        // Hingucken der Lixen wird im Update der Lixen erledigt. Hier
        // geht es nur darum, einen Klick und dessen Koordinaten zu
        // registrieren. Es wird entsprechend auch ein Netzwerkpaket versandt.
        if (hardware.get_ml() && !pan.pause.is_mouse_here())
         for (LixIt lem = trlo->lixvec.begin();
         lem != trlo->lixvec.end(); ++lem) if (lem->get_aiming()) {
            pan.pause.set_off();
            // Klick sauber vormerken fuers naechste Update und verschicken.
            Replay::Data data = new_replay_data();
            data.action       = Replay::AIM;
            data.what         = (my * level.initial * map.get_xl())
                              + (mx * level.initial)
                              + lem - trlo->lixvec.begin(); // siehe replay.h
            replay.add(data);
            Network::send_replay_data(data);

            // Gegen zweimaliges Schiessen pro Update
            malo->aiming = 1;

            // Sound in der Effektliste speichern, damit er nicht beim Update
            // nochmal ertoent, und zusatzlich wird er hier nochmals gespielt,
            // damit wir sicher gehen, dass er auf jeden Fall beim Klick kommt.
            Sound::play_loud(lem->get_sound_aim());
            effect.add_sound(cs.update + 1, *trlo, lem - trlo->lixvec.begin(),
                                                   lem->get_sound_aim());
            break;
        }
    }








    // Panels ueberpruefen. Zuerst Singleplayer-Panels.
    if (cs.tribes.size() == 1) {
        // Plus und Minus werden nicht auf "clicked" geprueft, sondern aktiv,
        // wenn gehalten. Dennoch benutzen wir Button::clicked(), um Halten
        // zu ermoeglichen.
        // Doppelklicks auf F1 oder F2 werden leider nicht erkannt, im Gegen-
        // satz zu Doppelklicks auf F12 fuer die Nuke. Das liegt daran, dass
        // die Tasten nach etwas gedrueckt gehaltener Zeit immer wieder
        // key_once() ausloesen und es somit auch beim Gedrueckthalten unbe-
        // absichtigt zur Max-/Min-Einstellung kaeme.
        // Dies aendert nur die angezeigte Zahl. Die Ratenaenderung wird erst,
        // wenn ein Update ansteht, zum Replay geschickt. Das spart Ballast,
        // da alle bis auf die letzte Aenderung pro Update egal sind.
        // Modulo 2 rechnen wir bei den Help::timer_ticks, weil Frank die
        // Aenderung der Rate auch bei 60 /sec zu rasant war.
        bool minus_clicked = pan.rate_minus.get_clicked();
        bool plus_clicked  = pan.rate_plus .get_clicked();
        if (hardware.key_hold(useR->key_rate_minus)) {
            pan.rate_minus.set_down();
        }
        if (pan.rate_minus.get_down() || minus_clicked) {
            // Doppelklick?
            if (minus_clicked && Help::timer_ticks - timer_tick_last_F1
             <= hardware.doubleclick_speed) {
                pan.rate.set_number(trlo->rate_min);
            }
            else if (minus_clicked) timer_tick_last_F1 = Help::timer_ticks;
            else {
                // Normales Halten
                if (pan.rate.get_number() > trlo->rate_min) {
                    if (Help::timer_ticks % 2)
                     pan.rate.set_number(pan.rate.get_number() - 1);
                }
                else pan.rate_minus.set_down(false);
            }
        }
        // Plus
        if (hardware.key_hold(useR->key_rate_plus)) {
            pan.rate_plus.set_down();
        }
        if (pan.rate_plus.get_down() || plus_clicked) {
            if (plus_clicked && Help::timer_ticks - timer_tick_last_F2
             <= hardware.doubleclick_speed) {
                pan.rate.set_number(99);
            }
            else if (plus_clicked) timer_tick_last_F2 = Help::timer_ticks;
            else {
                if (pan.rate.get_number() < 99) {
                    if (Help::timer_ticks % 2)
                     pan.rate.set_number(pan.rate.get_number() + 1);
                }
                else pan.rate_plus.set_down(false);
            }
        }
    }

    // Check skill buttons
    if (malo) {
        // This starts at the next button and breaks immediately if one is
        // pressed. This enables a hotkey to cycle through its skills.
        // We don't read malo->skill_sel, this is only updated after a
        // gameplay physics update. Switching skills is nicer when paused
        // if we use the following variable instead.
        size_t current_button = malo->skill_sel;
        if (current_button >= pan.skill.size()) current_button = 0;
        for (size_t i = 0; i < pan.skill.size(); ++i)
         if (pan.skill[i].get_on()) {
            current_button = i;
            break;
        }
        // This next if makes that an overloaded hotkey will select the skill
        // that's more to the left always, unless a skill with the same
        // hotkey is already selected. If the if wasn't there, sometimes the
        // first hit of a new skillkey would select a different than the
        // leftmost occurence, based on where the skills are.
        if (!pan.skill[current_button].get_clicked()) current_button = 7;

        for (size_t j = 0; j < pan.skill.size(); ++j) {
            size_t i = (current_button + j + 1) % pan.skill.size();
            if (pan.skill[i].get_number() != 0
             && pan.skill[i].get_clicked()
             && !pan.skill[i].get_on()) {
                // Dies wird zwar auch beim naechsten Update erledigt, aber
                // so sieht der Button viel antwortfreudiger aus!
                pan.set_skill_on(i);
                // Das hier ist das eigentlich Wichtige
                Replay::Data data = new_replay_data();
                data.action       = Replay::SKILL;
                data.what         = i;
                replay.add(data);
                Network::send_replay_data(data);
                // Jetzt schon den Klang abspielen, dafuer beim Update nicht.
                // Es wird also der Effekt gesichert und zusaetzlich manuell
                // der Effekt vorgemerkt, falls jemand in der Pause wechselt.
                effect.add_sound(cs.update + 1, *trlo, i, Sound::PANEL);
                Sound::play_loud(Sound::PANEL);
                // Don't check any more buttons, see comment before the loop.
                break;
            }
        }
    }
    // Restliche Buttons in der normalen Calculate-Funktion

    // Atombombe
    if (!pan.nuke_single.get_on()
     && !pan.nuke_multi .get_on()) {
        if (pan.nuke_single.get_clicked()
         || pan.nuke_multi .get_clicked()) {
            if (Help::timer_ticks - timer_tick_last_F12
             <= hardware.doubleclick_speed) {
                // set_on() kommt zwar auch im Update, aber wenn wir das
                // hier immer machen, sieht es besser aus. Gleiches gilt fuer
                // den Sound, ist wie beim normalen Anklicken.
                pan.nuke_single.set_on();
                pan.nuke_multi .set_on();
                pan.pause      .set_off();
                Replay::Data data = new_replay_data();
                data.action       = Replay::NUKE;
                replay.add(data);
                Network::send_replay_data(data);
                effect.add_sound(cs.update + 1, *trlo, 0, Sound::OHNO);
                Sound::play_loud(Sound::OHNO);
            }
            else timer_tick_last_F12 = Help::timer_ticks;
        }
        // Andrueck-Effekt nachholen bei Hotkey-Ausloesung
        if (!pan.nuke_single.get_on()
         && !pan.nuke_multi .get_on()
         && hardware.key_hold(pan.nuke_single.get_hotkey())) {
            pan.nuke_single.set_down();
            pan.nuke_multi .set_down();
        }
    }

}
