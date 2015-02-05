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
    name  ( 60, 100, 140, Texttype::ALLOW_UNICODE),
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
    static Language::Language const orderedLanguages[] = {
        Language::CUSTOM,
        Language::ENGLISH,
        Language::GERMAN,
    };

    size_t const displayOrderLength = sizeof(orderedLanguages) /
                                      sizeof(orderedLanguages[0]);
    unsigned displayOrderStart = 0;
    // argument true means only load the custom language's name
    if (!Language::try_load_custom_language(true)) {
        // reach here if no custom language (data/translate.txt) installed
        // drop the button for that from UI and resize window accordingly
        displayOrderStart = 1;
        set_y (LEMSCR_Y/2 - 15 * (Language::MAX) +  5);
        set_yl(             30 * (Language::MAX) - 10);
    }

    for (unsigned i = displayOrderStart; i < displayOrderLength; ++i) {
        button.push_back(TextButton(20, 10 + 30 * (i - displayOrderStart + 1),
                                    100, 20,
                                    Language::language_name[orderedLanguages[i]]));
        corresponding_language.push_back(orderedLanguages[i]);
    }

    for (unsigned i = 0; i < button.size(); ++i)
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
    std::vector <TextButton> ::const_iterator i = button.begin();
    for (; i != button.end(); ++i)
     if (i->get_clicked()) {
        exit_with = corresponding_language[i - button.begin()];
        exit = true;
    }
}

} // Ende Namensraum Api
