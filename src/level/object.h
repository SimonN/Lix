/*
 * level/object.h
 *
 * Levelobjekt, also Terrain, Klappe, Ausgang, Falle, Einbahnstrasse und
 * dergleichen.
 *
 * Ein solches Objekt speichert sowohl die Daten, die schon in L1 wichtig
 * waren und aus den DAT-Dateien ausgelesen werden, als auch andere Dinge,
 * sofern ich das brauche. Auch selbstgezeichnete Spezialobjekte sollen
 * hierdurch dargestellt werden koennen.
 *
 */

#pragma once

#include "../graphic/cutbit.h"
#include "../graphic/sound.h"

struct Object {

    enum Type {
        EMPTY,
        TERRAIN, // subtype 1 = steel
        DECO,    // subtype 1 = flying flag, not wanted in networked games
        HATCH,
        GOAL,
        TRAP,
        WATER,   // subtype 1 = fire
        ONEWAY,  // subtype 1 = pointing right instead of left
        FLING,	 // subtype & 1 = always same xdir, subtype & 2 = non-constant
        TRAMPOLINE,
        MAX
    };

    Cutbit cb;

    Type type;
    int  subtype;

    int  selbox_x;  // These coordinates locate the smallest rectangle inside
    int  selbox_y;  // the object's cutbit's frame (0, 0) that still holds all
    int  selbox_xl; // nontransparent pixels. This refines the selection
    int  selbox_yl; // with a pulled selection rectangle in the Editor.

    int  trigger_x;
    int  trigger_y;
    int  trigger_xl;
    int  trigger_yl;
    bool trigger_xc; // center around trigger_x instead of going right from it
    bool trigger_yc; // center around trigger_y instead of going down from it

    int  special_x; // not used for everything, but e.g. for flingers
    int  special_y;

    Sound::Id sound;

    ////////////////
    // Funktionen //
    ////////////////

    Object(const Cutbit&, Type = EMPTY, int = 0);
    ~Object();

    void read_definitions_file(const Filename&);

    // these do automatically the calculation of the absolute trigger location
    inline int get_trigger_x() const {
                                return trigger_x - trigger_xc*trigger_xl/2; }
    inline int get_trigger_y() const {
                                return trigger_y - trigger_yc*trigger_yl/2; }

    // Macht aus TERRAIN = 0, HATCH = 1, ... die richtige Anzeigereihenfolge
    static Object::Type perm(const int);

private:

    void determine_selbox();

};
