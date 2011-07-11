/*
 * api/list_lev.h
 *
 * Dateiliste mit Levelnamen,
 * ferner eine Dateiliste mit Replays.
 *
 * Beides sind Erweiterungen der Klasse ListFile.
 *
 */

#pragma once

#include "list_fil.h"

namespace Api {
class ListLevel : public ListFile {

private:

    bool write_file_names; // "dateiname: Levelname" anstatt "Levelname"
    bool replay_style;     // Replay-Infos anfordern statt Levelinfos
    bool checkmark_style;  // Einzelspieler: Geschaffte Levels abhaken

    virtual void add_file_button(const int, const int);
    virtual void add_flip_button();

    // Kopierverbot
    ListLevel       (const ListLevel&);
    void operator = (const ListLevel&);

public:

    ListLevel(const int, const int, const int, const int);
    virtual ~ListLevel();

    inline void set_write_file_names(const bool b = true) { write_file_names = b; }
    inline void set_replay_style    (const bool b = true) { replay_style     = b; }
    inline void set_checkmark_style (const bool b = true) { checkmark_style  = b; }

    // Muss von anderen nie aufgerufen werden, kann aber gelesen/benutzt werden
    static bool search_criterion(const Filename&);

}; // Ende Klasse ListLevel
}  // Ende Namensraum Api
