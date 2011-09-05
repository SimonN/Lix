/*
 * gameplay/effect.h
 *
 * Effekt-Manager-Klasse. Gameplay besitzt ein Objekt dieser Art und fordert
 * es auf, Explosionen, Replay-Pfeile und Sounds zu verwalten. All diese Dinge
 * sind ja nicht notwendigerweise beim Neurechnen mehrfach anzuzeigen, sondern
 * nur, wenn sie noch nicht gesehen/gehoert worden sind.
 *
 * Wird allerdings manuell ein frueherer Status geladen, so muessen alle
 * zukuenftigen Effekte ebenfalls wieder auftreten. Dazu dient die Funktion
 * void delete_after(const unsigned).
 *
 * bool add(const Effect&)
 * bool add(const unsigned, const Tribe*, const unsigned = 0)
 *
 *   Fuegt das Element der Effektmenge hinzu, sofern es dort noch nicht ent-
 *   halten ist. Wenn es neu ist, dann liefert die Funktion true. Wenn es
 *   schon enthalten war, dann false. Man kann also solchen Code schreiben:
 *
 *     if effect_Manager::add(effect) Sound::play_sound(...);
 *
 */

#pragma once

#include <set>

#include "gui/arrow.h"
#include "tribe.h"

#include "../graphic/graphic.h"
#include "../graphic/map.h"
#include "../graphic/sound.h"
#include "../other/types.h"

struct Effect {

    const unsigned update;
    const Tribe*   tribe;
    const unsigned lixnr; // if not necessary, set to 0

    inline Effect(const unsigned u, const Tribe* t, const unsigned l = 0)
           : update(u), tribe(t), lixnr (l) {}
    inline ~Effect() {}

    inline bool operator < (const Effect& e) const {
        return update <  e.update
         ||   (update == e.update && tribe <  e.tribe)
         ||   (update == e.update && tribe == e.tribe && lixnr < e.lixnr);
    }
};



// ############################################################################
// ################################################################ Der Manager
// ############################################################################



class EffectManager {

private:

    static const int max_updates_history;
    static const int pickaxe_y_frame;

    // Verwaltungshilfen fuer die tatsaechlichen Grafiken
          Map&   map;  // Lokaler Spieler:
    const Tribe* trlo; // Wenn beim Sound p != 0 und p != trlo,
                       // dann den Sound leise abspielen statt laut
    const unsigned timer_ticks_for_explosion;
    long  unsigned timer_tick_last_explosion;

    // Vektoren mit den Infos ueber die Effekte
    std::set <Effect> arrow;
    std::set <Effect> explosion;
    std::set <Effect> debris;
    std::set <Effect> sound;
    unsigned          overtime;

    // Vektoren mit den tatsaechlichen Grafiken
    std::list <Arrow>   obj_arrow;
    std::list <Graphic> obj_explosion;
    std::list <Debris>  obj_debris;

    // Hilfsfunktion fuer calc()
    void delete_before(std::set <Effect>&, const unsigned);

public:

    EffectManager(Map&);
    ~EffectManager();

    inline void set_trlo(const Tribe* t) { trlo = t; }

    // add_explosion nimmt die Koordinaten der Explosion, add_sound: bool=laut
    // add_pickaxe   nimmt die Koordinaten der Hacke sowie ihre Rotation
    // add_overtime  nimmt Updates, fertigen Spieler, Anzahl Sekunden Nachzeit
    // add_sound_gen spielt laut ab, braucht keine Infos zu Tribe oder Lix
    // add_sound     spielt laut ab, falls Tribe& == trlo, sonst leise.
    // add_sound_qui spielt immer leise ab und nutzt dieselbe Effektmenge.
    // add_sound_if_ spielt laut ab, wenn Tribe == trlo, sonst gar nicht.
    void add_arrow        (Uint, const Tribe&, Uint, const Arrow&);
    void add_explosion    (Uint, const Tribe&, Uint, int, int);
    void add_pickaxe      (Uint, const Tribe&, Uint, int, int, int);
    void add_sound_general(Uint,                     const Sound::Id);
    void add_sound        (Uint, const Tribe&, Uint, const Sound::Id);
    void add_sound_quiet  (Uint, const Tribe&, Uint, const Sound::Id);
    void add_sound_if_trlo(Uint, const Tribe&, Uint, const Sound::Id);
    void add_overtime     (Uint, const Tribe&, Uint);

    int  get_effects();

    void delete_after   (const unsigned);
    void calc           (const unsigned);
    void draw           ();

    // Wichtig, damit den Bildern nicht der Boden unter den Fuessen weg-
    // gezogen wird: Erst die Bilder alle killen in ~Gameplay().
    void clear_all_lists();

};
