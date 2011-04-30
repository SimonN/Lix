#include "brow_sav.h"
#include "manager.h"

#include "../level/level.h" // fuer die haeufig benutzte/beispielhafte Box
#include "../other/help.h"

namespace Api {

const int SaveBrowser::window_yl        (380);
const int SaveBrowser::frame_offset     ( 20);
const int SaveBrowser::info_file_name_y ( 40);
const int SaveBrowser::info_level_name_y( 60);
const int SaveBrowser::dir_list_y       (100);
const int SaveBrowser::dir_list_xl      (100);
const int SaveBrowser::dir_list_yl      (220);
const int SaveBrowser::file_list_xl     (360);

SaveBrowser::SaveBrowser(const std::string& bdir,
                         const std::string& ext,
                         const std::string& dir,
                               std::string file,
                               ListFile::SearchCrit crit,
                               BoxMessage* (*cmb)(const std::string&),
                         const bool replay_style)
:
    Window(60, 20, frame_offset*3
     + dir_list_xl + file_list_xl,
     window_yl, Language::save_browser_title),

    dir_list  (frame_offset,
     dir_list_y, dir_list_xl,
     dir_list_yl, bdir, dir),

    level_list(2 * frame_offset + dir_list_xl,
     dir_list_y, file_list_xl, dir_list_yl),

    file_name(2*frame_offset + dir_list_xl,
     dir_list_y + dir_list_yl + frame_offset, 140),

    ok    (file_name.get_x() + 160, file_name.get_y(),  80),
    cancel(file_name.get_x() + 260, file_name.get_y(), 100),

    desc_file_name(    frame_offset, info_file_name_y,
                       Language::editor_file_name),
    desc_level_name(   frame_offset, info_level_name_y,
                       Language::editor_file_name),
    desc_save_filename(frame_offset, file_name.get_y(),
                       Language::save_filename),
    label_file_name (level_list.get_x(), info_file_name_y),
    label_level_name(level_list.get_x(), info_level_name_y),

    exit_with(false),
    extension(ext),
    box_overwrite(0),
    new_box_overwrite(cmb)
{
    add_child(dir_list);
    add_child(level_list);
    add_child(file_name);
    add_child(ok);
    add_child(cancel);
    add_child(desc_file_name);
    add_child(desc_level_name);
    add_child(desc_save_filename);
    add_child(label_file_name);
    add_child(label_level_name);

    level_list.set_search_criterion(crit);
    level_list.set_write_file_names();
    if (replay_style) level_list.set_replay_style();
    level_list.load_dir(dir);
    set_subtitle(dir);

    ok    .set_hotkey(KEY_ENTER);
    ok    .set_text  (Language::ok);
    cancel.set_text  (Language::cancel);
    // cancel.set_hotkey(KEY_ESC); // Wird nicht gemacht, weil es manuell
    // kontrolliert wird. Beim Texteintippen soll ESC nur das Eintippen abbr.

    Help::string_remove_dir(file);
    Help::string_remove_extension(file);
    file_name.set_text(file);
    file_name.set_scroll();
}

SaveBrowser::~SaveBrowser()
{
    if (box_overwrite) delete box_overwrite;
}



void SaveBrowser::calc_self()
{
    // Unterfenster
    if (box_overwrite) {
        box_overwrite->set_draw_required(); // Damit Buttons eingedrueckt werd.
        switch (box_overwrite->get_button_clicked()) {
        case 1:
            exit_with = true; // Ja, speichern
            set_exit();
            // faellt durch
        case 2:
            delete box_overwrite;
            box_overwrite = 0;
            set_draw_required(); // remove remainders of the overlying box
        default:
            ;
        }
    }

    // Dieses Fenster
    else {
        // Das Texteintippen vereinnahmt alle Benutzereingaben mit Ausnahme
        // der OK- und Abbrechen-Buttons, die werden dennoch ausgefuehrt.
        bool typing = false;
        if (file_name.get_on()) {
            typing = true;
        }

        if (!typing) {
            if (hardware.key_once(KEY_ESC)) {
                set_exit();
            }
            else if (dir_list.get_clicked()) {
                level_list.load_dir(dir_list.get_current_dir());
                set_subtitle(dir_list.get_current_dir());
            }
            else if (level_list.get_clicked()) {
                std::string s = level_list.get_current_file();
                Help::string_remove_extension(s);
                Help::string_remove_dir(s);
                file_name.set_text(s);
                file_name.set_on();
            }
        }

        // Etwas aus editor/bit_brow.cpp kopierter Code
        if (hardware.get_mr()
         &&  (dir_list.is_mouse_here()
         || level_list.is_mouse_here())) {
            if (dir_list.get_current_dir() == dir_list.get_base_dir()) {
                if (!typing) set_exit();
            }
            else {
                dir_list  .set_current_dir_to_parent_dir();
                level_list.load_dir(dir_list.get_current_dir());
            }
        }
        else if (ok.get_clicked()) {
            if (file_name.get_text().empty()) return; // Nichts passiert

            std::string complete_file_name = dir_list.get_current_dir()
             + file_name.get_text() + extension;

            // Testen, ob die Datei bereits exisitiert; ggf. box_overwrite new
            if (exists(complete_file_name.c_str())) {
                box_overwrite = new_box_overwrite(complete_file_name);
                Manager::add_focus(box_overwrite);
            }
            // Datei exisitiert noch nicht
            else {
                exit_with = true; // Lass den Editor den Level speichern
                set_exit();
                Manager::remove_focus(this);
            }
        }
        if (cancel.get_clicked()) {
            set_exit();
            Manager::remove_focus(this);
        }
    } // Ende von diesem Fenster
}



void SaveBrowser::set_info_file_name(const std::string& s)
{
    label_file_name.set_text(s);
}



void SaveBrowser::set_info_level_name(const std::string& s)
{
    label_level_name.set_text(s);
}



std::string SaveBrowser::get_current_file()
{
    if (file_name.get_text().empty()) return gloB->empty_string;
    else return file_name.get_text() + gloB->ext_level;
}

std::string SaveBrowser::get_current_dir_and_file()
{
    if (file_name.get_text().empty()) return gloB->empty_string;
    else {
        std::string str = dir_list.get_current_dir();
        str += file_name.get_text();
        str += gloB->ext_level;
        return str;
    }
}



// Hauefig benoetigt und ein gutes Beispiel:
// BoxMessage im Speicherdialog, und Suchkriterium
bool SaveBrowser::search_criterion_level(const std::string& s)
{
    return Help::string_ends_with(s, gloB->ext_level)
     ||    Help::string_ends_with(s, gloB->ext_level_orig);
}



BoxMessage* SaveBrowser::new_box_overwrite_level(const std::string&file)
{
    std::string s1 = Language::editor_file_name  + ' ' + file;
    std::string s2 = Language::editor_level_name + ' ' + Level::get_name(file);

    Api::BoxMessage* box_overwrite = new Api::BoxMessage(500, 3,
                                     Language::save_box_overwrite_title);

    box_overwrite->add_text(Language::save_box_overwrite_question);
    box_overwrite->add_text(s1);
    box_overwrite->add_text(s2);
    box_overwrite->add_button(Language::yes, KEY_ENTER);
    box_overwrite->add_button(Language::no,  KEY_ESC);

    return box_overwrite;
}

}
// Ende Namensraum Api
