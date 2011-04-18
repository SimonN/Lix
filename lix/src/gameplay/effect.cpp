/*
 * gameplay/effect.cpp
 *
 */

#include <sstream>

#include "effect.h"

#include "../graphic/gra_lib.h"
#include "../other/console.h"
#include "../other/globals.h"
#include "../other/user.h"
#include "../other/language.h"

const int EffectManager::max_updates_history = 15 * 10;
const int EffectManager::pickaxe_y_frame     =       0;

EffectManager::EffectManager(Map& m)
:
    map     (m),
    trlo    (0),
    timer_ticks_for_explosion(Help::timer_ticks_per_second / 60),
    timer_tick_last_explosion(Help::timer_ticks),
    overtime(0)
{
}



EffectManager::~EffectManager()
{
}



void EffectManager::add_arrow
(const unsigned u, const Tribe& t, const unsigned l , const Arrow& a)
{
    Effect effect(u, &t, l);
    if (arrow.find(effect) == arrow.end()) {
        arrow.insert(effect);
        obj_arrow.push_back(a);
    }
}



void EffectManager::add_explosion
(const unsigned u, const Tribe& t, const unsigned l,
 const int      x, const int y)
{
    Effect effect(u, &t, l);
    if (explosion.find(effect) == explosion.end()) {
        explosion.insert(effect);
        Graphic e(GraLib::get(gloB->file_bitmap_explosion), map);
        e.set_x(x - e.get_xl() / 2);
        e.set_y(y - e.get_yl() / 2);
        obj_explosion.push_back(e);

        // Haengt von der Option mit den Sternen ab.
        // Je nach Anzahl der bereits vorhandenen Truemmer im Spiel weniger
        // neue Sternchen, Rauchwoelkchen, ... erzeugen.
        if (useR->debris_amount == 2
         ||(useR->debris_amount == 1 && &t == trlo)) {
            int count = 40;
            count -= obj_debris.size() / 13;
            if (useR->debris_type == 1) count = count * 2;
            for (int i = 0; i < count; ++i)
             obj_debris.push_back(
             useR->debris_type == 0 ? Debris(map, x, y)
                                            : Debris(map, x, y, t.style));
        }
    }
}



void EffectManager::add_pickaxe
(const unsigned u, const Tribe& t, const unsigned l,
 const int      x, const int    y, const int rotation)
{
    Effect effect(u, &t, l);
    if (debris.find(effect) == debris.end()) {
        debris.insert(effect);
        obj_debris.push_back(Debris(map, x, y, pickaxe_y_frame, rotation));
    }
}



void EffectManager::add_sound_general(
    const unsigned  u,
    const Sound::Id id
) {
    Effect effect(u, 0, static_cast <unsigned> (id));

    if (sound.find(effect) == sound.end()) {
        sound.insert(effect);
        Sound::play_loud(id);
    }
}



void EffectManager::add_sound(
    const unsigned  u,
    const Tribe&    t,
    const unsigned  l,
    const Sound::Id id
) {
    Effect effect(u, &t, l);

    if (sound.find(effect) == sound.end()) {
        sound.insert(effect);
        Sound::play_loud_if(id, &t == trlo);
    }
}



void EffectManager::add_sound_quiet(
    const unsigned  u,
    const Tribe&    t,
    const unsigned  l,
    const Sound::Id id
) {
    Effect effect(u, &t, l);

    if (sound.find(effect) == sound.end()) {
        sound.insert(effect);
        Sound::play_quiet(id);
    }
}



void EffectManager::add_sound_if_trlo(
    const unsigned  u,
    const Tribe&    t,
    const unsigned  l,
    const Sound::Id id
) {
    Effect effect(u, &t, l);

    if (&t == trlo && sound.find(effect) == sound.end()) {
        sound.insert(effect);
        Sound::play_loud(id);
    }
}



void EffectManager::add_overtime
(const unsigned u, const Tribe& t, const unsigned how_much)
{
    if (overtime == 0) {
        overtime = u;

        // Erste Zeile mit Namen und Ergebnis
        std::ostringstream s;
        s << t.get_name() << ' ' << Language::net_game_overtime_1 << ' ';
        if (t.lix_saved == 1) s << Language::net_game_overtime_2_one;
        else {
            s << t.lix_saved;
            s << ' ' << Language::net_game_overtime_2;
        }
        // Nachricht nur lokal anzeigen, nicht verschicken
        Console::push_back(s.str());

        // Zweite Zeile mit Ankuendigung der Nachspielzeit
        const unsigned ups = gloB->updates_per_second;
        std::ostringstream o;
        o << Language::net_game_overtime_3
         << ' ' << how_much / ups / 60
         << ':' << how_much / ups % 60 / 10 // to get leading zero
         <<        how_much / ups % 10
         << ' ';
        if      (how_much >  ups * 60) o << Language::net_game_overtime_4;
        else if (how_much == ups * 60) o << Language::net_game_overtime_4_one;
        else                           o << Language::net_game_overtime_4_sec;
        Console::push_back(o.str());

        Sound::play_loud(Sound::OVERTIME);
    }
}



void EffectManager::delete_after(const unsigned u)
{
    std::set <Effect> ::iterator itr;
    for (itr = arrow.begin(); itr != arrow.end(); ) {
        if (itr->update > u) arrow.erase(itr++);
        else ++itr;
    }
    for (itr = explosion.begin(); itr != explosion.end(); ) {
        if (itr->update > u) explosion.erase(itr++);
        else ++itr;
    }
    for (itr = sound.begin(); itr != sound.end(); ) {
        if (itr->update > u) sound.erase(itr++);
        else ++itr;
    }
    if (overtime > u) overtime = 0;
}



void EffectManager::delete_before(
    std::set <Effect>& efset,
    const unsigned           when
) {
    for (std::set <Effect> ::iterator
     itr = efset.begin(); itr != efset.end(); ++itr) {
        // Erstes Element, das noch jung genug ist: Loesche alle davor.
        if (itr->update >= when) {
            efset.erase(efset.begin(), itr);
            break;
        }
    }
}



void EffectManager::calc(const unsigned current_update)
{
    // Effektspeicherungen: Alte Speicherungen entfernen
    if (current_update > (unsigned) max_updates_history) {
        delete_before(arrow,     current_update - max_updates_history);
        delete_before(explosion, current_update - max_updates_history);
        delete_before(debris,    current_update - max_updates_history);
        delete_before(sound,     current_update - max_updates_history);
    }

    // Pfeile
    for (std::list <Arrow> ::iterator itr = obj_arrow.begin();
     itr != obj_arrow.end(); ++itr) {
        // Pfeil zerstoeren?
        if (itr->age >= 60) obj_arrow.erase(itr--);
        else {
            // Pfeil nach oben bewegen
            int up = 0;
            switch (itr->age - 2) {
            case -2:                   up = 8; break;
            case -1:                   up = 4; break;
            case  0:                   up = 2; break;
            case  1: case  2: case  4:
            case  8: case 16:          up = 1; break;
            }
            itr->set_y(itr->get_y() - up);
            itr->age += 1;
        }
    }

    // Explosionen
    if (Help::timer_ticks - timer_ticks_for_explosion
     >= timer_tick_last_explosion) {
        timer_tick_last_explosion = Help::timer_ticks;
        const int anim_speed = useR->debris_type == 0 ? 1 : 2;
        for (std::list <Graphic> ::iterator
         itr = obj_explosion.begin(); itr != obj_explosion.end(); ++itr) {
            itr->set_x_frame(itr->get_x_frame() + anim_speed);
            if (itr->get_x_frame() >= itr->get_x_frames()) {
                std::list <Graphic> ::iterator j = itr;
                --itr;
                obj_explosion.erase(j);
            }
        }
    }

    // Truemmer
    for (std::list <Debris> ::iterator itr = obj_debris.begin();
     itr != obj_debris.end(); ++itr) {
        if (itr->get_ttl() <= 0) {
            std::list <Debris> ::iterator j = itr;
            --itr;
            obj_debris.erase(j);
        }
        else {
            itr->calc();
        }
    }

}



int  EffectManager::get_effects()
{
    return obj_arrow    .size()
         + obj_explosion.size()
         + obj_debris   .size();
}



void EffectManager::draw()
{
    for (std::list <Arrow>   ::iterator
     itr =  obj_arrow    .begin();
     itr != obj_arrow    .end(); ++itr) itr->draw();
    for (std::list <Graphic> ::iterator
     itr =  obj_explosion.begin();
     itr != obj_explosion.end(); ++itr) itr->draw();
    for (std::list <Debris> ::iterator
     itr =  obj_debris   .begin();
     itr != obj_debris   .end(); ++itr) itr->draw();
}



void EffectManager::clear_all_lists()
{
    obj_arrow    .clear();
    obj_explosion.clear();
    obj_debris   .clear();
}
