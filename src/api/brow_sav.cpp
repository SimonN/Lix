#include "brow_sav.h"
#include "manager.h"

#include "../level/level_me.h" // fuer die haeufig benutzte/beispielhafte Box
#include "../other/help.h"
#include "../other/user.h"

namespace Api {

const int SaveBrowser::window_yl        (380);
const int SaveBrowser::frame_offset     ( 20);
const int SaveBrowser::info_file_name_y ( 40);
const int SaveBrowser::info_level_name_y( 60);
const int SaveBrowser::dir_list_y       (100);
const int SaveBrowser::dir_list_xl      (100);
const int SaveBrowser::dir_list_yl      (220);
const int SaveBrowser::file_list_xl     (360);

SaveBrowser::SaveBrowser(const Filename&    bdir,
                         const std::string& ext,
                         const Filename&    filename,
                               ListFile::SearchCrit crit,
                               BoxMessage* (*cmb)(const Filename&),
                         const bool replay_style)
:
    Window(60, 20, frame_offset*3
     + dir_list_xl + file_list_xl,
     window_yl, Language::save_browser_title),

    dir_list  (frame_offset,
     dir_list_y, dir_list_xl,
     dir_list_yl, bdir, filename),

    level_list(2 * frame_offset + dir_list_xl,
     dir_list_y, file_list_xl, dir_list_yl),

    file_name(2 * frame_offset + dir_list_xl,
              dir_list_y + dir_list_yl + frame_offset,
              140, Texttype::ALLOW_ONLY_FILENAME),

    ok    (file_name.get_x() + 160, file_name.get_y(),  80),
    cancel(file_name.get_x() + 260, file_name.get_y(), 100),

    desc_file_name(    frame_offset, info_file_name_y,
                       Language::editor_file_name),
    desc_level_name(   frame_offset, info_level_name_y,
                       Language::editor_level_name),
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
    level_list.load_dir(filename);
    set_subtitle(filename.get_dir_rootless());

    ok    .set_hotkey(useR->key_me_okay);
    cancel.set_hotkey(useR->key_me_exit);
    ok    .set_text  (Language::common_ok);
    cancel.set_text  (Language::common_cancel);

    file_name.set_text(filename.get_file_no_ext_no_pre_ext());
    file_name.set_scroll();
    file_name.set_on_enter(this, texttype_on_enter_callback);

    // Immediately start typing
    file_name.set_on();
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
        bool typing = file_name.get_on();

        if (!typing) {
            if (dir_list.get_clicked()) {
                level_list.load_dir(dir_list.get_current_dir());
                set_subtitle(dir_list.get_current_dir().get_rootless());
            }
            else if (level_list.get_clicked()) {
                file_name.set_text(level_list.get_current_file()
                    .get_file_no_ext_no_pre_ext());
                file_name.set_on();
            }
        }

        if (hardware.get_mr()
         &&  (dir_list.is_mouse_here()
         || level_list.is_mouse_here())) {
            if (!typing) set_exit();
        }
        else if (ok.get_clicked()) {
            check_for_file_exist_and_maybe_save_and_exit();
        }
        if (cancel.get_clicked()) {
            set_exit();
            Manager::remove_focus(this);
        }
    } // Ende von diesem Fenster
}



void SaveBrowser::set_info_filename(const Filename& fn)
{
    label_file_name.set_text(fn.get_rootless());
}



void SaveBrowser::set_info_level_name(const std::string& s)
{
    label_level_name.set_text(s);
}



void SaveBrowser::set_texttype(const std::string& s)
{
    file_name.set_text(s);
    make_texttype_valid();
}


void SaveBrowser::make_texttype_valid()
{
    std::string str = file_name.get_text();
    std::string::iterator itr = str.end();
    while (itr != str.begin()) {
        const std::string::iterator itr_char_encoding_end = itr;
        Help::move_iterator_utf8(str, itr, -1);
        const int c = ::ugetc(&*itr);
        if (! ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
         ||    (c >= '0' && c <= '9')
         ||     c == '.' || c == ',' || c == '-' || c == '_' || c == '+'))
        {
            // Unicode block of mostly Latin letters with diacritics
            // [up to Latin Extended-A]
            if (0x00C0 <= c && c <= 0x017F) {
                // each line of string captures 16 chars starting from U+00C0
                // we set aside 2 chars per codepoint as a few codepoints'
                // closest non-diacritic equivalents are digraphs (ae, oe,
                // ij, etc.).  The rest mapping to single characters will have
                // ' ' as placeholder 2nd character.
                static char const mapping[] = "A A A A A A AEC E E E E I I I I "
                                              "D N O O O O O x O U U U U Y Thss"
                                              "a a a a a a aec e e e e i i i i "
                                              "d n o o o o o - o u u u u y thy "
                                              "A a A a A a C c C c C c C c D d "
                                              "D d E e E e E e E e E e G g G g "
                                              "G g G g H h H h I i I i I i I i "
                                              "I i IJijJ j K k q L l L l L l L "
                                              "l L l N n N n N n -nNgngO o O o "
                                              "O o OEoeR r R r R r S s S s S s "
                                              "S s T t T t T t U u U u U u U u "
                                              "U u U u W w Y y Y Z z Z z Z z s ";
                const int index = (c - 0x00C0) * 2;
                *itr = mapping[index];
                ++itr;
                if (mapping[index+1] != ' ') {
                    *itr = mapping[index+1];
                    ++itr;
                }
            }
            else if (c != ' ' && itr_char_encoding_end != str.end()) {
                *itr = '-';
                ++itr;
            }

            if (itr < itr_char_encoding_end)
                itr = str.erase(itr, itr_char_encoding_end);
        }
    }
    // if all else fails, generate random filename
    if (str.empty())
        for (int i = 0; i < 8; ++i)
            str += 'a' + ::rand() % 26;

    file_name.set_text(str);
}



// This is called when clicking OK, or when the user has hit enter after
// typing something into the box
void SaveBrowser::check_for_file_exist_and_maybe_save_and_exit()
{
    // generate random filename if empty, among other things
    make_texttype_valid();

    Filename complete_file_name(
        dir_list.get_current_dir().get_dir_rootful()
        + file_name.get_text() + extension);

    // Testen, ob die Datei bereits exisitiert; ggf. box_overwrite new
    if (exists(complete_file_name.get_rootful().c_str())) {
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



Filename SaveBrowser::get_current_file()
{
    make_texttype_valid();
    if (file_name.get_text().empty()) return Filename("");
    else {
        std::string str = dir_list.get_current_dir().get_dir_rootful();
        str += file_name.get_text();
        str += gloB->ext_level;
        return Filename(str);
    }
}



// Hauefig benoetigt und ein gutes Beispiel:
// BoxMessage im Speicherdialog, und Suchkriterium
bool SaveBrowser::search_criterion_level(const Filename& fn)
{
    return ListLevel::search_criterion(fn);
}



BoxMessage* SaveBrowser::new_box_overwrite_level(const Filename &filename)
{
    std::string s1 = Language::editor_file_name  + ' '
                   + filename.get_rootless();
    std::string s2 = Language::editor_level_name + ' '
                   + LevelMetaData(filename).get_name();

    Api::BoxMessage* box_overwrite = new Api::BoxMessage(500, 3,
                                     Language::save_box_overwrite_title);

    box_overwrite->add_text(Language::save_box_overwrite_question);
    box_overwrite->add_text(s1);
    box_overwrite->add_text(s2);
    box_overwrite->add_button(Language::common_yes, useR->key_me_okay);
    box_overwrite->add_button(Language::common_no,  useR->key_me_exit);

    return box_overwrite;
}



void SaveBrowser::texttype_on_enter_callback(void* browser_void)
{
    SaveBrowser* b = static_cast <SaveBrowser*> (browser_void);
    b->check_for_file_exist_and_maybe_save_and_exit();
}

}
// Ende Namensraum Api
