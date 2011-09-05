/*
 * editor/graph_ed.h
 *
 * Erweiterung des Grafikobjekts aus graphic/graphic.h, um Rueckbezueglichkeit
 * zu dem Levelobjekt, das wir abbilden moechten, nicht zu verlieren.
 *
 */

#pragma once

#include "../graphic/graphic.h"
#include "../graphic/map.h"
#include "../level/object.h"

class Lookup;

class EdGraphic : public Graphic {

private:

    const Object* object;
    bool          draw_info; // draw the trigger area and hatch direction

public:

    EdGraphic(Torbit&, const Object*, int = 0, int = 0);
    virtual ~EdGraphic();

    inline const Object* get_object() const { return object; }

    inline bool get_draw_info()              { return draw_info; }
    inline void set_draw_info(bool b = true) { draw_info = b;    }

    virtual int get_xl()            const;
    virtual int get_yl()            const;
    virtual int get_pixel(int, int) const;

    virtual int get_selbox_x()  const;
    virtual int get_selbox_y()  const;
    virtual int get_selbox_xl() const;
    virtual int get_selbox_yl() const;

    virtual void animate();
    virtual void draw();
            void draw_with_trigger_area();
    virtual void draw_lookup(Lookup&);

};
