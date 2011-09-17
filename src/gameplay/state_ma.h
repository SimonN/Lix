/*
 * gameplay/state.h
 *
 * Statesaves-Manager: Hier werden alle vollwertigen Spielstaende verwaltet,
 * die unterweges so anfallen. Das angeforderte Speichern im Einspielermodus
 * zaehlt dazu, aber auch das automatische Speichern im Netzwerkmodus.
 * In diesen Faellen wird bei verspaetet eintreffenden Paketen ab einem
 * geeingeten Spielstand neu gerechnet.
 *
 * For the user-triggered save, it also remembers the replay that was active
 * then. This is important since the user could otherwise restart, do something
 * deviating, and then load the user state that supposes the old, differing
 * replay.
 *
 * void calc_save_auto(unsigned long, std::vector <Player>&, BITMAP*)
 *
 *   Schaut sich die uebergebene Update-Zahl an und entscheidet, wie mit den
 *   weiteren uebergebenen Daten zu verfahren ist: Speichern oder nichts tun.
 *   In dieser Funktion werden, je nach Bedarf, auch die bisher angesammelten
 *   automatischen Staende per Zeigervertausch umgelegt.
 *
 * const State& load_user()
 * const State& load_auto(unsigned long)
 *
 *   Diese laden einen zuvor gespeicherten Spielstand: Entweder den per
 *   Klick auf die Speichertaste angelegten Stand oder den juengsten Stand,
 *   der mindestens so alt ist wie die angegebene Spielzeit in Updates.
 *
 *   Die letztere Funktion wird die Gameplay-Klasse moeglicherweise dazu
 *   bringen, vieles neu auszurechnen und dabei auch haeufig neue calc()-
 *   -Anforderungen zu stellen. Neuere automatisch gespeicherte Staende als
 *   der von load_auto() zurueckgegebene werden dadurch beim Neurechnen
 *   ebenfalls aktualisiert.
 *
 * Laden von nicht vorhandenen Staenden
 *
 *   Wenn einmal kein Stand gespeichert ist, auf den die Kriterien zutreffen,
 *   wird ein standardkonstruiertes State-Objekt zurueckgegeben. Dies zeigt
 *   auf kein Bitmap, hat keine Spieler und eine Update-Zahl von 0. Es liegt
 *   in diesem Fall am Gameplay, ein neues Spiel aufzusetzen. Um ein Update
 *   auf Brauchbarkeit zu ueberpruefen, kann mit State::operator bool
 *   getestet werden.
 *
 */

#pragma once

#include "state.h"
#include "replay.h"

class StateManager {

private:

    static const unsigned updates_sml;
    static const unsigned updates_med;
    static const unsigned updates_big;

    GameState
        zero,  user,
        sml_1, sml_2,
        med_1, med_2,
        big_1, big_2;

    Replay userrep;

public:

    inline void  save_zero(const GameState& s) { zero = s; }
    inline void  save_user(const GameState& s,
                           const Replay& r)    { user = s; userrep = r; }

    inline const GameState& get_zero()        { return zero;    }
    inline const GameState& get_user()        { return user;    }
    inline const Replay&    get_user_replay() { return userrep; }

    const        GameState& get_auto(Ulng);
    void                    calc_save_auto(const GameState&);

};
