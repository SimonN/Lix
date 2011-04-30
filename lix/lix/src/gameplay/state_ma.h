/*
 * gameplay/state.h
 *
 * Statesaves-Manager: Hier werden alle vollwertigen Spielstaende verwaltet,
 * die unterweges so anfallen. Das angeforderte Speichern im Einspielermodus
 * zaehlt dazu, aber auch das automatische Speichern im Netzwerkmodus.
 * In diesen Faellen wird bei verspaetet eintreffenden Paketen ab einem
 * geeingeten Spielstand neu gerechnet.
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

class StateManager {

private:

    static const unsigned updates_sml;
    static const unsigned updates_med;
    static const unsigned updates_big;

    State zero,  user,
          sml_1, sml_2,
          med_1, med_2,
          big_1, big_2;

public:

    inline void         save_zero(const State& s) { zero = s;    }
    inline void         save_user(const State& s) { user = s;    }
    inline const State& load_zero()               { return zero; }
    inline const State& load_user()               { return user; }

    const        State& load_auto(Ulng);
    void                calc_save_auto(const State&);

};
