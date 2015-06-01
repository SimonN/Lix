/*
 * gameplay/panel.h
 *
 * Das Panel am unteren Bildschirmrand, welches Informationen ueber die
 * Lix out sowie Lix saved gibt, target information, ...
 *
 */

#pragma once

#include "../tribe.h"

#include "../../api/button/button.h"
#include "../../api/label.h"
#include "../../api/frame.h"
#include "../../lix/lix_enum.h" // Farben und Tarinf

 // Gameplay panel mode
enum GapaMode {
    GM_NONE,
    GM_PLAY_SINGLE,
    GM_PLAY_MULTI,
    GM_REPLAY_SINGLE,
    GM_REPLAY_MULTI,
    GM_SPEC_MULTI
};



class GameplayStats : public Api::Element {

private:

    struct PanelTribe {

        static const int frame_outopp;
        static const int frame_out;
        static const int frame_hatch;
        static const int frame_in;
        static const int frame_countd;
        static const int frame_stopw;
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
           const int*, const int*,
           const Lixxie*, int, const int) const;

        bool operator > (const PanelTribe&) const;

    private:

        //               1    X,   Y,   gruen? (X = links)
        void draw_nr_sml(int, int, int, const bool = false) const;
        void draw_nr_ctr(int, int, int, const bool = false) const;
    };
    // Ende Klasse PanelTribe

    Api::Button stats_bar;   // The long horizon bar over the skill panel
    Api::Button stats_multi; // The extra space at the right
    Api::Frame  frame_around_scores;

    std::vector <PanelTribe> tribes;
    GapaMode gapamode;   // show the multiplayer stats
    bool show_countd;    // Wenn unbefristet, zeige Stoppuhr

    int           tarcnt; // Gesamtzahl der Lixxies unterm Cursor
    const Lixxie* tarinf;
    int           countd;
    int           stopw;

    Api::Label    help; // if nonempty, show this instead of regular stuff

    void draw_clones_bar(int, int, int, int, LixEn::Style); // abs x, y, xl, yl
    void draw_button_connection(); // for multiplayer eye candy

    void draw_self_play_single();
    void draw_self_play_multi();
    void draw_self_spec_multi();

public:

    GameplayStats();
    ~GameplayStats();

    void add_tribe(const Tribe&);

    inline void set_gapamode     (GapaMode gm)       { gapamode = gm;        }
    inline void sdr()                                { set_draw_required();  }

           void set_tribe_local  (const Tribe*);
    inline void set_use_countdown(const bool b=true) { show_countd= b; sdr();}
    inline void set_tarinf       (const Lixxie*   l) { tarinf     = l; sdr();}
    inline void set_tarcnt       (const int u)       { tarcnt     = u; sdr();}
    inline void set_countdown    (const int u)       { countd     = u; sdr();}
    inline void set_stopwatch    (const int u)       { stopw      = u; sdr();}

           void set_help         (const std::string& s = "");

protected:

           virtual void calc_self(); // just un-down all buttons
           virtual void draw_self();

};
// Ende Klasse GameplayPanel
