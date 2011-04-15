#include "manager.h"
#include "element.h"

#include "../graphic/glob_gfx.h"

namespace Api {

Torbit*                Manager::torbit(0);
std::set <Element*>    Manager::elders;
std::vector <Element*> Manager::focus;
bool                   Manager::clear_next_draw(false);



void Manager::initialize(int xl, int yl)
{
    if (torbit) return;
    torbit = new Torbit(xl, yl);
    torbit->clear_to_color(::color[COL_PINK]);
    clear_next_draw = false;
}



void Manager::deinitialize()
{
    if (!torbit) return;
    delete torbit;
}



void Manager::add_elder(Element* e)
{
    elders.insert(e);
}



void Manager::remove_elder(Element* e)
{
    if (elders.erase(e)) clear_next_draw = true;
}



void Manager::add_focus(Element* e)
{
    // Erase all instances (should be at most one) of &e in the queue
    for (std::vector <Element*> ::iterator itr = focus.begin();
     itr != focus.end(); ++itr) if (*itr == e) focus.erase(itr--);
    focus.push_back(e);
}



void Manager::remove_focus(Element* e)
{
    for (std::vector <Element*> ::iterator itr = focus.begin();
     itr != focus.end(); ++itr) if (*itr == e) {
        focus.erase(itr--);
        clear_next_draw = true;
    }
}



const Element* Manager::get_focus()
{
    return focus.empty() ? 0 : focus.back();
}



void Manager::calc()
{
    // We do it a bit complicated here. The idea is to calc the focus,
    // and the rest only if nothing is focussed. But this leads to dialogue
    // leftovers. (Messagebox unfocuses, spawner gets drawn only partially
    // on its place, then its spawner deletes a file which takes long,
    // then it redraws itself.)
    // The solution here calcs until the focus list hasn't changed in size.
//    size_t stop_at_size = 0;
//    while (!focus.empty() && focus.size() != stop_at_size) {
//        stop_at_size = focus.size();
//        focus.back()->calc();
//    }
    if (!focus.empty()) focus.back()->calc();
    else {
        for (std::set <Element*> ::iterator itr = elders.begin();
         itr != elders.end(); ++itr) (**itr).calc();
    }
    for (std::set <Element*> ::iterator itr = elders.begin();
     itr != elders.end(); ++itr) (**itr).work();
    for (std::vector <Element*> ::iterator itr = focus.begin();
     itr != focus.end(); ++itr) (**itr).work();
}



void Manager::draw()
{
    if (clear_next_draw) {
        clear_next_draw = false;

        torbit->clear_to_color(::color[COL_PINK]);

        for (std::set <Element*> ::iterator itr = elders.begin();
         itr != elders.end(); ++itr) (**itr).set_draw_required();
        for (std::vector <Element*> ::iterator itr = focus.begin();
         itr != focus.end(); ++itr) (**itr).set_draw_required();
    }
    for (std::set <Element*> ::iterator itr = elders.begin();
     itr != elders.end(); ++itr) (**itr).draw();
    for (std::vector <Element*> ::iterator itr = focus.begin();
     itr != focus.end(); ++itr) (**itr).draw();
}



void Manager::draw_to_pre_screen()
{
    torbit->draw(*pre_screen);
}

}
// end of namespace
