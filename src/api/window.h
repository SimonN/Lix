/*
 * api_wind.h
 *
 * class Window
 *
 *   Ein rechteckiges Fenster mit Titelleiste. Hiervon werden die einzelnen
 *   Dialoge abgeleitet. Enthaelt bereits die "Exit"-Variable, aber die
 *   einzelnen Rueckgabe-Abruffunktionen muessen die abgeleiteten Klassen
 *   selber bereitstellen.
 *
 * bool exit
 *
 *   Dies wird von der Instanz, die das Fensterobjekt verwaltet, abgefragt.
 *   Normalerweise wird das Fenster dann jedoch nicht beendet, sondern
 *   lediglich:
 *   1. die Fensterausgabe abgefragt (get_exit_with())
 *   2. exit wieder auf "false" gesetzt
 *   3. das Fenster unsichtbar gemacht (siehe visible).
 *
 * void draw_text(std::string& s, const int x, const int y)
 *
 *   Zeichnet einen Schattentext in etwas hellerer Farbe als der des Fensters
 *   linksbuendig an die entsprechende Stelle. Das ist einfach ein nicht
 *   anklickbarer Erklaerungstext. Diese Funktion wird sehr haeufig benutzt
 *   in vielen Fenstern, darum kann man sie zu einem entsprechend kuerzeren
 *   Aufruf verallgemeinern.
 *
 */

#pragma once

#include "element.h"
#include "label.h"

namespace Api {
class Window : public Element {

private:

    bool        exit;

    std::string title;
    std::string subtitle;

    Label       label_title;

public:

    // Wird 20 gesetzt. Fenster müssen mindestens 30 y-lang sein.
    static const int title_bar_height;

             //     wohin, x, y, x_len, y_len, Fenstertitel
             Window(const int, const int,
                    const unsigned, const unsigned, const std::string&);
    virtual ~Window();

    inline Torbit&       get_torbit  ()       { return get_ground(); }
    inline std::string&  get_title   ()       { return title;        }
    inline std::string&  get_subtitle()       { return subtitle;     }
           void          set_title   (const std::string&);
           void          set_subtitle(const std::string&);

    inline bool get_exit() const        { return exit;    }
           void set_exit();

    void draw_title();

protected:

    virtual void draw_self();

};
// Ende von class Window

}
// Ende von namespace Api

