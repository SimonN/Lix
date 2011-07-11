/*
 * level/crunch.h
 *
 * Wrapper-Klasse, die die DAT-Komprimierung entpackt und die Datei in ihre
 * Sektionen sortiert. Es wird erst entkomprimiert, wenn eine Datei auch
 * tatsaechlich benoetigt wird.
 *
 * Moechte man auf die Sektion am Anfang der Datei beispiel.dat zugreifen,
 * so geht dies folgendermassen:
 *
 *   const Crunch::File* myfile = Crunch::get_file("example.dat");
 *
 *   // Does it exist? Pointer is 0 for missing files
 *   if (myfile) {
 *       const Crunch::Section& mysection = (*myfile)[0];
 *       // Get third byte from the section
 *       unsigned char mybyte = mysection[2];
 *       // ...
 *   }
 *
 */

#pragma once

#include <fstream>
#include <map>
#include <vector>

#include "../other/filename.h"
#include "../other/types.h"

class Crunch {

public:

    typedef std::vector <Byte>    Section;
    typedef std::vector <Section> File;

    static const File* get_file    (const Filename&);
    static void        save_section(const Section&, const Filename&);

private:

    static Crunch cr;

    std::map <Filename, File> data;

    inline Crunch() {};
    inline ~Crunch() {};

    // Kopierverbot
    Crunch(const Crunch&);
    void operator = (const Crunch&);

    static int read_byte(std::ifstream&);
    static int read_word(std::ifstream&);

};
