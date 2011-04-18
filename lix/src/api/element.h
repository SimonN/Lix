/*
 * element.h
 *
 * class Element
 *
 *   Grundlegende Eigenschaften eines Fensterelements sind gesammelt in dieser
 *   Basisklasse. Alle anderen Elemente sind hiervon abgeleitet mit
 *   "class SpeziellesElement : public Element".
 *
 * int undraw_color
 *
 *   Wenn ein Element entzeichnet wird, so malt es einen rechteckigen Kasten
 *   auf seine Stelle im Bitmap "ground". Wenn undraw_color == 0 ist, wird
 *   dieser Schritt uebersprungen und undraw() setzt nur ein paar wenige
 *   Variablen zurueck. 0 ist Standardeinstellung, nicht color[COL_PINK]!
 *
 * std::vector <Element*> children
 *
 *   These objects will be drawn and calcd (with *this's x, y).
 *   If something is hidden, it won't get drawn, calcd or hidden.
 *
 */

#pragma once

#include "../other/myalleg.h" // Die Api zeichnet eigenständig

#include <set> // children

#include "../other/hardware.h" // Maussteuerung

namespace Api {
class Element {

private:

    Torbit* ground;

    int x;
    int y;

    int xl;
    int yl;

    bool hidden;
    bool drawn;
    bool draw_required;

    int  undraw_color;

    Element*            parent; // for positioning of the mouse
    std::set <Element*> children;

public:

    typedef std::set <Element*> ::const_iterator ElIt;

    //               x        y        Breite   Länge
             Element(int = 0, int = 0, int = 0, int = 0);
    virtual ~Element();

    Torbit&      get_ground() const               { return *ground;       }

           int   get_x_here() const; // loops back through the parents,
           int   get_y_here() const; // adds their x/y pos, like is_mouse_here
    inline int   get_x() const                    { return x;             }
    inline int   get_y() const                    { return y;             }
    virtual void set_x(const int);
    virtual void set_y(const int);

    inline int   get_xl() const                   { return xl;            }
    inline int   get_yl() const                   { return yl;            }
    virtual void set_xl(const unsigned);
    virtual void set_yl(const unsigned);

    inline int   get_hidden() const               { return hidden;        }
    inline void  hide      (const bool b = true)  { set_hidden(b);        }
    inline void  show      (const bool b = true)  { set_hidden(!b);       }
           void  set_hidden(const bool b = true);

    inline bool  is_drawn() const                 { return drawn;         }

    inline bool  get_draw_required() const        { return draw_required; }
           void  set_draw_required(bool = true);

    // Hintergrundfarbe beim Entzeichnen gewuenscht? (siehe Headerkommentar)
    inline int   get_undraw_color() const { return undraw_color; }
    inline void  set_undraw_color(int i)  { undraw_color = i;    }

    bool         is_mouse_here() const;

           void  add_child   (Element&);
           void  remove_child(Element&);
           void  clear_children();
           void  hide_all_children(bool = true);

    // You will be killed if you override these methods!
    // Override calc_self() etc. instead.
    // draw() undraws *this and children if drawn && hidden.
    // work() is supposed to not process any keystrokes, so that even something
    // unfocussed gets updated besides drawing (because drawing doesn't take
    // place all the time, and happens after the end of the LMain-calc).
    // The order is calc -> work -> draw.
                   void calc(); // calcs all children, then this.
                   void work(); // works all ch., then this.
                   void draw(); // draws this, then all children.

protected:

    inline virtual void calc_self() {}
    inline virtual void work_self() {}
    inline virtual void draw_self() {}
           virtual void undraw_self();

    inline std::set <Element*>& get_children() { return children; }

};
}
