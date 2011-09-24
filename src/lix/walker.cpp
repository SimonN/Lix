/*
 * lix/walker.cpp
 *
 * Der Walker nutzt auch viel den Ascender zum Erklimmen von hoehren Stufen,
 * der entsprechende Code steht in climber.cpp.
 *
 */

#include "ac.h"

void assclk_walker(Lixxie& l)
{
    l.set_special_y(0);
    l.set_special_x(0);
    if (l.get_ac() == LixEn::WALKER
     || l.get_ac() == LixEn::RUNNER
     || l.get_ac() == LixEn::LANDER) {
        l.turn();
        // Da bei Walker -> Walker nicht --frame von evaluate_click() kommt,
        // setzen wir hier manuell das Frame auf -1, weil wir das 0. wollen.
        if (l.get_ac() == LixEn::WALKER) l.set_frame(-1);
        if (l.get_ac() == LixEn::RUNNER) l.set_frame(-1);
    }
    else if (l.get_ac() == LixEn::STUNNER
          || l.get_ac() == LixEn::ASCENDER) {
        // lix_ac.cpp only allows to get here when the frame is high enough
        l.become(LixEn::WALKER);
        l.turn();
    }
    else if (l.get_ac() == LixEn::BLOCKER) {
        // Da assign haeufig beim Mausklick-Zuweisen aufgerufen wird, gilt
        // wieder die Konvention, dass --frame hinterher gemacht wird, also:
        if (l.get_frame() < 20) l.set_frame(21);
        else                    l.turn(); // turn a blocker->walker transistion
    }
    else if (l.get_ac() == LixEn::PLATFORMER && l.get_frame() > 5) {
        l.set_ac(LixEn::SHRUGGER2);
        l.set_frame(9);
        // see also next else if. Clicking twice on the platformer shall turn
        // it around.
    }
    else if (l.get_ac() == LixEn::SHRUGGER || l.get_ac() == LixEn::SHRUGGER2) {
        l.become(LixEn::WALKER);
        l.turn();
    }
    else {
        l.become(LixEn::WALKER);
    }
}



void become_walker(Lixxie& l)
{
    if (l.get_runner()) l.become        (LixEn::RUNNER);
    else                l.become_default(LixEn::WALKER);
}



void update_walker_or_runner(Lixxie& l, const UpdateArgs& ua);

void update_walker(Lixxie& l, const UpdateArgs& ua)
{
    if (l.is_last_frame())  l.set_frame(3);
    else                    l.next_frame();

    update_walker_or_runner(l, ua);
}



void update_walker_or_runner(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Das erste Frame dient zur kurzen Pause, die die Lix vor dem
    // Weiterlaufen machen soll, wenn die Walker-Faehigkeit vom Benutzer
    // explizit zugewiesen wird. Dieses Frame darf im normalen Walker-
    // Framezyklus nicht angenommen werden.
    // Also true for runners.
    if (l.get_frame() != 0) l.move_ahead();

    bool turn_after_all   = false;
    int  old_ex           = l.get_ex();
    int  old_ey           = l.get_ey();

    // Pruefung auf Boden unter der neuen Position
    // Falls da nichts ist, gucken wir etwas darüber nach - vielleicht
    // handelt es sich um eine sehr dünne Aufwärts-Brücke, durch die
    // wir nicht hindurchfallen wollen?
    if (l.is_solid() || l.is_solid(0, 1)) {
        // Bei zu starker Steigung umdrehen...
        int up_by = l.solid_wall_height(0);
        if (up_by == 13) {
            turn_after_all = true;
        }
        // ...sonst sich nach oben bewegen...
        else {
            l.move_up(up_by);
            // ...und ggf. kurz anhalten, um Höhen ab 6 Pixeln zu klettern
            if (up_by >= 6) {
                l.become(LixEn::ASCENDER);
                // Frame wählen und hinunter bewegen, denn der Ascender
                // hängt ja zunächst in der Wand
                l.set_frame(up_by >= 10 ? 12 - up_by : 11 - up_by);
                if (!l.is_solid(2, -2)) l.move_ahead();
                // Höhe dem Frame angleichen
                l.move_down(10 - 2 * l.get_frame());
            }
            // Ende vom eventuellen Ascender
        }
        // Ende von Bewegung nach oben
    }
    // Ende von "Boden unter den Füßen"

    // Kein Boden? Dann hinunter gehen oder zu fallen beginnen
    else {
        int moved_down_by = 0;
        for (int i = 3; i < 11; ++i) {
            if (!l.is_solid()) {
                l.move_down(1);
                ++moved_down_by;
            }
            else break;
        }
        if (l.is_solid()) {
            // Bei zu starker Steigung umdrehen
            if (l.solid_wall_height(0) == 11) {
                turn_after_all = true;
            }
            // Don't move that far back up as the check about 10 lines further
            // down that reads very similar in its block
            else if (moved_down_by > 6) {
                l.move_up(4);
                l.become(LixEn::FALLER);
                l.set_special_x(moved_down_by - 4);
            }
        }
        else {
            // Aber nicht direkt so weit nach unten zum Fallen bewegen
            l.move_up(6);
            l.become(LixEn::FALLER);
            l.set_special_x(2);
        }
    }

    // Wenn die Lix umdrehen soll, beginnt sie entweder zu klettern
    // oder dreht um, beides auf seiner alten Stelle
    if (turn_after_all) {
        l.set_ex(old_ex);
        l.set_ey(old_ey);
        bool climbed_after_all = false;

        if (l.get_climber()) {
            // Auf Landschaft über der derzeitigen Position prüfen
            bool enough_space = true;
            for (int i = 1; i < 13; ++i) {
                if (l.is_solid(-2, -i)) {
                    enough_space = false;
                    break;
                }
            }
            if (enough_space) {
                l.become(LixEn::CLIMBER);
                climbed_after_all = true;
            }
        }
        if (! climbed_after_all) {
            l.turn();
            l.move_ahead();
        }
    }
    // Ende der Umdrehen-Kontrolle
}
