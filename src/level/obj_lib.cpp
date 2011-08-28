/*
 * level/obj_lib.cpp
 *
 */

#include <sstream>

#include "obj_lib.h"
#include "graset_1.h"
#include "graset_2.h"

#include "../graphic/gra_lib.h" // Namens-Replacement
#include "../other/globals.h"
#include "../other/help.h"

#define PUSH_BACK(v, a, b, c, d) \
 v.push_back(a); v.push_back(b); v.push_back(c); v.push_back(d);

ObjLib* ObjLib::lib = 0;

const std::string ObjLib::vgaspec_string("Vgaspec");



ObjLib::ObjLib()
:
    graphic_set   (MAX, 0)
{
    // Die Verzeichnisse nach Bilddateien durchsuchen
    // Abk.-Deklarationen, um die Funktionsaufrufe in einer Zeile zu haben
    const Filename& db                 = gloB->dir_bitmap;
    void (*cb)(const Filename&, void*) = load_file_callback;

    Help::find_tree(db, gloB->mask_ext_bmp, cb, (void*) this);
    Help::find_tree(db, gloB->mask_ext_tga, cb, (void*) this);
    Help::find_tree(db, gloB->mask_ext_pcx, cb, (void*) this);

    orig_set_string[DIRT]     = "Dirt";
    orig_set_string[HELL]     = "Hell";
    orig_set_string[PASTEL]   = "Pastel";
    orig_set_string[PILLAR]   = "Pillar";
    orig_set_string[CRYSTAL]  = "Crystal";
    orig_set_string[BRICK]    = "Brick";
    orig_set_string[ROCK]     = "Rock";
    orig_set_string[SNOW]     = "Snow";
    orig_set_string[BUBBLE]   = "Bubble";
    orig_set_string[HOLIDAY]  = "Holiday";

	orig_set_string[CLASSIC]  = "Classic";
	orig_set_string[BEACH]    = "Beach";
	orig_set_string[CAVELEMS] = "Cavelems";
	orig_set_string[CIRCUS]   = "Circus";
	orig_set_string[EGYPTIAN] = "Egyptian";
	orig_set_string[HIGHLAND] = "Highland";
	orig_set_string[MEDIEVAL] = "Medieval";
	orig_set_string[OUTDOOR]  = "Outdoor";
	orig_set_string[POLAR]    = "Polar";
	orig_set_string[SHADOW]   = "Shadow";
	orig_set_string[SPACE]    = "Space";
	orig_set_string[SPORTS]   = "Sports";

	orig_set_string[MAX]      = "Max";

    // Ins Levelverzeichnis kopierte Originallevels laden. Wir machen das hier,
    // weil die Level-Klasse nicht initialisiert wird, dies ist danach das
    // Naheliegendste. ORIGHACK
    Help::find_tree(gloB->dir_levels, "*.DAT", decrunch_level, (void*)this);
}



void ObjLib::load_file(const std::string& no_ext, const Filename& fn)
{
    // zweites Argument: Nur Schneideversuch unternehmen, wenn mit Prae-End.
    char pe = fn.get_pre_extension();
    const Cutbit c(fn, pe);

    Object::Type type = Object::TERRAIN;
    int  st = 0;
    if      (pe == gloB->pre_ext_steel[0])        { type = Object::TERRAIN; st = 1; }
    else if (pe == gloB->pre_ext_hatch[0])          type = Object::HATCH;
    else if (pe == gloB->pre_ext_deco[0])           type = Object::DECO;
    else if (pe == gloB->pre_ext_goal[0])           type = Object::GOAL;
    else if (pe == gloB->pre_ext_trap[0])           type = Object::TRAP;
    else if (pe == gloB->pre_ext_water[0])          type = Object::WATER;
    else if (pe == gloB->pre_ext_fire[0])         { type = Object::WATER;   st = 1; }
    else if (pe == gloB->pre_ext_oneway_left[0])    type = Object::ONEWAY;
    else if (pe == gloB->pre_ext_oneway_right[0]) { type = Object::ONEWAY;  st = 1; }

    // This amends an issue with Allegro 4.2 on Linux.
    // If the xl/yl of the image isn't divisible by 2, it doesn't get rotated
    // properly, creating a different landscape ==> desyncs.
    // We amend Terrain and Steel here, both are type == Object::TERRAIN.
    if (type == Object::TERRAIN
     && (c.get_xl() % 2 == 1 || c.get_yl() % 2 == 1)) {
        int nxl = c.get_xl() + (c.get_xl() % 2);
        int nyl = c.get_yl() + (c.get_yl() % 2);
        BITMAP* amended = create_bitmap(nxl, nyl);
        clear_to_color(amended, color[COL_PINK]);
        blit(c.get_al_bitmap(), amended, 0, 0, 0, 0, c.get_xl(), c.get_yl());
        Cutbit amended_cb(amended, false);
        // this gets to own the BITMAP. false = don't cut.
        object.insert(std::make_pair(no_ext, Object(amended_cb, type, st)));
    }
    else {
        object.insert(std::make_pair(no_ext, Object(c, type, st)));
    }

    // Load overriding definitions from a possibly accompanying text file.
    // That file must have the same name, only its extension must be replaced.
    if (type != Object::TERRAIN) {
        std::map <std::string, Object> ::iterator itr = object.find(no_ext);
        if (itr != object.end() && itr->second.type != Object::TERRAIN) {
            const Filename defs(fn.get_rootless_no_extension()
                              + gloB->ext_object_definitions);
            itr->second.read_definitions_file(defs);
        }
    }
}



void ObjLib::load_file_callback(const Filename& s, void* v) {
    static_cast <ObjLib*> (v)->queue.insert(
        std::make_pair(s.get_rootless_no_extension(), s));
}


// Originale Grafik-Sets laden
// ORIGHACK
void ObjLib::load_l1_set(const int set_id)
{
    if (set_id < 0 || set_id >= L1_MAX
     || set_id >= (int) graphic_set.size()
     || graphic_set[set_id] != 0) return;
    std::ostringstream gro; gro << gloB->dir_bitmap_orig_l1.get_rootful() << set_id << ". ";
    std::ostringstream vgr; vgr << gloB->dir_bitmap_orig_l1.get_rootful() << set_id << ". ";
    gro << orig_set_to_string(set_id) << "/GROUND"  << set_id << "O.DAT";
    vgr << orig_set_to_string(set_id) << "/VGAGR"   << set_id << ".DAT";

    // Determine steel objects. BUBBLE has no steel, HOLIDAY just one object.
    // This uses the preprocessor macro PUSH_BACK, see top of file.
    std::vector <int> steel_ids;
    switch (set_id) {
        case DIRT:    PUSH_BACK(steel_ids, 23, 24, 25, 26); break;
        case HELL:    PUSH_BACK(steel_ids, 12, 13, 14, 15); break;
        case PASTEL:  PUSH_BACK(steel_ids,  5, 57, 58, 59); break;
        case PILLAR:  PUSH_BACK(steel_ids, 27, 48, 49, 50); break;
        case CRYSTAL: PUSH_BACK(steel_ids, 31, 32, 33, 34); break;
        case BRICK:   PUSH_BACK(steel_ids, 51, 52, 53, 54); break;
        case ROCK:    PUSH_BACK(steel_ids, 56, 57, 58, 59); break;
        case SNOW:    PUSH_BACK(steel_ids, 29, 30, 31, 32); break;
        case HOLIDAY: steel_ids.push_back( 29);             break;
        default:                                            break;
    }
    // Determine waving flag special objects. HOLIDAY has none.
    std::vector <int> flag_ids;
    switch (set_id) {
        case DIRT:    flag_ids.push_back(2); flag_ids.push_back( 9); break;
        case HELL:    flag_ids.push_back(2); flag_ids.push_back( 9); break;
        case PASTEL:  flag_ids.push_back(2); flag_ids.push_back( 7); break;
        case PILLAR:  flag_ids.push_back(2); flag_ids.push_back( 7); break;
        case CRYSTAL: flag_ids.push_back(2); flag_ids.push_back( 3); break;
        case BRICK:   flag_ids.push_back(2); flag_ids.push_back( 8); break;
        case ROCK:    flag_ids.push_back(2); flag_ids.push_back(11); break;
        case SNOW:    flag_ids.push_back(2); flag_ids.push_back( 7); break;
        case BUBBLE:  flag_ids.push_back(2); flag_ids.push_back( 7); break;
        default:                                                     break;
    }

    // If file not found, then the graphic set is empty and returns only
    // null pointers if one tries to get to its graphics. graphic_set[set_id]
    // is still != 0 in that case.
    graphic_set[set_id] = new GraphicSetL1(
        Filename(gro.str()), Filename(vgr.str()), steel_ids, flag_ids);
}



void ObjLib::load_l2_set(const int set_id)
{
    if (set_id < CLASSIC || set_id >= L2_MAX
     || set_id >= (int) graphic_set.size()
     || graphic_set[set_id] != 0) return;

    std::ostringstream sty;
	sty << gloB->dir_bitmap_orig_l2.get_rootful()
	    << orig_set_to_string(set_id) << "/"
	    << orig_set_to_string(set_id) << ".dat";
    graphic_set[set_id] = new GraphicSetL2(sty.str());
}




void ObjLib::load_vgaspec(const int id)
{
    if (id >= (int) vgaspec.size()) vgaspec.resize(id + 1);
    if (id < 0 || vgaspec[id] != NULL) return;

    std::ostringstream filename_vspec;
    filename_vspec << gloB->dir_bitmap_orig_l1.get_rootful();
    filename_vspec << "VGASPEC" << (id - 1) << ".DAT";
    // id - 1 is done here because in LVL files, 0x0000 indicates that
    // no vgaspec file is used.

    const Crunch::File* crunchfile = Crunch::get_file(
        Filename(filename_vspec.str()));
    if (crunchfile && crunchfile->size() == 1) {
        BITMAP* spec_bitmap
         = GraphicSetL1::new_read_spec_bitmap((*crunchfile)[0]);
        vgaspec[id] = new Object(Cutbit(spec_bitmap, false));
    }
}



// ORIGHACK
void ObjLib::decrunch_level(const Filename& fn, void* v)
{
    v = v; // counters the warning about unused variables
    const Crunch::File* crunchfile = Crunch::get_file(fn);
    std::string s = fn.get_rootful();
    s += '-';
    if (crunchfile)
     for (Crunch::File::const_iterator
     itr = crunchfile->begin(); itr != crunchfile->end(); ++itr) {
        std::ostringstream sstr;
        sstr << s << itr - crunchfile->begin() << gloB->ext_level_orig;
        Crunch::save_section(*itr, Filename(sstr.str()));
    }
}



ObjLib::~ObjLib()
{
    for (std::vector <GraphicSet*> ::iterator
     itr = graphic_set.begin(); itr != graphic_set.end(); ++itr) {
        if (*itr) delete *itr;
        *itr = 0;
    }
    for (std::vector <Object*> ::iterator
     itr = vgaspec.begin(); itr != vgaspec.end(); ++itr) {
        if (*itr) delete *itr;
        *itr = 0;
    }
}



void ObjLib::initialize()   { if (!lib)  lib = new ObjLib;      }
void ObjLib::deinitialize() { if (lib) { delete lib; lib = 0; } }



// Genau wie fuer string_to_ac() etc. in lix/init.cpp
const std::string& ObjLib::orig_set_to_string(const int o) {
    for (std::map <OrigSet, std::string>::iterator
     i  =  lib->orig_set_string.begin();
      i != lib->orig_set_string.end(); ++i) {
        if (i->first == o) return i->second;
    } return (--(lib->orig_set_string.end()))->second;
}



ObjLib::OrigSet ObjLib::string_to_orig_set(const std::string& s) {
    for (std::map <OrigSet, std::string>::iterator
     i  =  lib->orig_set_string.begin();
      i != lib->orig_set_string.end(); ++i) {
        if (i->second == s) return i->first;
    } return MAX;
}



const Object* ObjLib::get(const Filename& fn)
{
    return get(fn.get_rootless_no_extension());
}



const Object* ObjLib::get(const std::string& str)
{
    // This function has a lot of returns along its way. Successfully found
    // objects are always returned directly.

    // Return object from readily loaded image file
    std::map <std::string, Object> ::const_iterator
     itr = lib->object.find(str);
    if (itr != lib->object.end()) return &itr->second;

    // Return object from a graphics set (OrigSet)
    // Format: Dirt-t02 oder Pillar-s63
    // Detect if the end of the string is of this nature
    bool orig_ending = false;
    std::string::const_iterator stritr = --str.end();
    while (stritr != str.begin() && *stritr >= '0' && *stritr <= '9') --stritr;
    if (stritr != str.begin() && (*stritr == 't' || *stritr == 's')) {
        --stritr;
        if (stritr != str.begin() && *stritr == '-') orig_ending = true;
        // Still can't be the beginning, as the setname comes before '-'.
    }
    if (orig_ending) {
        // *stritr == '-' right now
        const std::string setname = std::string(str.begin(), stritr);
        int set =  string_to_orig_set(setname);
        if (set != MAX) {
            int c = *++stritr; // c contains either 's' or 't' now
            int id = 0;
            while (++stritr != str.end()) {
                id *= 10;
                id += (*stritr - '0');
            }
            // Return the proper object
            if (setname == vgaspec_string) return get_orig_vgaspec(id);
            if (c == 's' || c == 'S')      return get_orig_special(set, id);
            else                           return get_orig_terrain(set, id);
        }
        // Do not return 0 here just because setname didn't match anything
        // from the preset names. Some person might have put in a '-' into
        // his regular graphic file names, thouse would be missed otherwise.
    }

    // Seek object name in the prefetch queue, if it's there, load and return
    std::map <std::string, Filename> ::iterator
     pre_itr = lib->queue.find(str);
    if (pre_itr != lib->queue.end()) {
        lib->load_file(pre_itr->first, pre_itr->second);
        lib->queue.erase(pre_itr);
        return lib->get(str); // recursive, but only depth 1
    }

    // Otherwise use deprecated text replacement
    const std::string& search_str = GraLib::replace_filestring(str);
    if (search_str != str) {
        return lib->get(search_str); // recursive, hopefully depth 1 <_<;
    }

    // Otherwise, strap "./" from the string
    if (str.length() > 3) return lib->get(str.substr(2, std::string::npos));

    else return 0;
}



const Object* ObjLib::get_orig_special(
    const int set,
    const int spe_id
) {
    if (set < 0 || set >= (int) lib->graphic_set.size()) return 0;
    if (!lib->graphic_set[set]) {
        if (set < L1_MAX) lib->load_l1_set(set);
        else              lib->load_l2_set(set);
    }
    return lib->graphic_set[set]->get_special(spe_id);
}



const Object* ObjLib::get_orig_terrain(
    const int set,
    const int ter_id
) {
    if (set < 0 || set >= (int) lib->graphic_set.size()) return 0;
    if (!lib->graphic_set[set]) {
        if (set < L1_MAX) lib->load_l1_set(set);
        else              lib->load_l2_set(set);
    }
    return lib->graphic_set[set]->get_terrain(ter_id);
}



const Object* ObjLib::get_orig_vgaspec(
    const int id
) {
    if (id < 0) return 0;
    lib->load_vgaspec(id);
    return lib->vgaspec[id];
}



const std::string ObjLib::get_filename(const Object* const ob)
{
    for (std::map <std::string, Object> ::const_iterator
     itr = lib->object.begin(); itr != lib->object.end(); ++itr)
     if (ob == &itr->second) return itr->first;
    // If we keep going from here, the search wasn't succesful
    int terrain_id = -1;
    int special_id = -1;
    for (int set = 0; set < (int) lib->graphic_set.size(); ++set) {
        // Do not load a graphic set if it's not yet loaded. It's not neces-
        // sary because Object ob cannot be something from an unloaded set.
        if (!lib->graphic_set[set]) continue;
        terrain_id = lib->graphic_set[set]->get_terrain_id(ob);
        special_id = lib->graphic_set[set]->get_special_id(ob);
        if (terrain_id != -1 || special_id != -1) {
            std::ostringstream ret;
            ret << orig_set_to_string(set) << '-';
            if (special_id != -1) ret << 's' << special_id;
            else                  ret << 't' << terrain_id;
            return ret.str();
        }
    }
    for (int i = 0; i < (int) lib->vgaspec.size(); ++i)
     if (ob == lib->vgaspec[i]) {
        std::ostringstream ret;
        ret << vgaspec_string << "-t" << i;
        return ret.str();
    }
    // Object not found anywhere
    return gloB->empty_string;
}
