/*
 * editor/bit_brow.h
 *
 * Der Bild-Browser fuer den Editor. Basisverzeichnis ist immer das
 * Bitmap-Verzeichnis, das aktuelle Verzeichnis wird angegeben.
 *
 * void register_pre_extension(const std::string&)
 *
 *   Siehe Kommentar in ../api/list_fil.h.
 *
 * std::string& get_exit_with()
 *
 *   Liefert den Dateinamen des zu ladenden Bildes. Der String ist leer, wenn
 *   "Cancel" gedrueckt wurde oder aus sonstigen Gruenden nichts geladen
 *   werden kann.
 *
 */

#pragma once

#include "../api/list_dir.h"
#include "../api/list_fil.h"
#include "../api/window.h"
#include "../level/object.h" // return object

namespace Api {

class ListBitmap : public ListFile {

private:

    static const int xl;
    static const int yl;
    static const int in_row;

    virtual void add_file_button(const int, const int);
    virtual void add_flip_button();

    virtual void on_dir_load(); // override ListFile's for orig. grahpic sets

public:

    ListBitmap(int, int, int, int, ListFile::SearchCrit = 0);
    virtual ~ListBitmap();

}; // Ende von Klasse ListBitmap





class BitmapBrowser : public Window {

private:

    friend class ListBitmap; // um die folgenden Variablen auszulesen

    static const int this_yl;
    static const int frame_offset;

    static const int dir_list_xl;
    static const int dir_list_yl;
    static const int dir_list_entry_yl;

    static const int file_list_xl;
    static const int file_list_yl;
    static const int file_list_entry_space;
    static const int file_list_entry_xl;
    static const int file_list_entry_yl;

    // Dateien und Verzeichnisse
    DirList    dir_list;
    ListBitmap list_bitmap;
    TextButton cancel;

    const Object* return_object;

public:

    BitmapBrowser(ListFile::SearchCrit = 0,
                  const Filename&    = gloB->dir_bitmap,   // cur. directory
                  const std::string& = gloB->empty_string, // Fenstertitel
                  const int = 0);                          // page to load
    ~BitmapBrowser();

    void load_dir (const Filename&, const int initial_page);

    inline int             get_page()       { return list_bitmap.get_page();  }
    inline const Filename& get_current_dir(){ return dir_list.get_current_dir();}
    inline const Object*   get_exit_with()  { return return_object;           }

protected:

    virtual void calc_self();

}; // Ende von Klasse BitmapBrowser
} // Ende von Namensraum Api
