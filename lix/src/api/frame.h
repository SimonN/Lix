/*
 * frame.h
 *
 * class Frame
 *
 *   Dies ist ein rechteckiger Rahmen, dessen Inneres nicht gezeichnet wird.
 *   Sein Inneres erscheint nach hinten versetzt.
 *
 *   Der Zeichen- und bei gesetzter undraw_color der Entzeichnungsbereich
 *   ist um 2 Pixel nach oben, unten, rechts und links groesser als die Groesse
 *   dieses Api-Elements!
 *
 */

#pragma once

#include "element.h"

namespace Api {
class Frame : public Element {

public:

    //             Worauf?         x        y        Breite   Länge
             Frame(int = 0, int = 0, int = 0, int = 0);
    virtual ~Frame();

protected:

    virtual void draw_self();
    virtual void undraw_self();

};
} // Api
