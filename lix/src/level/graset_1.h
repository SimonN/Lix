/*
 * level/graset_1.h
 *
 * This is an L1 graphics set. All of its private things are preparations
 * to make the graphics from the original files.
 *
 */

#pragma once

#include <string>

#include "crunch.h"
#include "graset.h"

class GraphicSetL1 : public GraphicSet {

public:

    GraphicSetL1(const std::string&,        // groundXo.dat
                 const std::string&,        // vgagrX.dat
                 const std::vector <int>&,  // all steel ids
                 const std::vector <int>&); // all waving flag ids

    static BITMAP* new_read_spec_bitmap(const Crunch::Section&);

private:

    struct ObjectInfo {
        int animation_flags;
        int start_animation_frame_index;
        int end_animation_frame_index;
        int width;
        int height;
        int animation_frame_data_size;
        int mask_offset_from_image;
        int trigger_left;
        int trigger_top;
        int trigger_width;
        int trigger_height;
        int trigger_effect_id;
        int animation_frames_base_loc;
        int preview_image_index;
        int trap_sound_effect_id;
    };

    struct TerrainInfo {
        int width;
        int height;
        int image_loc;
        int mask_loc;
    };

    // These vectors are only used while reading in the terrain. Afterwards,
    // everything is stored in std::vector <Object> terrain and special.
    std::vector <ObjectInfo>  speinf;
    std::vector <TerrainInfo> terinf;
    std::vector <int>         palette;

    int  read_byte(std::ifstream& file);
    int  read_word(std::ifstream& file);

    void read_specials(std::ifstream&);
    void read_terrain (std::ifstream&);
    void read_palette (std::ifstream&);

    // make_specials takes a vector of waving flag ids as its 2nd argument,
    // make_terrain takes a vector of steel ids as its 2nd arg.
    void make_specials(const Crunch::Section&, const std::vector <int>&);
    void make_terrain (const Crunch::Section&, const std::vector <int>&);

    BITMAP* new_read_bitmap(const Crunch::Section&, int, int, int, int);

};
