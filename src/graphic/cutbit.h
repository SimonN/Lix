/*
 * cutbit.h
 *
 * Laed eine Bilddatei und speichert sie in einem neu erzeugten Memory-Bitmap.
 * Es werden fuer den spaeteren Zugriff bereits die Frame-Laenge und die
 * Frame-Anzahl ausgerechnet.
 *
 * Cutbit(const std::string&, bool cut)
 *
 *   Laedt die Datei. Wenn (cut == true) ist, so wird das Bitmap geschnitten,
 *   sofern es den Eindruck macht, es bestehe aus Frames. Wenn (cut == false)
 *   ist, so wird grundsaetzlich nie geschnitten.
 *
 * Cutbit(BITMAP*, bool cut)
 *
 *   Creates a Cutbit with this bitmap, if bool cut, then it gets cut.
 *   Otherwise, it's always a single frame.
 *
 *   The BITMAP gets owned by the Cutbit! Do not destroy the BITMAP after
 *   this Cutbit has been created, it gets destroyed automatically when the
 *   Cutbit is destroyed.
 *
 * int get_pixel(const int px, const int py);
 * int get_pixel(const int xfr, const int yfr, const int px, const int py);
 *
 *   Liefert die Farbe des Pixels an Position (px, py) im Frame (xfr, yfr).
 *   In der Version mit zwei Argumenten wird immer Frame (0, 0) benutzt.
 *   Liegt der abgefragte Pixel ausserhalb des Frames, wird -1 zurueckgegeben.
 *
 * Die Rotationskennziffer gibt an, um wie viele Vielfache von Pi/2 das
 * Bild gedreht werden soll. 0 ist die Normalansicht, 2 eine halbe Drehung.
 *
 */

#pragma once

#include <vector>

#include "torbit.h"
#include "../other/filename.h"

class Cutbit {

private:

    BITMAP* bitmap;

    int xl;
    int yl;
    int x_frames;
    int y_frames;

    void cut_bitmap();

public:

    enum Mode {
        NORMAL,
        NOOW,
        DARK,
        NOOW_EDITOR,
        DARK_EDITOR,
        DARK_SHOW_NOOW, // only for some steel mask internal drawing
        STEEL,
        STEEL_NOOW
    };

    Cutbit();
    Cutbit(const Cutbit&);
    Cutbit(const Filename&, const bool);
    Cutbit(BITMAP*,         const bool); // takes over the bitmap!
    Cutbit(const std::vector <BITMAP*>&);
    ~Cutbit();

    Cutbit& operator =  (const Cutbit&);

    bool    operator == (const Cutbit&) const;
    bool    operator != (const Cutbit&) const;

    inline operator bool()        const { return bitmap;   }
    inline BITMAP* get_al_bitmap()const { return bitmap;   }
    inline int     get_xl()       const { return xl;       }
    inline int     get_yl()       const { return yl;       }
    inline int     get_x_frames() const { return x_frames; }
    inline int     get_y_frames() const { return y_frames; }

           int     get_pixel(const int, const int)                       const;
           int     get_pixel(const int, const int, const int, const int) const;

    // Cutbit zeichnen auf die angegebene Unterlage
    void draw(Torbit&,
              const int    = 0, const int = 0, // X-, Y-Position
              const int    = 0, const int = 0, // X-, Y-Frame
              const bool   = false,            // Vertikal gespiegelt?
              const double = 0,                // Vierteldrehungen?
              const double = 0,                // Strecken? Nein = 0 oder 1.
              const Mode   = NORMAL) const;    // Farbe malen anstatt Objekt,
                                               // gaengig: -1 fuer Loeschterr.
};
