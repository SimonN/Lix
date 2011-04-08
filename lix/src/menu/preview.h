/*
 * menu/preview.h
 *
 * Eine Levelvorschau in einem schoenen Rahmen
 *
 */

#pragma once

#include "../api/frame.h"
#include "../graphic/graphic.h"
#include "../level/level.h"

namespace Api {
class Preview : public Frame {

private:

    Torbit*       torbit;
    Graphic       icon_status;
    Graphic       icon_torus;
    Level::Status status;

public:

    Preview(int, int, unsigned, unsigned);
    Preview(const Preview&);
    virtual ~Preview();

    Preview& operator = (const Preview&);

    inline Level::Status get_status() { return status; }

    void set_level(const Level&);
    void clear();

protected:

    void draw_self();

}; // Ende Klasse Preview
}  // Ende Namensraum Api
