/*
 * graph_bg.h
 *
 * Grafikobjekt, welches beim Zeichnen seinen Hintergrund in einem separaten
 * Bitmap speichert und beim Entzeichen dieses Bitmap an seinen Platz kopiert.
 *
 */

#pragma once

#include "graphic.h"

class GraphicBG : public Graphic {

private:

    BITMAP *bg;

    bool   drawn;
    int    x_drawn;
    int    y_drawn;

public:

    GraphicBG(const Cutbit&,      Torbit&, int = 0, int = 0);
    GraphicBG(const GraphicBG&);
    virtual ~GraphicBG();

    GraphicBG& operator = (const GraphicBG&);

    virtual void undraw();
    virtual void draw();

};
