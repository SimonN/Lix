/*
 * lemming/walker.cpp
 *
 * Der Walker nutzt auch viel den Ascender zum Erklimmen von hoehren Stufen,
 * der entsprechende Code steht in climber.cpp.
 *
 */

#include "ac.h"

void assign_walker(Lixxie& l)
{
    l.set_special_y(0);
    l.set_special_x(0);
    if (l.get_ac() == LixEn::WALKER
     || l.get_ac() == LixEn::LANDER) {
        l.turn();
        // Da bei Walker -> Walker nicht --frame von evaluate_click() kommt,
        // setzen wir hier manuell das Frame auf -1, weil wir das 0. wollen.
        if (l.get_ac() == LixEn::WALKER) l.set_frame(-1);
    }
    else if (l.get_ac() == LixEn::BLOCKER && l.get_frame() < 20) {
        // Da assign haeufig beim Mausklick-Zuweisen aufgerufen wird, gilt
        // wieder die Konvention, dass --frame hinterher gemacht wird, also:
        l.set_frame(21);
    }
    else if (l.get_ac() == LixEn::PLATFORMER && l.get_frame() > 5) {
        l.set_ac(LixEn::SHRUGGER2);
        l.set_frame(9);
    }
    else {
        l.set_ac(LixEn::WALKER);
        l.set_frame(0);
    }
}



void update_walker(Lixxie& l, const UpdateArgs& ua)
{
    ua.suppress_unused_variable_warning();

    // Das erste Frame dient zur kurzen Pause, die der Lemming vor dem
    // Weiterlaufen machen soll, wenn die Walker-Faehigkeit vom Benutzer
    // explizit zugewiesen wird. Dieses Frame darf im normalen Walker-
    // Framezyklus nicht angenommen werdent.
    if (l.is_last_frame())  l.set_frame(3);
    else                    l.next_frame();
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
        int up_by_what1 = l.solid_wall_height(0);
        int up_by_what2 = l.solid_wall_height(0);
        if (up_by_what1 == 13 && up_by_what2 == 13) turn_after_all = true;
        // ...sonst sich nach oben bewegen...
        else {
            int u;
            (up_by_what1 > up_by_what2) ? u = up_by_what2 : u = up_by_what1;
            l.move_up(u);
            // ...und ggf. kurz anhalten, um Höhen ab 7 Pixeln zu klettern
            if (up_by_what1 > 6 || up_by_what2 > 6) {
                l.assign(LixEn::ASCENDER);
                // Frame wählen und hinunter bewegen, denn der Ascender
                // hängt ja zunächst in der Wand
                l.set_frame(12-u);
                if (!l.is_solid(2, -2)) l.move_ahead();
                // Höhe dem Frame angleicihen
                l.move_down(2*u - 12);
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
            if (l.solid_wall_height(-1) == 11 ||
                l.solid_wall_height( 0) == 11) {
                turn_after_all = true;
            }
        }
        else {
            // Aber nicht direkt so weit nach unten zum Fallen bewegen
            l.move_up(6);
            l.assign(LixEn::FALLER);
            l.set_special_x(2);
        }
    }

    // Wenn der Lemming umdrehen soll, beginnt er entweder zu klettern
    // oder er dreht um, beides auf seiner alten Stelle
    if (turn_after_all) {
        l.set_ex(old_ex);
        l.set_ey(old_ey);

        if (l.get_climber()) {
            // Auf Landschaft über der derzeitigen Position prüfen
            bool enough_space = true;
            for (int i = 1; i < 13; ++i) {
                if (l.is_solid(-2, -i)) {
                    enough_space = false;
                    break;
                }
            }
            if (enough_space) l.assign(LixEn::CLIMBER);
        }
        if (l.get_ac() == LixEn::WALKER) {
            l.turn();
            l.move_ahead();
        }
    }
    // Ende der Umdrehen-Kontrolle
}
