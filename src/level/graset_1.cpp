/*
 * level/gra_set.cpp
 *
 * Ich zitiere haeufig aus
 *
 *   (Lemmings File Archive)/lix_vgagrx_dat_groundxo_dat_file_format.txt
 *
 */

#include <fstream>

#include "../other/myalleg.h"

#include "graset_1.h"
#include "../graphic/glob_gfx.h"

GraphicSetL1::GraphicSetL1(
    const Filename&          filename_defin,
    const Filename&          filename_vgagr,
    const std::vector <int>& steel_ids,
    const std::vector <int>& waving_flag_ids
) {
    std::ifstream defin(filename_defin.get_rootful().c_str(),
                        std::ios::binary);
    read_specials(defin);
    read_terrain(defin);
    read_palette(defin);
    defin.close();

    const Crunch::File* crunchfile = Crunch::get_file(filename_vgagr);
    if (crunchfile && crunchfile->size() == 2) {
        make_terrain ((*crunchfile)[0], steel_ids);
        make_specials((*crunchfile)[1], waving_flag_ids);
    }

    terinf.clear();
    speinf.clear();
}



int GraphicSetL1::read_byte(std::ifstream& file)
{
    unsigned char c;
    file.read((char*) &c, 1);
    return c;
}



int GraphicSetL1::read_word(std::ifstream& file)
{
    // First of all, a word on WORD and BYTE in groundXo.dat.
    // Unlike the .LVL file format, WORDs in groundXo.dat are stored
    // little-endian.
    unsigned char c1 = read_byte(file);
    unsigned char c2 = read_byte(file);
    return 256 * c2 + c1; // Hier wird die Endianess beachtet
}



void GraphicSetL1::read_specials(std::ifstream& defin)
{
    // In other words, first comes 16 slots for describing 16 objects.
    // An OBJECT_INFO slot is 28 bytes in size, so the OBJECT_INFO array
    // takes up a total of 28*16 = 448 bytes.
    speinf.clear();
    defin .seekg(0);

    for (int loop = 0; loop < 16; ++loop) {
        ObjectInfo spe;
        spe.animation_flags             = read_word(defin);
        spe.start_animation_frame_index = read_byte(defin);
        spe.end_animation_frame_index   = read_byte(defin);
        spe.width                       = read_byte(defin);
        spe.height                      = read_byte(defin);
        spe.animation_frame_data_size   = read_word(defin);
        spe.mask_offset_from_image      = read_word(defin);
                                          read_word(defin); // wegwerfen
                                          read_word(defin); // wegwerfen
        spe.trigger_left                = read_word(defin);
        spe.trigger_top                 = read_word(defin);
        spe.trigger_width               = read_byte(defin);
        spe.trigger_height              = read_byte(defin);
        spe.trigger_effect_id           = read_byte(defin);
        spe.animation_frames_base_loc   = read_word(defin);
        spe.preview_image_index         = read_word(defin);
                                          read_word(defin); // wegwerfen
        spe.trap_sound_effect_id        = read_byte(defin);
        speinf.push_back(spe);
    }
}



void GraphicSetL1::read_terrain(std::ifstream& defin)
{
    // In other words, first comes 16 slots for describing 16 objects.
    // An OBJECT_INFO slot is 28 bytes in size, so the OBJECT_INFO array
    // takes up a total of 28*16 = 448 bytes.
    terinf.clear();
    defin .seekg(448);

    // A TERRAIN_INFO is 8 bytes, so the TERRAIN_INFO array takes up a total
    // of 8*64 = 512 bytes.
    for (int loop = 0; loop < 64; ++loop) {
        TerrainInfo ter;
        ter.width     = read_byte(defin);
        ter.height    = read_byte(defin);
        ter.image_loc = read_word(defin);
        ter.mask_loc  = read_word(defin);
                        read_word(defin); // wegwerfen
        terinf.push_back(ter);
    }
}



// Nimmt Farben mit 0x3F als Hoechstwert, braucht Allegro
int make_0x3F_color(int r, int g, int b)
{
    return makecol(r * 255 / 63,   g * 255 / 63,   b * 255 / 63);
}

void GraphicSetL1::read_palette(std::ifstream& defin)
{
    palette.clear();

    // Zunaechst 8 feste Farben
    // palette[7] ist Kopie der ersten waehlbaren Farbe, siehe unten.
    palette.push_back(make_0x3F_color(0x00, 0x00, 0x00)); // black
    palette.push_back(make_0x3F_color(0x10, 0x10, 0x38)); // blue
    palette.push_back(make_0x3F_color(0x00, 0x2C, 0x00)); // green
    palette.push_back(make_0x3F_color(0x3C, 0x34, 0x34)); // white
    palette.push_back(make_0x3F_color(0x2C, 0x2C, 0x00)); // dirty yellow
    palette.push_back(make_0x3F_color(0x3C, 0x08, 0x08)); // red
    palette.push_back(make_0x3F_color(0x20, 0x20, 0x20)); // gray

    // OBJECT_INFO array takes up a total of 28*16 = 448 bytes.
    // TERRAIN_INFO array takes up a total of 8*64 = 512 bytes.
    // An ega palette entry takes up a single byte, while
    //  a vga palette entry takes up 3 bytes.
    // And so the structure of the PALETTES section goes as follows:
    // EGA_PAL_ENTRY ega_custom[8];
    // EGA_PAL_ENTRY ega_standard[8];
    // EGA_PAL_ENTRY ega_preview[8];
    // VGA_PAL_ENTRY vga_custom[8];
    // VGA_PAL_ENTRY vga_standard[8];
    // VGA_PAL_ENTRY vga_preview[8];
    defin.seekg(448 + 512 + 3*8);
    // vga_custom auslesen
    for (int loop = 0; loop < 8; ++loop) {
        int r = read_byte(defin);
        int g = read_byte(defin);
        int b = read_byte(defin);
        int c = make_0x3F_color(r, g, b);
        if (c == color[COL_BLACK]) c = makecol(0x00, 0x0A, 0x16);
        palette.push_back(c);
        // palette[7]: So you don't get an additional color to specify,
        // since it is just a duplicate of one of your pickable colors.
        if (loop == 0) palette.push_back(palette[7]);
    }
}



// Neues Bitmap erschaffen und zurueckgeben
BITMAP* GraphicSetL1::new_read_bitmap(
    const Crunch::Section& section,
    const int xl,
    const int yl,
    const int image_loc,
    const int mask_loc
) {
    int data_ptr = image_loc;

    // Buntes Bitmap: 4 Bit pro Pixel
    // Das Bitmap liegt komponentenweise im Speicher, erst ein monochromes
    // Bitmap, was das erste Bit jedes Pixels beschreibt, dann das zweite
    // monochrome Bitmap, was alle zweiten Bits beschreibt usw.,
    // insgesamt gibt es 4 Ebenen.
    // Wir beginnen mit einem BITMAP, das ueberall 0 ist, und addieren
    // 8 pro gesetztem ersten Bit an jedem Pixel, dann 4 pro zweitem usw.
    // Ganz am Ende wird pro Pixel p dann ausgefuehrt: p = palette[p];

    // Gegen Allegro-Bug (?): Wir erzeugen das Bitmap um 1 zu gross in jeder
    // Richtung, nutzen aber den Extraplatz nicht. Am Ende wird b->w - 1,
    // b->h - 1 kopiert auf die doppelte Groesse mit stretch_blit, und ohne
    // den Rand und mit b->w, b->h als Angaben stuertzte es immer ab.
    BITMAP* b = create_bitmap(xl + 1, yl + 1);
    clear_to_color(b, 0);

    int eight_pixel_byte = 0;
    for  (int plane = 0; plane < 5;       ++plane)
     for (int pixel = 0; pixel < xl * yl; ++pixel) {
        const int x = pixel % xl;
        const int y = pixel / xl;
        if (plane != 4) {
            if (pixel % 8 == 0 && data_ptr < (int) section.size())
             eight_pixel_byte = section[data_ptr++];
            const bool c = eight_pixel_byte & (1 << (7 - pixel%8));
            _putpixel32(b, x, y, _getpixel32(b, x, y) + (c << plane));
        }
        else {
            _putpixel32(b, x, y, palette[_getpixel32(b, x, y)]);
        }
    }
    // Monochrome Maske: 1 Bit pro Pixel
    // Die Maske ist 1, wenn der Pixel zum Bitmap gehoert, und
    // 0, wenn es zum Hintergrund gehoert.
    data_ptr = mask_loc;
    for (int pixel = 0; pixel < xl * yl; ++pixel) {
        if (pixel % 8 == 0 && data_ptr < (int) section.size())
         eight_pixel_byte = section[data_ptr++];
        const int  x = pixel % xl;
        const int  y = pixel / xl;
        const bool c = eight_pixel_byte & (1 << (7 - pixel%8));
        if (!c) _putpixel32(b, x, y, color[COL_PINK]);
    }
    // Und alles aufs Doppelte fuer L++ vergroessern
    BITMAP* big = create_bitmap(2 * xl, 2 * yl);
    // Siehe Kommentar "gegen Allegro-Bug (?)" weiter oben
    stretch_blit(b, big, 0, 0, xl, yl, 0, 0, 2*xl, 2*yl);

    destroy_bitmap(b);
    return big;
}



BITMAP* GraphicSetL1::new_read_spec_bitmap(
    const Crunch::Section& section
) {
    int data_ptr = 0;

    std::vector <int> palette;

    // read VGA palette (should actually override end of graphics set palette)
    for (int i = 0; i < 8; ++i)
    {
        int r = section[data_ptr++];
        int g = section[data_ptr++];
        int b = section[data_ptr++];
        int c = make_0x3F_color(r, g, b);
        palette.push_back(c);
    }

    // skip EGA palette
    data_ptr += 8 * 2;

    // special graphics are a constant size
    BITMAP* b = create_bitmap(960, 160);
    clear_to_color(b, color[COL_PINK]);

    // terrain is split into 4 40-scanline chunks
    for (int chunk = 0; chunk < 4; ++chunk) {
        std::vector <Byte> data;

        // 0x80 indicates end of chunk
        while (section[data_ptr] != 0x80) {
            // 0x00 - 0x7f indicates subsequent raw data of n+1 bytes
            if (section[data_ptr] < 0x80) {
                for (int i = section[data_ptr]; i >= 0; --i)
                    data.push_back(section[++data_ptr]);
            }
            // 0x81 - 0xff indicates run of 257-n pixels
            else {
                int n = 0xff - section[data_ptr] + 2;
                data.resize(data.size() + n, section[++data_ptr]);
            }
            ++data_ptr;
        }
        ++data_ptr;

        const int planes = 3;
        const int chunk_w = 960, chunk_h = 40;

        // make sure bit-planed data isn't too small
        if (data.size() < ((chunk_w * chunk_h) * planes) / 8)
            break;

        std::vector <Byte>::iterator data_i[planes];
        for (int i = 0; i < planes; ++i)
            data_i[i] = data.begin() + i * (chunk_w * chunk_h) / 8;

        for (int y = chunk * chunk_h; y < (chunk + 1) * chunk_h; ++y) {
            for (int x = 0; x < chunk_w; x += 8) {
                Byte pixel[8] = {0, 0, 0, 0, 0, 0, 0, 0};

                for (int plane = planes - 1; plane >= 0; --plane) {
                    for (int bit = 0; bit < 8; ++bit) {
                        pixel[bit] <<= 1;
                        pixel[bit] |= (*data_i[plane] & (0x80 >> bit)) ? 1 : 0;
                    }
                    ++data_i[plane];
                }

                for (int p = 0; p < 8; ++p)
                    if (pixel[p] != 0)
                        putpixel(b, x + p, y, palette[pixel[p]]);
            }
        }
    }

    // Und alles aufs Doppelte fuer L++ vergroessern
    BITMAP* big = create_bitmap(2 * b->w, 2 * b->h);
    // Siehe Kommentar "gegen Allegro-Bug (?)" weiter oben
    stretch_blit(b, big, 0, 0, b->w, b->h, 0, 0, big->w, big->h);

    destroy_bitmap(b);

    return big;
}



void GraphicSetL1::make_specials(
    const Crunch::Section&   section,
    const std::vector <int>& waving_flag_ids
) {
    for (int spe_id = 0; spe_id < (int) speinf.size(); ++spe_id) {
        const ObjectInfo& sp = speinf[spe_id];
        // An unused slot is set to all 0s.
        // We assume that after an unsed slot there are only unused slots.
        // Otherwise, the IDs would not be corret anyway as I'd rather not
        // have Object instances for unsused slots.
        if (sp.width == 0 || sp.height == 0) break;

        std::vector <BITMAP*> bitvec(sp.end_animation_frame_index);
        for (int fr = 0; fr < sp.end_animation_frame_index; ++fr)
         bitvec[fr] = new_read_bitmap(section, sp.width, sp.height,
         sp.animation_frames_base_loc + fr * sp.animation_frame_data_size,
         sp.animation_frames_base_loc + fr * sp.animation_frame_data_size
                                      +      sp.mask_offset_from_image);
        Object::Type type    = Object::TERRAIN;
        int          subtype = 0;
        switch (sp.trigger_effect_id) {
            case 0:  type = Object::DECO;                 break;
            case 1:  type = Object::GOAL;                 break;
            case 4:  type = Object::TRAP;                 break;
            case 5:  type = Object::WATER;                break;
            case 6:  type = Object::WATER;   subtype = 1; break;
            case 7:  type = Object::ONEWAY;               break;
            case 8:  type = Object::ONEWAY;  subtype = 1; break;
            case 9:  type = Object::TERRAIN; subtype = 1; break;
            default: type = Object::DECO;                 break;
        }
        if (spe_id == 1 && type == Object::DECO) {
            type = Object::HATCH;
            // Adapt the bitvec to the L++ hatch frame order, which is
            // the same as in L2. Simply move the pointers around without
            // doing anything with the data.
            BITMAP* fully_opened_hatch = bitvec.front();
            for (size_t fr = 0; fr < bitvec.size() - 1; ++fr)
             bitvec[fr] = bitvec[fr + 1];
            bitvec.back() = fully_opened_hatch;
            push_back_special(Object(Cutbit(bitvec), type, subtype));
        }
        else {
            Object ob(Cutbit(bitvec), type, subtype);
            // Wir vermehren alle Werte 4 in der Doku zu 8, weil L++ doppelte
            // Aufloesung benutzt.
            ob.trigger_xc = false;
            ob.trigger_x  = sp.trigger_left   * 8;
            ob.trigger_xl = sp.trigger_width  * 8;
            // Make a better experience by using L++ default y values for
            // any goal. You have to mine in CindyLand now to get in.
            // They don't jump through the roof in the Crystal set.
            if (type != Object::GOAL) {
                ob.trigger_yc = false;
                ob.trigger_y  = sp.trigger_top    * 8 - 8;
                ob.trigger_yl = sp.trigger_height * 8;
            }
            // Trap: Dies aktiviert ueberhaupt erst die Stampfer in
            // Compression Method 1.
            if (type == Object::TRAP) {
                ob.trigger_y  -= 4;
                ob.trigger_yl += 4;
            }
            // Old:
            // ob.trigger_y  -= 8; // 8 ist extrem (Cindyland),
            // ob.trigger_yl += 8; // man teste mal Have An Ice Day.
            // ob.trigger_xl /= 2;

            // Now check if it's a waving flag.
            if (type == Object::DECO) for (std::vector <int> ::const_iterator
             wfitr = waving_flag_ids.begin(); wfitr != waving_flag_ids.end();
             ++wfitr) if (*wfitr == spe_id) {
                ob.subtype = 1;
                break;
            }
            push_back_special(ob);
        }
        // Das Cutbit nimmt den Vektor nicht in Beschlag, anders als bei
        // der Funktion zur Generierung des Terrains, also hier loeschen.
        for (int fr = 0; fr < (int) bitvec.size(); ++fr)
         destroy_bitmap(bitvec[fr]);
    }
}



void GraphicSetL1::make_terrain(
    const Crunch::Section&   section,
    const std::vector <int>& steel_ids
) {
    // Create Cutbit. The cutbit gets to own the BITMAP, so it does not have
    // to be destroyed here via destroy_bitmap(b).
    // We assume that after a non-defined bitmap there are only other null
    // bitmaps.
    for (int ter_id = 0; ter_id < (int) terinf.size(); ++ter_id)
     if (terinf[ter_id].width > 0 && terinf[ter_id].height > 0) {
        BITMAP* b = new_read_bitmap(section,
         terinf[ter_id].width,     terinf[ter_id].height,
         terinf[ter_id].image_loc, terinf[ter_id].mask_loc);
        Object ob(Cutbit(b, false), Object::TERRAIN); // false: don't cut
        // Check for steel
        for (std::vector <int> ::const_iterator steel_itr = steel_ids.begin();
         steel_itr != steel_ids.end(); ++steel_itr)
         if (*steel_itr == ter_id) {
            ob.subtype = 1;
            break;
        }
        push_back_terrain(ob);
    }
}
