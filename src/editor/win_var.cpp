/*
 * varwin.cpp
 *
 */

#include "win_var.h"

#include "../other/language.h"
#include "../other/user.h"

namespace Api{

const int WindowVariables::this_xl(480);
const int WindowVariables::this_yl(260);

WindowVariables::WindowVariables(Level& l)
:
    Window       (LEMSCR_X/2 - this_xl/2,
                  LEMSCR_Y/2 - this_yl/2 - 30,
                   this_xl, this_yl, Language::win_var_title),
    author       (160,  40, 300,   l.author),
    name_german  (160,  70, 300,   l.name_german),
    name_english (160, 100, 300,   l.name_english),

    initial      (160, 130, 180, 3, 1, 999,   l.initial,  true),
    required     (160, 160, 180, 3, 1, 999,   l.required, true),
    spawnint     (180, 190, 140, 2, Level::spawnint_min,
                                    Level::spawnint_max,  l.spawnint),
    seconds      (160, 220, 180, 5, 0, 30*60, l.seconds,  true),

    ok           (360, this_yl - 70, 100, 20),
    cancel       (360, this_yl - 40, 100, 20),

    desc_author  (20, author.get_y(), Language::win_var_author),
    desc_german  (20, name_german.get_y(), Language::win_var_name_german),
    desc_english (20, name_english.get_y(), Language::win_var_name_english),
    desc_initial (20, initial.get_y(), Language::win_var_lems),
    desc_required(20, required.get_y(), Language::win_var_required),
    desc_spawnint(20, spawnint.get_y(), Language::win_var_spawnint),
    desc_clock   (20, seconds.get_y(), Language::win_var_clock)
{
    level = &l;

    ok     .set_hotkey(useR->key_me_okay);
    cancel .set_hotkey(useR->key_me_exit);
    ok     .set_text  (Language::ok);
    cancel .set_text  (Language::cancel);

    seconds.set_format_time();
    seconds.set_step_big(60);
    seconds.set_step_med(10);
    seconds.set_step_sml( 1);

    add_child(author);
    add_child(name_german);
    add_child(name_english);
    add_child(initial);
    add_child(required);
    add_child(spawnint);
    add_child(seconds);
    add_child(ok);
    add_child(cancel);

    add_child(desc_author);
    add_child(desc_german);
    add_child(desc_english);
    add_child(desc_initial);
    add_child(desc_required);
    add_child(desc_spawnint);
    add_child(desc_clock);
}

WindowVariables::~WindowVariables()
{
}



void WindowVariables::calc_self()
{
    // Ausstieg ohne Speichern und mit Speichern
    if      (cancel.get_clicked()) set_exit();
    else if (ok    .get_clicked() || hardware.get_mr()) {
        // Gegen bloede Werte
        if (required.get_number() > initial.get_number()) {
            required.set_number(initial.get_number());
        }
        level->author       = author      .get_text();
        level->name_german  = name_german .get_text();
        level->name_english = name_english.get_text();

        level->initial      = initial     .get_number();
        level->required     = required    .get_number();
        level->spawnint     = spawnint    .get_number();
        level->seconds      = seconds     .get_number();
        set_exit();
    }
    // Ende vom Ausstieg
}

} // Ende Namensraum
