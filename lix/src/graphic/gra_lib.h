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
 * Lemming-Bitmaps und andere interne Bitmaps werden direkt als Referenz
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

class GraLib {

private:

    static GraLib* singleton;

    std::map    <std::string, std::string>           replacement;
    std::map    <std::string, const Cutbit>          internal;
    std::vector <Cutbit>                             style;
    std::vector <Cutbit>                             icons;

    static void sort_string_into_cutbitfile(std::string&, void*);
    static void sort_string_into_internal  (std::string&, void*);

           void recolor_into_vector(const Cutbit&, std::vector <Cutbit>&);

    GraLib();
    ~GraLib();

    // Kopierverbot
    GraLib          (const GraLib&);
    void operator = (const GraLib&);

public:

    static void  initialize();
    static void  deinitialize();

    static const Cutbit& get      (const std::string&);
    static const Cutbit& get_lix  (const LixEn::Style);
    static const Cutbit& get_icon (const LixEn::Style); // for the panel

    static const std::string& replace_string(const std::string&);

};
