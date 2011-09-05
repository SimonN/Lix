/*
 * lix/lix_ac.cpp
 *
 * Auslagerung aller faehigkeitslastigen Memberfunktionen
 *
 */

#include "lix.h"

#include "../gameplay/gameplay.h" // Abstand fuer Blocker zum Ziel
#include "../gameplay/goal.h"     // Ebenso Blockerabstand
#include "../other/user.h"

unsigned Lixxie::get_priority(
    LixEn::Ac  new_ac,
    int        tribes_active,
    const bool personal
) {
    // Argument:  Die Faehigkeit, die beim Klick gegeben wuerde.
    // Rueckgabe: 0, falls nicht anklickbar und Cursor geschlossen.
    //            1, falls nicht anklickbar und Cursor dennoch offen.
    //            2 bis 99.998, falls anklickbar. Hoehere Zahl = hoehere Prior.

    unsigned p = 0;

    // Nothing allowed at all, don't even open the cursor
    if (ac == LixEn::NOTHING || ac_func[ac].leaving) return 0;

    // Permanent skills
    if ((new_ac == LixEn::EXPLODER  && updates_since_bomb > 0)
     || (new_ac == LixEn::EXPLODER2 && updates_since_bomb > 0)
     || (new_ac == LixEn::RUNNER    && runner)
     || (new_ac == LixEn::CLIMBER   && climber)
     || (new_ac == LixEn::FLOATER   && floater) ) return 1;

    // Don't plant blockers close to exits in multiplayer
    if (new_ac == LixEn::BLOCKER && tribes_active > 1
        && (enc_foot & Lookup::bit_goal_prox))
        return 1;

    switch (ac) {
        // Nur Exploder ist erlaubt, und zwar mit wesentlich hoeherer Prior.!
        // Falls der Walker zuweisbar ist, dann ist er es ebenfalls Blockern.
        case LixEn::BLOCKER:
            if (new_ac == LixEn::WALKER
             || new_ac == LixEn::EXPLODER
             || new_ac == LixEn::EXPLODER2) p = 5000;
            else return 1;
            break;

        // Stunners may be turned in their later frames, but otherwise act just
        // like regular mostly unassignable-to acitivities (case falls through)
        case LixEn::STUNNER:
            if (get_frame() >= 16) {
                p = 3000;
                break;
            }
            // falls through if frame is less

        // Sonstige vereinnahmende Aktivitaeten, die die Lix mehr oder
        // weniger willentlich in Komplettbeschlag nehmen:
        // Bleibende Fähigkeiten und Exploder sind immer erlaubt
        case LixEn::FALLER:
        case LixEn::TUMBLER:

        case LixEn::ASCENDER:
        case LixEn::CLIMBER:
        case LixEn::FLOATER:
        case LixEn::JUMPER:
            if (new_ac == LixEn::RUNNER
             || new_ac == LixEn::CLIMBER
             || new_ac == LixEn::FLOATER
             || new_ac == LixEn::EXPLODER
             || new_ac == LixEn::EXPLODER2) p = 2000;
            else return 1;
            break;

        // Standard activities, not considered working lixes
        case LixEn::WALKER:
        case LixEn::LANDER:
        case LixEn::RUNNER:
            p = 3000;
            break;

        // Builder kann selbstzuweisen, Ausnahme vom Default-Wert erforderlich
        // Mit verneinter Einstellung oder im Mehrspielermodus
        // wird's allerdings wird's wie Default behandelt.
        case LixEn::BUILDER:
        case LixEn::PLATFORMER:
            if      (new_ac == ac
                && (!personal || useR->multiple_builders)) p = 1000;
            else if (new_ac != ac)                         p = 4000;
            else                                           return 1;
            break;

        // Usually, anything different from the current activity can be assign.
        default:
            if (new_ac != ac) p = 4000;
            else return 1;

    }
    p += (new_ac == LixEn::BATTER && useR->batter_priority
          ? -updates_since_bomb : updates_since_bomb);
    p += 400 * runner + 200 * climber + 100 * floater;
    return p;
}



void Lixxie::evaluate_click(LixEn::Ac new_ac)
{
    assclk(new_ac);
}
