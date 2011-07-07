/*
 * api/list_fil.h
 *
 * Dateiliste: Abstrakte Basisklasse, von der die Listen fuer Levels oder
 * Bilder abgeleitet werden.
 *
 * Zu beachten beim Ableiten:
 *
 *   Folgende Methoden muessen definiert werden:
 *
 *   void add_file_button(const int, const int)
 *   void add_flip_button()
 *
 *   Sie sollen Button-Zeiger auf neu erschaffene Button-Objekte oder Objekte
 *   beliebiger von Button abgeleiteter Klassen einbringen. Dazu kann die
 *   Funktion void button_push_back(Button* b) genutzt werden, siehe dazu
 *   weiter unten in diesem Kommentar.
 *
 *   Beispiele fuer die Nutzung der beiden zu definierenden Funktionen gibt
 *   es in den Dateien "api/list_lev.cpp" oder "api/list_bit.cpp".
 *
 *   Aufgerufen werden die beiden Funktionen durch ListFile::load_dir().
 *
 * void button_push_back(Button* b)
 *
 *   Nimmt einen Zeiger auf einen Button oder gerne auch auf ein Objekt einer
 *   von Button abgeleiteten Klasse, setzt die Undraw-Farbe auf die normale
 *   Fensterfarbe und ordnet den Zeiger in den Vektor ein.
 *
 * unsigned int bottom_button
 *
 *   Der Vektor-Eintrag des untersten Buttons. Wenn zu viele Levels im Ver-
 *   zeichnis sind, dann wird der unterste Button zum Blaetter-Button.
 *
 * bool clicked(...)
 *
 *   Liefert "true", wenn ein Level angeklickt wurde. Wird lediglich der
 *   Blaetter-Button angeklickt, wird wie im Nichtklickfall "false" zurueck-
 *   gegeben.
 *
 *   Die angeklickte Datei wird in "current file" geschrieben. Der komplette
 *   Pfad ist dann "current_dir + current_file".
 *
 */

#pragma once

#include <string>
#include <vector>

#include "button/button.h"
#include "frame.h"

#include "../other/log.h"

namespace Api {
class ListFile : public Frame {

public:

    typedef bool (*SearchCrit)(const std::string&);

    ListFile(int, int, int, int, SearchCrit = 0);
    virtual ~ListFile();

    void load_dir            (const std::string&, const int which_page = 0);
    void highlight_file      (const std::string&);

    inline void set_search_criterion(SearchCrit sc = 0)  { search_criterion = sc;   }
    inline SearchCrit get_search_criterion()             { return search_criterion; }

    inline bool               get_clicked()              { return clicked;          }
    inline Button*            get_button_last_clicked()  { return button_last_clicked; }
    inline unsigned           get_bottom_button()        { return bottom_button;    }
    inline unsigned           get_files_total()          { return file.size();      }
    inline int                get_page()                 { return page;             }

    inline const std::string& get_file(const unsigned i) { return file[i];          }
    inline const std::string& get_current_file()         { return current_file;     }
    inline const std::string& get_current_dir()          { return current_dir;      }
    inline const std::string& get_current_file_dir()     { return current_file_dir; }

    inline void  set_bottom_button(const unsigned i)     { bottom_button = i;       }
    inline void  set_activate_clicked_button()           { activate_clicked_button = true; }

protected:

            void button_clear    ();
            void button_push_back(Button* b);
    virtual void add_file_button (const int, const int) = 0;
    virtual void add_flip_button ()                     = 0;

    // Alter the listed directory contents if necessary.
    virtual void on_dir_load     ()                     { }
    inline  void put_to_file_list(const std::string& s) { file.push_back(s); }

    virtual void calc_self();
    virtual void draw_self();

private:

    unsigned page;
    unsigned bottom_button;
    unsigned file_number_at_top;
    bool     bottom_button_flips_page;

    bool     activate_clicked_button;
    bool     clicked;

    std::vector <std::string> file;
    std::vector <Button*>     button;
    Button*                   button_last_clicked;

    std::string current_dir;      // Geoeffnetes Verzeichnis
    std::string current_file;     // Zuletzt angeklickte/gewaehlte Datei, die
    std::string current_file_dir; // ...nicht im current_dir sein muss!

    SearchCrit search_criterion;

    void load_current_dir();
    static void static_put_to_file(std::string&, void*);

    // Kopierverbot
    ListFile        (const ListFile&);
    void operator = (const ListFile&);

};
}
