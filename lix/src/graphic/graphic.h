/*
 * graphic.h
 *
 * A simple Graphic object. This can be placed somewhere on a torbit and
 * draws a cutbit to that torbit.
 *
 * This class is not about graphic sets from L1, ONML etc.
 * See level/gra_set.h for that.
 *
 */

#pragma once

#include <string>

#include "glob_gfx.h"
#include "torbit.h"

class Graphic {

private:

    const Cutbit* cutbit;
    Torbit&       ground;

    int x;
    int y;
    bool         mirror;
    double       rotation;
    Cutbit::Mode mode;

    int x_frame;
    int y_frame;

    void draw_at  (const int, const int);

public:

    Graphic(const Cutbit&, Torbit&, const int = 0, const int = 0);
    virtual ~Graphic();

    Graphic& operator = (const Graphic&);

    inline const Cutbit* get_cutbit() const          { return cutbit; }
    inline void          set_cutbit(const Cutbit& c) { cutbit = &c;   }

    inline BITMAP* get_al_bitmap()  { return cutbit->get_al_bitmap(); }
    inline Torbit& get_ground()                    { return ground;   }
    inline void    set_ground(Torbit& gr)          { ground = gr;     }

    inline         int  get_x() const                        { return x;     }
    inline         int  get_y() const                        { return y;     }
           virtual void set_x (const int);
           virtual void set_y (const int);
    inline         void set_xy(int nx, int ny)       { set_x(nx); set_y(ny); }

    inline bool         get_mirror  () const              { return mirror;   }
    inline double       get_rotation() const              { return rotation; }
    inline Cutbit::Mode get_mode    () const              { return mode;     }
    inline virtual void set_mode    (Cutbit::Mode m)      { mode   = m;      }
    inline virtual void set_mirror  (const bool b = true) { mirror = b;      }
           virtual void set_rotation(const double);

    inline int  get_x_frame() const { return x_frame; }
    inline int  get_y_frame() const { return y_frame; }
    inline void set_x_frame(const int new_x_frame) { x_frame = new_x_frame; }
    inline void set_y_frame(const int new_y_frame) { y_frame = new_y_frame; }

    // Wrapper functions, these return things from the Cutbit class
    // If the Graphic object is rotated, get_xl()/yl() are NOT wrappers,
    // but rotate with the Graphic object before they access the Cutbit part.
    // Same thing with get_pixel().
    virtual int  get_xl() const;
    virtual int  get_yl() const;
    inline  int  get_x_frames()  const { return cutbit->get_x_frames(); }
    inline  int  get_y_frames()  const { return cutbit->get_y_frames(); }
    virtual bool is_last_frame() const;
    virtual bool get_frame_exists(const int, const int) const;
    virtual int  get_pixel       (const int, const int) const;

   virtual void draw  ();

};
