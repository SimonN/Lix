/*
 * gameplay/panel.h
 *
 * Das Panel am unteren Bildschirmrand, welches Informationen ueber die
 * Lixxiee sowie geretteten Lixxiee gibt und ggf. die Target Information
 * anzeigt.
 *
 * Im Achtspielermodus brauchen wir mindestens 160 Pixel in der Breite, das
 * ist so viel, wie vier normale Skill-Buttons nebeneinander lang sind.
 *
 * Ein GameplayPanel verwaltet mehrere Spielerdaten, also Objekte der Klasse
 * PanelTribe. Diese Objekte koennen auf verschieden Arten gezeichnet werden:
 *
 *  draw_big()        draw_med()        draw_sml()
 *
 * [Lm] 56 / 80        [Lm]  56            [Lm]
 * [Ex] 23 / 78        [Ex]  23             56
 * <-- frei -->        < frei >             23
 *
 *   80 Pixel          50 Pixel          20 Pixel, man verplane aber mehr
 *
 * Die der draw_*()-Funktion uebergebene X-Koordinate gibt das Zentrum der
 * PanelTribe-Zeichenausgabe an, nicht den linken Rand!
 *
 * Im Falle von draw_big() wird die untere Zahlenzeile gruen gezeichnet, wenn
 * mindestens so viele Lixxiee gerettet worden sind wie benoetigt. In beiden
 * anderen Faellen haben die Spieler mit den meisten geretteten Lixxieen > 0
 * eine gruene untere Zahl.
 *
 * X- und Y-Koordinate sind hartkodiert, sie richten sich je nach der Breit-
 * heit des Panels, einstellbar ueber set_wide(bool). Gleiches gilt fuer
 * die Laenge.
 *
 */

#pragma once

#include "tribe.h"

#include "../api/button/button.h"
#include "../lix/lix_enum.h" // Farben und Tarinf

class GameplayStats : public Api::Element {

private:

    struct PanelTribe {

        static const int frame_outopp;
        static const int frame_out;
        static const int frame_in;
        static const int frame_clock;
        static const int frame_cup;
        static const int frame_cupall;
        static const int frame_target;

        static const int frame_color;
        static const int frame_gray;
        static const int frame_green;

        static const int name_short_width;
        static const int name_medium_width;

        Torbit*      ground;
        const Tribe* tr;
        std::string  name_short;
        std::string  name_medium;
        bool         white;

        PanelTribe(Torbit&);
        ~PanelTribe();

        // Die der draw_*()-Funktion uebergebene X-Koordinate gibt das Zentrum
        // der PanelTribe-Zeichenausgabe an, nicht den linken Rand!
        void draw_local(int, int, bool,
           const int*, const Lixxie*, int,
           const LixEn::Style, const bool, const int,
           const LixEn::Style, const bool, const int) const;
        void draw_med(int, int, int = 0) const;

        bool operator > (const PanelTribe&) const;

    private:

        //               1    X,   Y,   gruen? (X = links)
        void draw_nr_sml(int, int, int, const bool = false) const;
        void draw_nr_ctr(int, int, int, const bool = false) const;
    };
    // Ende Klasse PanelTribe

    Api::Button stats_bar;   // The extra space at the right
    Api::Button stats_multi; // The extra space at the right

    std::vector <PanelTribe> tribes;
    bool show_clock;     // Wenn unbefristet, ist die Uhr unnoetig.

    int           tarcnt; // Gesamtzahl der Lixxies unterm Cursor
    const Lixxie* tarinf;
    int           clock;

    void          draw_button_connection(); // for multiplayer eye candy

public:

    //            wohin
    GameplayStats();
    ~GameplayStats();

    void add_tribe(const Tribe&);

    inline void sdr()                                { set_draw_required();  }
           void set_tribe_local(const Tribe&);
    inline void set_show_clock (const bool b = true) { show_clock = b; sdr();}
    inline void set_tarinf     (const Lixxie*     l) { tarinf     = l; sdr();}
    inline void set_tarcnt     (const int u)         { tarcnt     = u; sdr();}
    inline void set_clock      (const int u)         { clock      = u; sdr();}

protected:

           virtual void calc_self(); // just un-down all buttons
           virtual void draw_self();

};
// Ende Klasse GameplayPanel
