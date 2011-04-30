/*
 * level/graset_2.h
 *
 * A graphics set from Lemmings 2: The Tribes.
 *
 */

#pragma once

#include <vector>
#include <string>

#include "crunch.h"
#include "graset.h"

class GraphicSetL2 : public GraphicSet {

public:

    GraphicSetL2(const std::string&); // L2 Style
    inline ~GraphicSetL2() {}

private:

    enum SectionType {
		L2CL,	// (palette)
		L2SS,	// (special-object sprites)
		L2SF,	// (special-object-frame data)
		L2SA,	// (special-object-frame data pointers)
		L2SI,	// (special-object-frame arrangement pointers)
		L2BE,	// (terrain tile arrangement)
		L2OB,	// (object information)
		L2BF,	// (animation-frame data)
		L2BA,	// (animation-frame data pointers)
		L2BI,	// (animation-frame arrangement pointers)
		L2BL,	// (8*16 sprites)
		L2BS,	// (preview 2*1 sprites)
		SECTION_MAX
	};

	struct Anim {
		int width;  // in 16x8 sprites
		int height; // in 16x8 sprites
		std::vector<int> offsets;
		//constuctor and destructor implicit
	};

	Crunch::File sty;

	Byte* read_file(const std::string& filename);
	void setup_file(const Byte* buffer);

    int  make_0x3F_color(int, int, int);
    int  read_word_le   (const Crunch::Section&, int offset = 0);
    long read_long_be   (const Byte*);

    std::vector <int> palette;

	void make_palette ();
    void make_specials();
    void make_terrain ();

	void desuckify_section(SectionType);

	BITMAP* new_read_bitmap(const Crunch::Section&, int, int, int);
	void sprite_blit_bitmap(BITMAP*, int, int, int);
	void piece_blit_bitmap (BITMAP*, const Crunch::Section&,
	                                 int, int, int, int, int);
	void l2ss_blit_bitmap  (BITMAP*, int, int, int);

};

// kate: tab-width 4;
