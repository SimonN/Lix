/*
 * menu/little.cpp
 *
 */

#include "little.h"

#include "../other/user.h"

namespace Api {

NewPlayerMenu::NewPlayerMenu()
:
    Window(190, 170, 260, 140, Language::option_new_player_title),
    exit  (false),
    name  ( 60, 100, 140),
    desc_hello(get_xl()/2, 40, Language::option_new_player_first,
                Label::CENTERED),
    desc_name (get_xl()/2, 60, Language::option_new_player_second,
                Label::CENTERED)
{
    add_child(name);
    add_child(desc_hello);
    add_child(desc_name);

    name.set_on_enter(this, name_on_enter);
    name.set_on();
}



NewPlayerMenu::~NewPlayerMenu()
{
}



void NewPlayerMenu::work_self()
{
    name.set_on();
}



void NewPlayerMenu::name_on_enter(void* menu_instance)
{
    NewPlayerMenu* men = static_cast <NewPlayerMenu*> (menu_instance);
    if (men->name.get_text().empty()) men->name.set_on();
    else {
        gloB->user_name = men->name.get_text();
        useR->load();
        gloB->save();
        men->exit = true;
    }
}



void NewPlayerMenu::draw_self()
{
    // Dies verhindert den pinken Hintergrund. Wir muessen dies tun, denn
    // wir waren bisher noch nicht im Hauptmenue.
    get_ground().clear_to_color(color[COL_BLACK]);

    Window::draw_self();
}



// ############################################################################
// ############################################################################
// ############################################################################



LanguageMenu::LanguageMenu()
:
    Window(250, LEMSCR_Y/2 - 15 * (Language::MAX + 1) +  5,
           140,              30 * (Language::MAX + 1) - 10,
           Language::main_name_of_the_game),
    exit  (false)
{
    for (unsigned i = 0; i < Language::MAX; ++i)
     button.push_back(TextButton(20, 10 + 30 * i, 100, 20,
                                 Language::language_name[i]));

    for (unsigned i = 1; i < button.size(); ++i)
     add_child(button[i]);

    // Dies verhindert den pinken Hintergrund. Wir muessen dies tun, denn
    // wir waren bisher noch nicht im Hauptmenue.
    get_ground().clear_to_color(color[COL_BLACK]);
}



LanguageMenu::~LanguageMenu()
{
}



void LanguageMenu::calc_self()
{
    std::vector <TextButton> ::iterator i = button.begin();
    for (++i; i != button.end(); ++i)
     if (i->get_clicked()) {
        exit_with = static_cast <Language::Language> (i - button.begin());
        exit = true;
    }
}

} // Ende Namensraum Api
