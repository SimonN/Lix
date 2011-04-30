/*
 * api/texttype.h
 *
 * Ein Button mit Texteingabezeile. Sobald das Feld angeklickt wird, nimmt es
 * Zeichen von der Tastatur auf. Damit das Ganze nicht zu kompliziert zu
 * programmieren wird, kann der Benutzer nur am Ende der Eingabezeile
 * den Text bearbeiten. [Backspace] loescht jedoch wie gewohnt :-)
 *
 * Ein Mausklick, ein Druck auf [Enter] oder [ESC] beendet den Schreibmodus.
 *
 * static Texttype* selection
 *
 *   Zeigt dies auf 0, so durchlaufen alle Fenster die normalen Schleifen.
 *   Ein Fenster mit Texteingabezeilen sollte jedoch pruefen, ob dieser
 *   Zeiger irgendwohin zeigt: In diesem Fall sollte nur die Hauptschleife
 *   des Zieles ausgefuehrt werden, damit Klicks und Tastendruecke nicht mehr
 *   als eine Aktion verursachen. Wird das Zeigerziel geloescht, so wird der
 *   Zeiger per Destruktor auf 0 gesetzt.
 *
 * void set_text(std::string& t)
 *
 *   Setzt den Text der Eingabezeile auf den String t, aber schneidet aehnlich
 *   wie bei der manuellen Eingabe ab, wenn der Text zu lang ist.
 *
 */

#pragma once

#include <string>

#include "button/button.h"

namespace Api {
class Texttype : public Button {

private:

    bool        invisible;    // Keinen Button zeichnen, nur den Eingabetext
    bool        scroll;       // Mehr Text eingebbar, als das Feld lang ist

    std::string text;
    std::string text_backup;  // Bei ESC/Rechtsklick wird Zurueckgeschrieben

    void*       on_enter_void;
    void*       on_esc_void;
    void        (*on_enter)(void*);
    void        (*on_esc)  (void*);

    bool        get_too_long(const std::string);
    bool        get_too_long() { return get_too_long(text); }

public:

    Texttype(const int, const int, const int);
    Texttype(const int, const int, const int, std::string&);
    Texttype(Texttype&);
    virtual ~Texttype();

    inline void         set_invisible(const bool b = true) { invisible = b;   }
    inline void         set_scroll   (const bool b = true) { scroll    = b;   }
    inline std::string& get_text     ()                    { return text;     }
           void         set_text  (const std::string& = "");

           void         set_on(bool = true);
           void         set_off();

    inline void set_on_enter(void* v, void (*f)(void*)) { on_enter = f; on_enter_void = v; }
    inline void set_on_esc  (void* v, void (*f)(void*)) { on_esc   = f; on_esc_void   = v; }

protected:

    virtual void        calc_self();
    virtual void        draw_self();

}; // Ende Texttype
}  // Ende Namensraum
