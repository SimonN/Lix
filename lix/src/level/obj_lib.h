/*
 * level/obj_lib.h
 *
 * Verwaltung fuer Terrainobjekte und Levelobjekte
 *
 */

#pragma once

#include <map>

#include "graset.h"

class ObjLib {

public:

    enum OrigSet {
        DIRT,
        HELL,
        PASTEL,
        PILLAR,
        CRYSTAL,
        BRICK,
        ROCK,
        SNOW,
        BUBBLE,
        HOLIDAY,
        L1_MAX,

        CLASSIC = L1_MAX,
        BEACH,
        CAVELEMS,
        CIRCUS,
        EGYPTIAN,
        HIGHLAND,
		MEDIEVAL,
		OUTDOOR,
        POLAR,
		SHADOW,
		SPACE,
		SPORTS,
        L2_MAX,

        MAX = L2_MAX
    };

    static void  initialize();
    static void  deinitialize();

    static const Object* get             (const std::string&);
    static const Object* get_orig_terrain(const int set, const int id);
    static const Object* get_orig_special(const int set, const int id);
    static const Object* get_orig_vgaspec(const int id);

    static const std::string  get_filename(const Object*);

    static const std::string& orig_set_to_string(const int);
    static const OrigSet      string_to_orig_set(const std::string&);

private:

    static ObjLib* lib;

    static const          std::string  vgaspec_string;
    std::map    <OrigSet, std::string> orig_set_string;

    std::map    <std::string, Object>      object;
    std::map    <std::string, std::string> queue;       // 1=w/o ext 2=filename
    std::vector <GraphicSet*>              graphic_set;
    std::vector <Object*>                  vgaspec;

           void load_l1_set       (const int);          // if used first time
           void load_l2_set       (const int);          // if used first time
           void load_vgaspec      (const int);          // if used first time
           void load_file         (const std::string&, const std::string&);
    static void load_file_callback(std::string&, void*);
    static void decrunch_level    (std::string&, void*);

    ObjLib();
    ~ObjLib();

    // Kopierverbot
    ObjLib          (const ObjLib&);
    void operator = (const ObjLib&);

};
