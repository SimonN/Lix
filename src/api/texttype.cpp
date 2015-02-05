#include "texttype.h"
#include "manager.h"

#include "../other/help.h"

namespace Api {

Texttype::Texttype(const int x,  const int y,
                   const int xl,
                   AllowChars allow_ch)
:
    Button(x, y, xl, 20), // 20 ist generell bei Textelementen richtig
    invisible    (false),
    scroll       (false),
    allow_chars  (allow_ch),
    on_enter_void(0),
    on_esc_void  (0),
    on_enter     (0),
    on_esc       (0)
{
}



Texttype::~Texttype()
{
}








bool Texttype::get_too_long(const std::string t)
{
    return text_length(font_med, t.c_str()) > get_xl()-10;
}



void Texttype::set_text(const std::string& t)
{
    text = t;
    while (!scroll && get_too_long(text)) Help::remove_last_utf8_char(text);
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
        clear_keybuf();
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
        if (hardware.get_ml() || hardware.get_mr()
                              || hardware.key_enter_once()) {
            set_off();
            if (on_enter_void && on_enter && hardware.key_enter_once())
             on_enter(on_enter_void);
        }
        else if (hardware.key_once(KEY_ESC)) {
            set_off();
            text = text_backup;
            if (on_esc_void && on_esc) on_esc(on_esc_void);
        }
        // Oder doch noch im Schreibmodus bleiben? Dann Tastatur lesen.
        else {
            int k      = hardware.get_key();
            // despite var name, can actually be Unicode character
            int kascii = hardware.get_key_ascii();

            // use the key and/or unicode value
            if (k == KEY_BACKSPACE && text.size() > 0) {
                Help::remove_last_utf8_char(text);
            }
            else if (kascii < 0x20) {
                // anything under 0x20 are control characters -- tab, etc.,
                // and I believe this also prevents keys like cursor-left
                // that don't generate letters
                return;
            }
            else if ((allow_chars == ALLOW_UNICODE)
                  || (allow_chars == ALLOW_ONLY_ASCII    && kascii <= 0xFF)
                  || (allow_chars == ALLOW_ONLY_FILENAME && kascii <= 0xFF
                     && kascii != '/' && kascii != '\\' && kascii != ':'
                     && kascii != '*' && kascii != '?'  && kascii != '"'
                     && kascii != '<' && kascii != '>'  && kascii != '|'))
            {
                std::string::size_type oldsize = text.size();
                text += Help::make_utf8_seq(kascii);
                if (!scroll && get_too_long(text)) text.resize(oldsize);
            }
            // Ende Tastenverarbeitung
        }
        // Ende Schreibmodus
    }
    // Ende Abfrage, ob man im Schreibmodus ist
}








void Texttype::draw_self()
{
    // Zeichnen vorbereiten
    // td ist "text to display";
    std::string td;
    int         text_color    = color[COL_TEXT];
    bool        align_right   = false;
    bool        caret_visible = false;

    if (scroll && get_too_long(text)) {
        align_right = true;
        std::string::const_iterator iter = text.end();
        while(!get_too_long(td) && iter != text.begin()) {
            Help::move_iterator_utf8(text, iter, -1);
            td = text.substr(iter - text.begin());
        }
        td.erase(0, uwidth(td.c_str()));
    }
    else td = text;

    if (get_on() && (!get_down() || invisible)) {
        set_draw_required();
        text_color = color[COL_TEXT_ON];
        if (Help::timer_ticks % 60 < 30) {
            td += '^';
            caret_visible = true;
        }
    }

    // draw everything
    if (invisible) undraw_self();
    else           Button::draw_self();
    Help::draw_shadow_text(get_ground(), font_med,
     td.c_str(), align_right ? get_x_here() + get_xl() - 7
     - text_length(font_med, td.c_str())
     + text_length(font_med, "^") * caret_visible
     : get_x_here() + 3, get_y_here(), text_color, color[COL_API_SHADOW]);
}






} // Ende Namensraum Api
