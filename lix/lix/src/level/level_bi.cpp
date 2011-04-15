/*
 * other/level_bi.cpp
 *
 * Laden von Levels im Original-Lemmings-Format (.LVL)
 *
 * Als Referenz dient diese Beschreibung des Formats, die im Lemmings File
 * Archive zu finden sein sollte:
 *
 *   http://camanis.net/lemmings/files/docs/lix_lvl_file_format.txt
 *
 * Many thanks to the active Lemmings community! Without these folks, such
 * things wouldn't be possible for me today. :-)
 *
 */

#include <fstream>

#include "level.h"
#include "obj_lib.h"

#include "../other/help.h"

int read_one_byte_levelbi(std::ifstream& file)
{
    unsigned char c;
    file.read((char*) &c, 1);
    return c;
}

int read_two_bytes_levelbi(std::ifstream& file)
{
    unsigned char c1 = read_one_byte_levelbi(file);
    unsigned char c2 = read_one_byte_levelbi(file);
    return 256 * c1 + c2;
}

std::string read_level_name_bytes(std::ifstream&);



void Level::load_from_binary(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.good()) {
        status = BAD_FILE_NOT_FOUND;
        return;
    }

    // ==GLOBALS===============================================================

    // BYTES 0x0000 to 0x0001
    // Release Rate	: 0x0000 is slowest, 0x00FA is fastest
    // 0x00FA ist 250 im Dezimalsystem. 99 ist die hoechste Rate. Korrigiert
    // wird die Rate, falls sie unter 1 liegt, noch in load_from().
    rate = read_two_bytes_levelbi(file);

    // BYTES 0x0002 to 0x0003
    // Num of lemmings	: maximum 0x0072.  0x0010 would be 16 lemmings.
    initial = read_two_bytes_levelbi(file);

    // BYTES 0x0004 to 0x0005
    // Num to rescue	: should be less than or equal to number of lemmings
    required = read_two_bytes_levelbi(file);

    // BYTES 0x0006 to 0x0007
    // Time Limit	: max 0x00FF, 0x0001 to 0x0009 works best
    seconds = read_two_bytes_levelbi(file) * 60;

    // BYTES 0x0008 to 0x0017 (2 bytes each, only lower byte is used)
    // Num of skills	: max 0x00FA.  order is Climber, Floater, Bomber,
    // 		  Blocker,Builder, Basher, Miner, Digger
    for (int i = 0; i < 8; ++i) {
        switch (i) {
            case 0: skill[i].ac = LixEn::CLIMBER;  break; // 0x0008 und 09
            case 1: skill[i].ac = LixEn::FLOATER;  break; // 0x000A ...
            case 2: skill[i].ac = LixEn::EXPLODER; break; // 0x000C
            case 3: skill[i].ac = LixEn::BLOCKER;  break; // 0x000E
            case 4: skill[i].ac = LixEn::BUILDER;  break; // 0x0010
            case 5: skill[i].ac = LixEn::BASHER;   break; // 0x0012
            case 6: skill[i].ac = LixEn::MINER;    break; // 0x0014 ...
            case 7: skill[i].ac = LixEn::DIGGER;   break; // 0x0016 und 17
        }
        skill[i].nr = read_two_bytes_levelbi(file);
    }

    // ORIGHACK: If a two-player level is loaded, make the given calculation
    // for the time, as the result (5 -> 2 minutes) is a nice overtime for
    // a multiplayer game. The overtime starts counting after the first player
    // is out of lemmings, but has some saved.
    // Also: Use knockback exploders instead of L1-style exploders.
    if (filename.find("network/") != std::string::npos) {
        skill[2].ac = LixEn::EXPLODER2;
        seconds = (seconds / 2) - 30;
        if (seconds <= 0) seconds = 15;
    }

    // BYTES 0x0018 to 0x0019
    // Start screen xpos : 0x0000 to 0x04F0.  is rounded to nearest multiple
    // 		    of 8.
    // We will multiply everything by 2 later, as L++ uses double the resol.
    start_x  = read_two_bytes_levelbi(file);
    size_x   = 1600;
    size_y   = 160;

    // BYTES 0x001A to 0x001B
    // Normal Graphic Set: 0x0000 is dirt, 0x0001 is fire, 0x0002 is squasher,
    // 		    0x0003 is pillar,0x0004 is crystal, 0x0005 is brick,
    // 		    0x0006 is rock, 0x0007 is snow and 0x0008 is bubble.
    // Bei mir zusaetzlich: 0x0009 is holiday
    int graphics_set = read_two_bytes_levelbi(file);

    // ORIGHACK: Bei Levels aus den Verzeichnissen zu ONML oder Holiday
    // entsprechend um 5 erhoehen bzw. auf 9 setzen.
    if (filename.find("ONML/")         != std::string::npos
     || filename.find("onml/")         != std::string::npos
     || filename.find("ore Lemmings/") != std::string::npos) graphics_set += 5;
    if (filename.find("oliday")        != std::string::npos) graphics_set =  9;

    // BYTES 0x001C to 0x001D
    // Extended Graphic Set: corresponds to VGASPEC?.DAT
    int spec_graphics = read_two_bytes_levelbi(file);
    if (spec_graphics != 0) {
        Pos ter;
        ter.x = 304;
        ter.y = 0;
        ter.ob = ObjLib::get_orig_vgaspec(spec_graphics);
        if (ter.ob) pos[Object::TERRAIN].push_back(ter);
        else status = BAD_IMAGE;
    }

    //  BYTES 0x001E to 0x001F
    // [04:14:50] <Mindless> "BYTES 0x001E to 0x001F" in level_bi.cpp
    // [04:15:37] <Mindless> that's 0x0000 in normal levels, 0xffff in
    // "Inroducing SuperLemming!" in which the gameplay is double speed
    // No support for this in L++ (yet).
    read_two_bytes_levelbi(file); // wegwerfen

    // ==OBJECTS===============================================================
    // BYTES 0x0020 to 0x011F (8 byte blocks)
    // each 8 byte block starting at byte 0x0020 represents an interactive
    // object. there can be a maximum of 32 objects.  write 0x00 to fill bytes
    // up to 0x0120 if there are less than 32 objects.
    for (int loop = 0; loop < 32; ++loop) {

        bool add = false; // Design der Funktion hier: vorzeitiges continue
        Pos  spe;         // fuehrte zu: zu wenige Werte werden ausgelesen
        // x pos  : min 0xFFF8, max 0x0638.  0xFFF8 = -24, 0x0001 = -16,
        //  0x0008= -8, 0x0010 = 0, 0x0018 = 8, ... , 0x0638 = 1576
        //  note: should be multiples of 8
        spe.x = read_two_bytes_levelbi(file);
        if (spe.x != 0) add = true; // 0x0001 ist -16; also 0x0000 wohl nix
        if (spe.x == 1) spe.x -= 1;
        if (spe.x >= 0xFFF8) spe.x -= 0x10000;
        spe.x -= 0x10;

        // y pos  : min 0xFFD7, max 0x009F.  0xFFD7 = -41, 0xFFF8 = -8,
        //  0xFFFF = -1, 0x0000 = 0, ... , 0x009F = 159.
        // 	note: can be any value in the specified range
        spe.y = read_two_bytes_levelbi(file);
        if (spe.y >= 0xFFD7) spe.y -= 0x10000;
        spe.y -= spe.y % 4;

        // obj id : min 0x0000, max 0x000F.  the object id is different in each
        // 	graphics set, however 0x0000 is always an exit and 0x0001 is
        //  always a start. see appendix a for full object listings
        int spe_id = read_two_bytes_levelbi(file);

        // modifier : first byte can be 80 (do not overwrite existing terrain)
        // or 40 (must have terrain underneath to be visible). 00 specifies
        // always draw full graphic. second byte can be 8F (display graphic
        // upside-down) or 0F (display graphic normally)
        // Wird von L++ nicht unterstuetzt
        read_two_bytes_levelbi(file); // wegwerfen

        if (add) {
            spe.ob = ObjLib::get_orig_special(graphics_set, spe_id);
            if (spe.ob) {
                // Do not load/show the waving green and blue flags.
                // L++ features a better way of showing the goal ownership.
                if (spe.ob->type != Object::DECO || spe.ob->subtype != 1)
                 pos[spe.ob->type].push_back(spe);
            }
            else status = BAD_IMAGE;
        }
    }

    // ==TERRAIN===============================================================
    // BYTES 0x0120 to 0x075F (4 byte blocks)

    // each 4 byte block starting at byte 0x0120 represents a terrain object.
    // there can be a maximum of 400 terrain objects. Write 0xFF fill the bytes
    // up to byte 0x0760 if need be.
    for (int loop = 0; loop < 400; ++loop) {
        Pos  ter;
        bool add = true;

        // x ter : min 0x0000, max 0x063F.  0x0000 = -16, 0x0008 = -8,
        //  0x0010 = 0, 0x063f = 1583.
        // note: the xter also contains modifiers.  the first nibble can be...
        ter.x = read_two_bytes_levelbi(file);
        int first_nibble = ter.x / 0x1000;
        ter.x %= 0x1000; // Erstes Nibble entfernen
        ter.x -= 16;
        if (ter.x > 0x700) add = false; //ter.x -= 0x1000; // Sir Edmund, ...

        // y ter : 9-bit value. min 0xEF0, max 0x518.  0xEF0 = -38,
        //  0xEF8 = -37, 0x020 = 0, 0x028 = 1, 0x030 = 2, 0x038 = 3, ... ,
        //  0x518 = 159
        //  note: the yter value bleeds into the next value since it is 9bits.
        // terrain id: min 0x00, max 0x3F.
        // not all graphic sets have all 64 graphics.

        ter.y = read_two_bytes_levelbi(file);
        int terrain_id = ter.y % 0x0080; // Terrain-ID sind 7 Bits davon
        //if (ter.y >= 0xEF00) ter.y -= 0x10000;
        if (ter.y >= 0xE000) ter.y -= 0x10080;
        ter.y -= 0x0200;
        ter.y /= 0x0080; // Hintere 7 Bits abschnibbeln

        if (terrain_id == 0x7F) add = false;

        // Write 0xFF fill the bytes up to byte 0x0760 if need be.
        // Terrain-ID hat 7 Bits mit Maximum 0x3F, also ist 0x7F der Wert,
        // den die Terrain-ID annimmt, wenn das Objekt nicht existiert.
        if (add) {
            ter.ob = ObjLib::get_orig_terrain(graphics_set, terrain_id);
            // note: the xter also contains modifiers.  the first nibble can be
            // 	8 (don't overwrite existing terr., 4 (display upside-down), or
            // 	2 (remove terrain instead add). you can add them together.
            // 	0 indicates normal.
            // 	eg: 0xC011 means draw at xter=1, do not overwirte, upside-down.
            ter.noow = first_nibble & 8;
            ter.mirr = first_nibble & 4;
            ter.dark = first_nibble & 2;
            if (ter.ob) pos[Object::TERRAIN].push_back(ter);
            else status = BAD_IMAGE;
        }
    }

    // ==STEEL AREAS===========================================================
    // BYTES 0x0760 to 0x07DF (4 byte blocks)
    //
    // x pos : 9-bit value.  min 0x000, max 0xC78.  0x000 = -16, 0x008 = -12,
    // 	0x010 = -8, 0x018 = -4, ... , 0xC78 = 1580.
    // 	note: each hex value represents 4 pixels.  since it is 9 bit value it
    // 	      bleeds into the next attribute.
    //
    // y pos : min 0x00, max 0x27. 0x00 = 0, 0x01 = 4, 0x02 = 8, ... ,
    //        0x27 = 156
    // 	note: each hex value represents 4 pixels
    //
    // area : min 0x00, max 0xFF.  the first nibble is the x-size, from 0 - F.
    //        each value represents 4 pixels. the second nibble is the y-size.
    //        0x00 = (4,4), 0x11 = (8,8), 0x7F = (32,64), 0x23 = (12,16)
    //
    // eg: 00 9F 52 00 = put steel at (-12,124) width = 24, height = 12
    //
    // each 4 byte block starting at byte 0x0760 represents a steel area which
    // the lemmings cannot bash through. The first three bytes are given above,
    // and the last byte is always 00.. what a waste of space considering how
    // compact they made the first 3 bytes!  write 0x00 to fill each byte up to
    // 0x07E0 if need be.

    // for (int loop = 0x0760; loop < 0x07E0; ++loop) read_one_byte(file);

    name_english = read_level_name_bytes(file);

    file.close();

    // Finalisieren: Level auf tatsaechliche Groesse bringen, denn L++ hat
    // alles doppelt so gross wie normal
    size_x  *= 2;
    size_y  *= 2;
    start_x *= 2;
    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (std::list <Pos> ::iterator
     itr = pos[type].begin(); itr != pos[type].end(); ++itr) {
        itr->x *= 2;
        itr->y *= 2;
    }
    // Ueble Machenschaften, die den Level gar nicht so sehr wie das Original
    // darstellen, sondern dafuer viel schoener! Links und rechts den Level
    // abschneiden, wenn der Platz nicht gebraucht wird.
    int min_x = size_x;
    int max_x = 0;
    // Minimum und Maximum finden
    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (std::list <Pos> ::iterator
     itr = pos[type].begin(); itr != pos[type].end(); ++itr) {
        if (itr->dark) continue;
        if (itr->x < min_x) min_x = itr->x;
        if (itr->x + itr->ob->cb.get_xl() > max_x) {
            max_x = itr->x + itr->ob->cb.get_xl();
        }
    }
    // Nun alles entsprechend verschieben
    size_x  = max_x - min_x;
    start_x -= min_x;
    for (int type = Object::TERRAIN; type != Object::MAX; ++type)
     for (std::list <Pos> ::iterator
     itr = pos[type].begin(); itr != pos[type].end(); ++itr) {
        itr->x -= min_x;
    }

    // ORIGHACK: In multiplayer levels, the hatch direction should point
    // towards the center because torus_x can't be set.
    if (filename.find("network/") != std::string::npos
     && pos[Object::HATCH].size() > 1) {
        for (std::list <Pos> ::iterator hatch = pos[Object::HATCH].begin();
         hatch != pos[Object::HATCH].end(); ++hatch)
         if (hatch->x + hatch->ob->trigger_x >= size_x / 2)
         hatch->rot = 1;
    }
    // ORIGHACK: In 2-player levels, the goal order should be set in such
    // a way that the distance for both players to get to their goal is
    // the same. This is only done for 1. Lemmings, as in 2. ONML, the
    // goal order is already as intended.
    if (filename.find("network/2player/1. Lemmings") != std::string::npos
     && pos[Object::HATCH].size() == 2 && pos[Object::GOAL].size() == 2) {
        const Pos& h1 = *  pos[Object::HATCH].begin();
        const Pos& h2 = *++pos[Object::HATCH].begin();
        const Pos& g1 = *  pos[Object::GOAL ].begin();
        const Pos& g2 = *++pos[Object::GOAL ].begin();
        double dist_cur  = Help::hypot(h1.x, h1.y, g1.x, g1.y)
                         + Help::hypot(h2.x, h2.y, g2.x, g2.y);
        double dist_swap = Help::hypot(h1.x, h1.y, g2.x, g2.y)
                         + Help::hypot(h2.x, h2.y, g1.x, g1.y);
        if (dist_swap > dist_cur) pos[Object::GOAL].reverse();
    }
    // ORIGHACK: In 2-player levels, if there's one hatch only, player 0 goes
    // to the right and player 1 to the left. Thus, put goal 0 to the right.
    if ((filename.find("network/2player/1. Lemmings") != std::string::npos
      || filename.find("network/2player/2. ONML")     != std::string::npos)
     && pos[Object::HATCH].size() == 1 && pos[Object::GOAL].size() == 2) {
        const Pos& g1 = *  pos[Object::GOAL ].begin();
        const Pos& g2 = *++pos[Object::GOAL ].begin();
        if (g2.x > g1.x)   pos[Object::GOAL].reverse();
    }

}



std::string read_level_name_bytes(std::ifstream& file)
{
    // ==LEVEL NAME============================================================
    // BYTES 0x07E0 to 0x07FF
    //
    // a character string 32 bytes long.
    // write 0x20 (space) to fill up the empty bytes.
    file.seekg(0x7E0);

    std::string ret;
    for (int loop = 0x7E0; loop < 0x0800; ++loop) {
        ret += (char) read_one_byte_levelbi(file);
    }

    // Leerzeichen am Anfang entfernen
    std::string::iterator itr = ret.begin();
    while (itr != ret.end() && *itr == ' ') ++itr;
    ret.erase(ret.begin(), itr);

    // Leerzeichen am Ende entfernen
    itr = --ret.end();
    while (itr != ret.begin() && *itr == ' ') --itr;
    ret.erase(++itr, ret.end());

    return ret;
}



std::string Level::get_name_binary(const std::string& filename)
{
    std::ifstream file(filename.c_str(), std::ios::binary);
    std::string ret = read_level_name_bytes(file);
    file.close();
    return ret;
}
