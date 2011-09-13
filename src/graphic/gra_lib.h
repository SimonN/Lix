/*
 * level/gra_lib.h
 *
 * Grafikbibliothek. Hier werden saemtliche Levelgrafiken des ganzen Spiels
 * verwaltet und stehen zum sinnvollen Abrufen bereit.
 *
 * Die Einfuehrung einer solchen Klasse wurde notwendig, als Originallevels
 * und deren Grafiken unterstuetzt wurden. Die Originalgrafiken werden naemlich
 * nicht als Datei gespeichert.
 *
 * Lix-Bitmaps und andere interne Bitmaps werden direkt als Referenz
 * zurueckgegeben, weil die Dinger immer existieren sollten. Ansonsten wird
 * leeres Cutbit in der Map erzeugt und zurueckgegeben, was dann zu roten
 * Frame-existiert-nicht-Boxen fuehrt.
 *
 */

#pragma once

#include <map>
#include <vector>

#include "cutbit.h"

#include "../lix/lix_enum.h"
#include "../other/filename.h"
#include "../other/types.h"

class GraLib {

private:

    static GraLib* singleton;

    std::map    <std::string, std::string> replace_exact;
    std::map    <std::string, std::string> replace_substr;

    std::map    <std::string, Cutbit>      internal;
    std::vector <Cutbit>                   style;
    std::vector <Cutbit>                   icons;

    static void sort_string_into_cutbitfile(const Filename&, void*);
    static void sort_string_into_internal  (const Filename&, void*);


    // The int variables should be != 0 for the character spreadsheet and
    // similar things that require both a GUI and a player color recoloring.
           void eidrecol_api       (const Filename&);
           void eidrecol_api       (Cutbit&, int = 0);
           void recolor_into_vector(const Cutbit&, std::vector <Cutbit>&,
                                    int = 0);
           void add_replace        (const std::string&, const std::string&);
           void add_substr_replace (const std::string&, const std::string&);

    GraLib();
    ~GraLib();

    // Kopierverbot
    GraLib          (const GraLib&);
    void operator = (const GraLib&);

public:

    static void  initialize();
    static void  deinitialize();

    static const Cutbit& get      (const Filename&);
    static const Cutbit& get_lix  (const LixEn::Style);
    static const Cutbit& get_icon (const LixEn::Style); // for the panel

    static std::string replace_filestring(const std::string&);

};
