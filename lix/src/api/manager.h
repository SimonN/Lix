/*
 * api/manager.h
 *
 * A window/element manager
 *
 * This manages the OSD that everything gets drawn onto.
 *
 * An element should be registered here as an "elder" if it doesn't have
 * a parent. All elders will get calc'd and drawn by the manager.
 *
 * If std::vector <Element*> focus is nonempty, calc'ing and drawing orders
 * to the manager will only calc or draw the _last_ element. The usage of
 * the focus queue is like this: If you spawn a new window that shall be
 * worked on first, you set it to have focus.
 *
 * I must decide what the best way is to give and take focus.
 *
 */

#pragma once

#include <set>
#include <vector>

#include "../graphic/torbit.h"

namespace Api {

class Element;

class Manager {

public:

           static void initialize(int, int); // generates given-sized Torbit
           static void deinitialize();       // destroys the Torbit

           static void add_elder   (Element* e);
           static void remove_elder(Element* e);

           static void add_focus   (Element*); // if already included, move to back
           static void remove_focus(Element*); // removes it from anywhere

    inline static const std::set    <Element*>& get_elders() { return elders; }
    inline static const std::vector <Element*>& get_focui () { return focus;  }
    inline static       Torbit&                 get_torbit() { return *torbit;}
           static const Element*                get_focus ();

           static void calc();
           static void draw();
           static void draw_to_pre_screen(); // after mouse etc. has been done

private:

    Manager();
    void operator = (const Manager&);

    static Torbit*                torbit;
    static bool                   clear_next_draw;

    static std::set    <Element*> elders;
    static std::vector <Element*> focus;

};  // end class

}   // end namespace
