/*
 * menu/brow_big.h
 *
 * BrowserBig
 *
 * Aus diesem Holz sind die grossen Browser des Spiels geschnitzt, die eine
 * Vorschau und grosse Buttons rechts haben.
 *
 * Die abgeleiteten Klassen sollten die leeren virtuellen Methoden ueberladen.
 * Die BrowserBig-Klasse ruft die Methoden mit dem Dateinamen der erbrowsten
 * Datei auf. Die abgeleiteten Klassen sollen dann den korrekten Level heraus-
 * suchen (entweder einfach Level(erbrowster Name) oder im Falle des Replay-
 * Browsers Level(Leveldateiname aus der erbrowsten Replaydatei)). Sie geben
 * z.B. dann den Befehl an BrowserBig, Preview::set_level(damit) aufzurufen.
 *
 * Die abgeleiteten Klassen sollten im Konstruktor pruefen, ob der automatisch
 * gewaehlte Level gut ist, und entsprechend on_level_highlight() selbst
 * aufrufen. Siehe dazu auch den Hinweis im Konstruktor.
 *
 */

#pragma once

#include "preview.h"

#include "../api/labeltwo.h"
#include "../api/list_dir.h"
#include "../api/list_lev.h"
#include "../api/window.h"

namespace Api {

class BrowserBig : public Window {

public:

    static const unsigned dir_list_xl = 120;
    static const unsigned lev_list_xl = 300;
    static const unsigned any_list_yl = 420;

    static const unsigned but_x             = 480;
    static const unsigned but_xl      = 140;
    static const unsigned but_yl      =  40;
    static const unsigned but_y_spacing     =  20;
    static const unsigned pre_yl      =  93;
    // 93 hier passt zwar nicht genau fuer 640/420, richtig waere 92.
    // Aber es macht es das Bild um eins laenger und durch die komischen
    // Konstanten in der level.cpp gewinnen wir hier Laenge ohne --Qualitaet.

    enum ExitWith {
        EXIT_WITH_NOTHING,
        EXIT_WITH_OKAY,
        EXIT_WITH_EDIT,
        EXIT_WITH_EXIT
    };

private:

    ExitWith    exit_with;
    unsigned    info_y;

    Filename    file_recent; // only used for highlighting, not selecting

    DirList     dir_list;
    ListLevel   lev_list;

    TextButton  button_play;
    TextButton  button_exit;

    Preview     preview;

    // Kopierverbot
    BrowserBig      (const BrowserBig&);
    void operator = (const BrowserBig&);

public:

             BrowserBig(const std::string&,  // Fenstertitel
                        const Filename&,     // Basisverzeichnis
                        const Filename&,     // letztes Verzeichnis/Datei
                        const bool = false,  // Checkmark-Stil in der LevelList
                        const bool = false); // Replay-Stil in der LevelList
    virtual ~BrowserBig();

    inline void         set_button_play_text(const std::string& s)
                                                    { button_play.set_text(s); }

           void         set_exit_with(ExitWith e);
    inline ExitWith     get_exit_with()             { return exit_with;     }

    inline const Filename& get_base_dir()     { return dir_list.get_base_dir(); }
    inline const Filename& get_current_file() { return lev_list.get_current_file(); }
           void            set_current_dir_to_parent_dir();

           void         reload_dir();

    inline void         set_preview_y(int y)        { preview.set_y(y);     }
    inline void         set_preview  (const Level&l){ preview.set_level(l); }
    inline void         clear_preview()             { preview.clear();      }

    inline unsigned     get_info_y   () const       { return info_y;        }
    inline void         set_info_y   (const int i)  { info_y = i;           }

    inline virtual void on_file_highlight(const Filename&) {}
    inline virtual void on_file_select   (const Filename&) {}

protected:

           virtual void calc_self();

};
} // Ende Namensraum Api
