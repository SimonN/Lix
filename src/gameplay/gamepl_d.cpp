/*
 * gameplay/gamepl_d.cpp
 *
 * Entzeichnen und Zeichnen inklusive Subfunktionen
 *
 */

#include <cmath> // std::sin fuer Replay-Zeichen

#include "gameplay.h"

#include "../api/manager.h"

void Gameplay::undraw()
{
    mouse_cursor.undraw();
    replay_sign .undraw();
}



// ############################################################################
// ############################################################################
// ############################################################################



void Gameplay::draw() {
    map.clear_screen_rectangle(bg_color);

    // Zuerst die interaktiven Objekte. Wir zeichnen Listen mit allen Indizies,
    // auch wenn die Listen von TERRAIN oder GOAL natuerlich immer leer sind.
    // Debugging: We draw gloals below deco if we're in singleplayer, and
    // vice-versa in multiplayer. This is because we want the ownership
    // to show on top of the decoration, but Frapela's windmill wings (deco)
    // top of the windmill goals. Should probably be done better, with the
    // ownerships as decoration as well (topmost of all decos) maybe?
    for (HatchIt i = hatches.begin(); i != hatches.end(); ++i)
     if (!i->get_has_style()) i->draw();
    for (HatchIt i = hatches.begin(); i != hatches.end(); ++i)
     if ( i->get_has_style()) i->draw();

    if (! multiplayer)
     for (Goal::It i = goal.begin(); i != goal.end(); ++i) i->draw(false);
    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (IacIt i =  special[Object::perm(type)].begin();
                i != special[Object::perm(type)].end(); ++i) i->draw();
    if (multiplayer)
     for (Goal::It i = goal.begin(); i != goal.end();     ++i) i->draw(true);
    for (IacIt i = cs.trap.begin();  i != cs.trap.end();  ++i) i->draw();
    for (IacIt i = cs.fling.begin(); i != cs.fling.end(); ++i) i->draw();

    map.load_masked_screen_rectangle(cs.land);

    effect.draw();

    // Erst die entfernten Spieler zeichnen, dann den lokalen.
    // Erst die Unmarkierten zeichnen, dann die Arbeitstiere
    // Markierungen bleiben erhalten bis zum naechsten Update.
    for (Tribe::It t = --cs.tribes.end(); t != --cs.tribes.begin(); --t) {
        if (&*t == trlo) continue;
        for (LixIt i = --t->lixvec.end(); i != --t->lixvec.begin(); --i)
         if (!i->get_mark()) i->draw();
        for (LixIt i = --t->lixvec.end(); i != --t->lixvec.begin(); --i)
         if ( i->get_mark()) i->draw();
    }
    if (trlo) {
        for (LixIt i = --trlo->lixvec.end(); i != --trlo->lixvec.begin(); --i)
         if (!i->get_mark()) i->draw();
        for (LixIt i = --trlo->lixvec.end(); i != --trlo->lixvec.begin(); --i)
         if ( i->get_mark()) i->draw();
    }

    // Draw chat behind the replay sign
    chat.draw();

    // OSD: Replay-Zeichen
    if (replaying && ! multiplayer) {
        replay_sign.set_y((int) (5 + 5 * -std::sin(local_ticks * 0.07)));
        replay_sign.draw();
    }

    // Statusanzeige
    if (cs.clock > 0) pan.stats.set_show_clock();
    pan.stats.set_clock(cs.clock);
    pan.draw();

    // Mauszeiger ganz obenauf
    mouse_cursor.set_x(hardware.get_mx()-mouse_cursor_offset);
    mouse_cursor.set_y(hardware.get_my()-mouse_cursor_offset);
    mouse_cursor.draw();

    map. draw(*pre_screen);
    Api::Manager::draw_to_pre_screen();

    blit_to_screen(pre_screen->get_al_bitmap());
}
// Ende draw()
