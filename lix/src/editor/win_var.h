/*
 * varwin.h
 *
 * Dies ist ein editorinternes Fenster zum Einstellen der Levelvariablen,
 * etwa Name und Anzahl der Lemminge. Faehigkeiten werden in einem separaten
 * Fenster konfiguriert!
 *
 */

#pragma once

#include "../api/button/b_text.h"
#include "../api/label.h"
#include "../api/number.h"
#include "../api/texttype.h"
#include "../api/window.h"
#include "../level/level.h"

namespace Api {
class WindowVariables : public Window {

private:

    static const int this_xl;
    static const int this_yl;

    Level*     level; // Wird als Referenz dem Konstruktor uebergeben

    Texttype   author;
    Texttype   name_german;
    Texttype   name_english;

    Number     initial;
    Number     required;
    Number     rate;
    Number     seconds;

    TextButton ok;
    TextButton cancel;

    Label desc_author,  desc_german,   desc_english,
          desc_initial, desc_required, desc_rate,    desc_clock;

    // Kopieren verboten
    WindowVariables (const WindowVariables&);
    void operator = (const WindowVariables&);

public:

             WindowVariables(Level&);
    virtual ~WindowVariables();

protected:

    virtual void calc_self();

}; // Ende class WindowVariables
}  // Ende Namensraum
