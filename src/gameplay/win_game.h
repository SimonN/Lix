/*
 * gameplay/win_game.h
 *
 * Das Fenster, welches im Spiel aufgerufen werden kann und nach dem Spiel auf
 * jeden Fall erscheint. Es gibt drei Formen dieses Fensters, die jeweils einen
 * eigenen Konstruktoraufruf besitzen. Die Variablen *_results merken sich's.
 *
 */

#pragma once

#include "tribe.h"
#include "replay.h"

#include "../api/brow_sav.h"
#include "../api/message.h"
#include "../api/window.h"

namespace Api {
class WindowGameplay : public Window {

public:

    enum ExitWith {
        NOTHING,
        RESUME,
        RESTART,
        MENU
    };

private:

    static const unsigned this_xl_network_results;
    static const unsigned this_yl_network_results;
    static const unsigned this_xl_single_results;
    static const unsigned this_yl_single_results;
    static const unsigned this_xl_no_results;
    static const unsigned this_yl_no_results;
    static const unsigned this_yl_net_no_results;
    static const unsigned top_item;
    static const unsigned y_saved;
    static const unsigned y_comment;
    static const unsigned y_button;

    bool       game_end;
    bool       game_net;
    ExitWith   exit_with;

    int        lix_saved;    // These are != only at end of singleplayer mode.
    int        lix_required; // We don't use labels for these, b/c big digits.
    int        lix_at_start; // This too

    TextButton resume;
    TextButton restart;
    TextButton save_replay;
    TextButton menu;

    Replay*            replay;
    const Level* const level;

    std::vector <Label> labels; // add everything here, only then add_child()

    SaveBrowser*       browser_save;

    // Sortierung der Netzwerkspieler
    struct SortablePlayer {
        bool        pl;
        std::string name;
        unsigned    score;
        SortablePlayer(const bool b, const std::string& n, const unsigned s)
         : pl(b), name(n), score(s) {}
        inline bool operator < (const SortablePlayer& s) const {
         return this->score>s.score || this->score==s.score && this->pl&&!s.pl;
        }
    };



    // Kopierverbot
    WindowGameplay  (const WindowGameplay&);
    void operator = (const WindowGameplay&);

    // Funktion
    void common_constructor();
    static Api::BoxMessage* new_box_overwrite           (const std::string&);
    static bool             search_criterion_save_replay(const std::string&);

public:

    // Aufruf waehrend des Spiels; Level* == 0 in singleplayer, != 0 in multpl
    WindowGameplay(Replay*, const Level* const);
    // Aufruf nach einem Einzelspielerspiel
    WindowGameplay(Replay*, int, int, int, int, int, const std::string&);
    // Aufruf nach einem Mehrspielerspiel
    WindowGameplay(Replay* rep, const std::vector <Tribe>&,
                // tribe_local   cs.tribes.size(), Fenstertitel
                   const Tribe*, const unsigned, const std::string&,
                   const Level* const);

    virtual ~WindowGameplay();

    inline bool     get_game_end () { return game_end;  }
    inline ExitWith get_exit_with() { return exit_with; }

protected:

    virtual void calc_self();
    virtual void draw_self();

}; // Ende Klasse WindowGameplay
}  // Ende Namensraum Api
