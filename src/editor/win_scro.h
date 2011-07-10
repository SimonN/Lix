/*
 * editor/win_scro.h
 *
 * Fenster, in dem die anfaengliche Scrollposition festgelegt werden kann
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
class WindowScroll : public Window {

private:

    static const unsigned this_xl;
    static const unsigned this_yl;
    static const unsigned nrxl; // Number X-Length
    static const unsigned rgxl; // Regular X-Length
    static const unsigned step_small; // Fuer Rasterbestimmung

    Level& level;
    Map&   map;

    Label desc_win_scroll;

    Number x;
    Number y;

    TextButton jump;
    TextButton current;

    Number     bg_r;
    Number     bg_g;
    Number     bg_b;

    Label      desc_win_scroll_x;
    Label      desc_win_scroll_y;
    Label      desc_win_scroll_r;
    Label      desc_win_scroll_g;
    Label      desc_win_scroll_b;

    TextButton ok;
    TextButton cancel;

    // Kopierverbot
    WindowScroll    (const WindowScroll&);
    void operator = (const WindowScroll&);

public:

             WindowScroll(Level&, Map&);
    virtual ~WindowScroll();

protected:

    virtual void calc_self();

}; // Ende Klasse WindowScroll
}  // Ende Namensraum Api
