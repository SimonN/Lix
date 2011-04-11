#include "texttype.h"
#include "manager.h"

#include "../other/help.h"

namespace Api {

Texttype::Texttype(const int x,  const int y, const int xl)
:
    Button(x, y, xl, 20), // 20 ist generell bei Textelementen richtig
    invisible    (false),
    scroll       (false),
    on_enter_void(0),
    on_enter     (0)
{
}



Texttype::Texttype(const int x,  const int y,
                   const int xl, std::string& t)
:
    Button(x, y, xl, 20),
    invisible    (false),
    scroll       (false),
    on_enter_void(0),
    on_enter     (0)
{
    set_text(t);
}



Texttype::~Texttype()
{
}








bool Texttype::get_too_long(const std::string t)
{
    return !t.empty() && al_get_text_width(font_med, t.c_str()) > get_xl()-10;
}



void Texttype::set_text(const std::string& t)
{
    text = t;
    while (!scroll && get_too_long(text))          text.resize(text.size()-1);
    while (text.size()>0 && *text.rbegin() == ' ') text.resize(text.size()-1);
    set_draw_required();
}



void Texttype::set_on(const bool b)
{
    Button::set_on(b);
    if (!b) set_off();
    else {
        // Schreibmodus beginnen
        Manager::add_focus(this);
        text_backup = text; // Fuer ESC-Druck/Rechtsklick
    }
}



void Texttype::set_off()
{
    Button::set_off();
    Manager::remove_focus(this);
    set_text(text); // Leerzeichen am Ende abschneiden
}



void Texttype::calc_self()
{
    Button::calc_self();

    if (!get_on()) {
        if (get_clicked()) set_on();
    }
    // Schreibmodus
    else {
        // Schreibmodus beenden? Einmal ohne, einmal mit Backup-Wiederherst.
        // callbacks are called from here as well if set.
        if (Hardware::get_ml() || Hardware::get_mr()
                              || Hardware::get_key_enter_once()) {
            set_off();
            if (on_enter_void && on_enter && Hardware::get_key_enter_once())
             on_enter(on_enter_void);
        }
        else if (Hardware::get_key_once(ALLEGRO_KEY_ESCAPE)) {
            set_off();
            text = text_backup;
            if (on_esc_void && on_esc) on_esc(on_esc_void);
        }
        // Oder doch noch im Schreibmodus bleiben? Dann Tastatur lesen.
        else {
            if (Hardware::get_key_once(ALLEGRO_KEY_BACKSPACE) && text.size())
             text.resize(text.size() - 1);

            text += Hardware::get_typed_characters();
            while (!scroll && get_too_long(text)) text.resize(text.size()-1);
        }
    }
    // Ende Abfrage, ob man im Schreibmodus ist
}








void Texttype::draw_self()
{
    // Zeichnen vorbereiten
    // td ist "text to display";
    std::string td;
    ALLEGRO_COLOR text_color    = color[COL_TEXT];
    bool          align_right   = false;
    bool          caret_visible = false;

    if (scroll && get_too_long(text)) {
        align_right = true;
        int i = text.size() - 1;
        while (!get_too_long(td) && i >= 0) td = text.substr(i--);
        td.erase(0, 1);
    }
    else td = text;

    if (get_on() && (!get_down() || invisible)) {
        set_draw_required();
        text_color = color[COL_TEXT_ON];
        if (Help::get_timer_ticks() % 60 < 30) {
            td += '^';
            caret_visible = true;
        }
    }

    // draw everything
    if (invisible) undraw_self();
    else           Button::draw_self();
    Help::draw_shadow_text(get_ground(), font_med,
     td.c_str(), align_right ? get_x_here() + get_xl() - 7
     - al_get_text_width(font_med, td.c_str())
     + al_get_text_width(font_med, "^") * caret_visible
     : get_x_here() + 3, get_y_here(), text_color);
}






} // Ende Namensraum Api
