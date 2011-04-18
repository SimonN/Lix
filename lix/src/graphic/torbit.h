/*
 * graphic/torbit.h
 *
 * Wrapper fuer Allegros BITMAP inklusive Torusfunktionen (optional).
 *
 */

#pragma once

#include "../other/myalleg.h"

class Torbit {

private:

    BITMAP* bitmap;

    bool    torus_x;
    bool    torus_y;

public:

    Torbit(int, int, bool = false, bool = false);
    Torbit(const Torbit&);
    virtual ~Torbit();

    Torbit& operator = (const Torbit&);

    inline BITMAP* get_al_bitmap() const { return bitmap; }
    inline int     get_xl()        const { return bitmap ? bitmap->w : 0; }
    inline int     get_yl()        const { return bitmap ? bitmap->h : 0; }

    virtual void   resize(int, int);

    inline bool         get_torus_x () const        { return torus_x;   }
    inline bool         get_torus_y () const        { return torus_y;   }
    inline virtual void set_torus_x (bool b = true) { torus_x = b;      }
    inline virtual void set_torus_y (bool b = true) { torus_y = b;      }
    inline virtual void set_torus_xy(bool x,bool y) { set_torus_x(x);
                                                      set_torus_y(y);   }
    // Abstandsfunktionen, die die Toruseigenschaft beachten
    // Diese Funktionen funktionieren semantisch wie 1. Arg. minus 2. Arg.
    int    distance_x(int, int)           const;
    int    distance_y(int, int)           const;
    double hypot     (int, int, int, int) const;

    // Malprimitiva
    void clear_to_color(int);
    void set_pixel     (int, int, int);
    int  get_pixel     (int, int) const;

    // Rechtecke haben, anders als Allegros rect() und rectfill(),
    // folgende Parameter: x, y, xl, yl, Farbe
    void draw_rectangle       (int, int, int, int, int);
    void draw_filled_rectangle(int, int, int, int, int);

    // (ggf. x, y, xl, yl), alte Farbe, neue Farbe
    void replace_color        (int, int);
    void replace_color_in_rect(int, int, int, int, int, int);

    // Den Torus auf den angegebenen Torus kopieren, und zwar an jene Stelle
    // auf dem uebergebenen Torus, die durch die Koordinaten beschrieben ist.
    // 2.: Sind die hinteren Koordinaten != 0, dann xl, yl, sonst alles malen.
    // 3.: Ein Bitmap auf ein Torbit zeichnen, x, y, mirr,rot,scal:0=1
    virtual void draw     (Torbit&, int=0, int=0)               const;
            void draw     (BITMAP*, int=0, int=0, int=0, int=0) const;
            void draw_from(BITMAP*, int=0, int=0, bool = false,
                                    double = 0, double = 0);

private:

    void rcir_at               (int, int, int, int, int, int);
    void draw_rectangle_private(int, int, int, int, int,
                                void (*)(BITMAP*, int, int, int, int, int));

    void draw_from_at(BITMAP*, int, int, // Quelle, x und y
                      void (*)(BITMAP*, BITMAP*, int, int) = 0,
                      void (*)(BITMAP*, BITMAP*, int, int, fixed, fixed) = 0,
                      double = 0, double = 0); // rot,(scal==0 -> 1),kein mirr!

};
