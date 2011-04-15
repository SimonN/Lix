/*
 * editor/win_size.h
 *
 * Editorfenster, in dem die Abmessungen der Landschaft festgelegt werden
 *
 * Nach erfolgreich bestaetigtem Fenster muss der Editor noch die Werte
 * exit_with_x/y_to_adjust abfragen und alle Objekte um diese Werte
 * auf der neuen Karte verschieben.
 *
 */

#pragma once

#include "../api/button/b_text.h"
#include "../api/label.h"
#include "../api/number.h"
#include "../api/window.h"
#include "../graphic/map.h"
#include "../level/level.h"

namespace Api {
class WindowSize : public Window {

private:

    static const unsigned this_xl;
    static const unsigned this_yl;
    static const unsigned nrxl; // Number X-Length
    static const unsigned x_offset;

    static const unsigned y_l;
    static const unsigned y_r;
    static const unsigned y_u;
    static const unsigned y_d;
    static const unsigned y_x;
    static const unsigned y_y;
    static const unsigned y_torus_x;
    static const unsigned y_torus_y;

    Level& level;
    Map&   map;

    int exit_with_x_to_adjust;
    int exit_with_y_to_adjust;

    Number l;
    Number r;
    Number u;
    Number d;

    Checkbox torus_x;
    Checkbox torus_y;

    TextButton ok;
    TextButton cancel;

    Label desc_l, desc_r, desc_u, desc_d, desc_x, desc_y;
    Label desc_torus_x,   desc_torus_y;

    // Kopieren verboten
    WindowSize(const WindowSize&);
    void operator = (const WindowSize&);

    // Auslagerungsfunktion
    void draw_calculation(const int, const int, const int);

public:

             WindowSize(Level&, Map&);
    virtual ~WindowSize();

    int get_exit_with_x_to_adjust() { return exit_with_x_to_adjust; }
    int get_exit_with_y_to_adjust() { return exit_with_y_to_adjust; }


protected:

    virtual void calc_self();
    virtual void draw_self();

}; // Ende Klasse WindowSize
}  // Ende Namensraum Api
