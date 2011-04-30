/*
 * gameplay/goal.h
 *
 * Ein Ziel ist ein Grafikobjekt, das im Mehrspielermodus zusaetzlich die
 * Farben der Spieler zeigt, denen eintretende Lemminge gutgeschrieben werden.
 *
 * Ferner bietet diese Klasse, aehnlich wie die anderen interaktiven Objekte,
 * eine Update-Methode zum Wechseln auf das naechste Frame.
 *
 * void draw(bool owners = false)
 *
 *   Wenn owners = true, dann werden die Besitzerfarben des Ausgangs mit-
 *   gezeichnet - dies macht nur Sinn bei mehreren Spielern.
 *
 */

#pragma once

#include <vector>

#include "tribe.h"

#include "../editor/graph_ed.h"

class Goal : public EdGraphic {

private:

    std::vector <const Tribe*> tribes;

public:

    typedef std::vector <Goal> ::iterator         It;
    typedef std::vector <Goal> ::reverse_iterator RIt;
    typedef std::vector <Goal> ::const_iterator   CIt;
    typedef std::vector <const Tribe*>            TribeVec;

             Goal(Torbit&, const Object*, const int = 0, const int = 0);
    virtual ~Goal();

    bool                   has_tribe (const Tribe* t) const;
    inline const TribeVec& get_tribes() const         { return tribes;       }
    inline void            add_tribe (const Tribe* t) { tribes.push_back(t); }

    virtual void draw(bool = false);

};
