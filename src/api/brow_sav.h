/*
 * editor/sav_brow.h
 *
 * Dateien speichern im Editor
 *
 * Waehrend des Betriebs nimmt die Dateinamenszeile nur den Dateinamen ohne
 * Endung und ohne Pfad auf. Erst beim Beenden des Fensters wird das Feld,
 * ohne es anzuzeigen, auf leer gesetzt (nicht speichern) oder auf den
 * kompletten Dateinamen inklusive Pfad und Endung.
 *
 */

#pragma once

#include "../api/list_dir.h"
#include "../api/list_lev.h"
#include "../api/texttype.h"
#include "../api/window.h"
#include "../api/message.h"
#include "../other/language.h"
#include "../other/filename.h"

namespace Api {
class SaveBrowser : public Window {

private:

    DirList   dir_list;
    ListLevel level_list;
    Texttype  file_name;

    TextButton ok;
    TextButton cancel;

    Label     desc_file_name;
    Label     desc_level_name;
    Label     desc_save_filename;
    Label     label_file_name;
    Label     label_level_name;

    bool exit_with;

    std::string extension;

    // Unterfenster
    BoxMessage* box_overwrite;
    BoxMessage* (*new_box_overwrite)(const Filename&);

    // Kopierverbot
    SaveBrowser     (const SaveBrowser&);
    void operator = (const SaveBrowser&);

    // Konstanten
    static const int window_yl;
    static const int frame_offset;
    static const int info_file_name_y;
    static const int info_level_name_y;
    static const int dir_list_y;
    static const int dir_list_xl;
    static const int dir_list_yl;
    static const int file_list_xl;

public:

    SaveBrowser(const Filename&,    // Basisverzeichnis
                const std::string&, // Dateiendung ohne Stern - NICHT Such*!
                const Filename&,    // Verzeichnis mit ggf. Datei
                      ListFile::SearchCrit,
                      BoxMessage* (*)(const Filename&),
                const bool = false); // Replay-Style in der Dateiliste?
    virtual ~SaveBrowser();

    inline bool get_exit_with() { return exit_with; }
    Filename    get_current_file();
           void set_info_filename  (const Filename&);
           void set_info_level_name(const std::string&);

    // Nutzbare Funktionen, auch als Beispiel
    static bool        search_criterion_level (const Filename&);
    static BoxMessage* new_box_overwrite_level(const Filename&);

protected:

    virtual void calc_self();

};

} // Ende Namensraum
