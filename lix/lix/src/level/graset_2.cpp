/*
 * level/graset_2.cpp
 *
 */

#include "../other/myalleg.h"

#include <fstream>
#include <algorithm>

#include "graset_2.h"

#include "../graphic/glob_gfx.h"

GraphicSetL2::GraphicSetL2(const std::string& filename_sty)
{
    Byte* sty_array = read_file(filename_sty);
	if (sty_array == NULL) return;

	setup_file(sty_array);
	delete[] sty_array;

	make_palette();
	make_terrain();
	make_specials();

    sty.clear();
}



// meyrel reads the file into a byte buffer
// and checks whether the file is a valid L2 style file
Byte* GraphicSetL2::read_file(const std::string& filename)
{
	std::ifstream file(filename.c_str(), std::ios::binary);

	// get length of file:
	file.seekg(0, std::ios::end);
	int length = file.tellg();
	if (length < 0x0C) return NULL;
	file.seekg(0, std::ios::beg);

	Byte* buffer = new Byte[length];

	file.read((char*) buffer, length);
	file.close();

	// ensure it's a valid L2 style file and the filesize is correct
	if (memcmp(&buffer[8], "L2VG", 4)
	 || (read_long_be(&buffer[4]) != length - 8)) {
		delete[] buffer;
		return NULL;
	}
	return buffer;
}



// turns the raw binary array into a Crunch::File
void GraphicSetL2::setup_file(const Byte* buffer) {
	sty.clear();

	// init with offset pointing to the size of the first section
	int offset = 0x10;
	for (int i = 0; i < SECTION_MAX; ++i) {
		// offset of the end of this section
		int off_new = offset+4 + read_long_be(&buffer[offset]);
		// +4 because we skip the size field which the offset is pointing to
		Crunch::Section section(&buffer[offset+4], &buffer[off_new]);
		sty.push_back(section);
		//skip the identifying 4 bytes
		offset = off_new + 4;
	}
}



// signed little endian word
int GraphicSetL2::read_word_le(const Crunch::Section& s, const int offset)
{
	int r = 0x100 * s[offset + 1] + s[offset];
	return r < 0x8000 ? r : r-0x10000;
}



// big endian longword
long GraphicSetL2::read_long_be(const Byte* buffer) {
	return         buffer[3]
	 + 0x100     * buffer[2]
	 + 0x10000   * buffer[1]
	 + 0x1000000 * buffer[0];
}



// Nimmt Farben mit 0x3F als Hoechstwert, braucht Allegro
int GraphicSetL2::make_0x3F_color(int r, int g, int b)
{
    return makecol(r * 255 / 63,   g * 255 / 63,   b * 255 / 63);
}



void GraphicSetL2::make_palette()
{
    palette.clear();
	for (int i = 0; i < 0x80; ++i)
		palette.push_back(
			make_0x3F_color(
				sty[L2CL][3*i + 2],
				sty[L2CL][3*i + 3],
				sty[L2CL][3*i + 4]
			)
		);
}



// Neues Bitmap erschaffen und zurueckgeben
BITMAP* GraphicSetL2::new_read_bitmap(
    const Crunch::Section& section,
	const int offset,
    const int width,
    const int height
) {
    // Gegen Allegro-Bug (?): Wir erzeugen das Bitmap um 1 zu gross in jeder
    // Richtung, nutzen aber den Extraplatz nicht. Am Ende wird b->w - 1,
    // b->h - 1 kopiert auf die doppelte Groesse mit stretch_blit, und ohne
    // den Rand und mit b->w, b->h als Angaben stuertzte es immer ab.

	BITMAP* b = create_bitmap(16 * width + 1, 8 * height + 1);
    //clear_to_color(b, 0);
	piece_blit_bitmap(b, section, offset, width, height, 0, 0);

    // Und alles aufs Doppelte fuer L++ vergroessern
    BITMAP* big = create_bitmap(32 * width, 16 * height);

    // Siehe Kommentar "gegen Allegro-Bug (?)" weiter oben
    stretch_blit(b, big, 0, 0, 16 * width, 8 * height, 0, 0, 32 * width, 16 * height);

    destroy_bitmap(b);
    return big;
}


void GraphicSetL2::piece_blit_bitmap(
	BITMAP* b,
    const Crunch::Section& section,
	const int offset,
    const int width,	// sprite coords
    const int height,	// sprite coords
	const int xoff,	// pixel coords
	const int yoff	// pixel coords
) {
	int i = 0;
	for (int y = 0; y < height; ++y)
		for (int x = 0; x < width; x++) {
			sprite_blit_bitmap(b, read_word_le(section, offset + i), 16 * x + xoff, 8 * y + yoff);
			i += 2;
		}
}

// Blits 16x8 sprite defined by the spriteID
// onto the bitmap at the specified position.
// no boundary checking
void GraphicSetL2::sprite_blit_bitmap(
	BITMAP* b,
	const int spriteID,
	const int x,
	const int y
) {
    // 7 Bit per pixel, padded to 8 bit, for a size of 16x8
	int offset = 0x80 * spriteID + 2;

	for (int i = 0; i < 0x80; ++i) {
		if (sty[L2BL][offset + i] != 0)
			putpixel(b, (4 * i) % 16 + i / 32 + x, (i / 4) % 8 + y, palette[sty[L2BL][offset + i]]);
		else
			putpixel(b, (4 * i) % 16 + i / 32 + x, (i / 4) % 8 + y, color[COL_PINK]);
	}
}



// Blits l2ss sprite defined by the offset within l2ss
// onto the bitmap at the specified position.
// no boundary checking
void GraphicSetL2::l2ss_blit_bitmap(
	BITMAP* b,
	const int offBase,
	const int xBase,
	const int yBase
) {
	Crunch::Section s = sty[L2SS];

    // unused variables
	// int w = read_word_le(s, offBase);
	// int h = read_word_le(s, offBase + 2);

	int offs[4];
	for (int i = 0; i < 4; ++i)
		offs[i] = read_word_le(s, offBase + 4 + 2 * i);

	// ****** begin code for blitting L2SS ****** //
	for (int i = 0; i < 4; ++i) {
		int off_curr = offs[i];
        int xPos = i + xBase;
        int yPos = yBase;
		int xPosAdd = 0;
        unsigned char byte = 0;
        int n = 0;
		int m = 0;
		int l = 0;
        int reminderm = false;
        int reminderl = false;
        int reminderl2 = false;
        while (byte != 0xFF) {
            byte = s[off_curr++];
            if (n || m || l) {
				if (byte != 0) putpixel(b, xPos, yPos, palette[byte]);
				else putpixel(b, xPos, yPos, color[COL_PINK]);
                xPos += 4;
                if (n) {
                    n--;
                    if (!n) {
                        yPos++;
                        xPos = i + xBase;
                    }
                }
                if (m) m--;
                if (!m && reminderm) {
                    xPos += 4 * xPosAdd;
                    reminderm = false;
                    xPosAdd = 0;
                }
                if (l) l--;
                if (!l && reminderl) {
                    if (reminderl2)
                    {
                        xPos += 4 * xPosAdd;
                        xPosAdd = 0;
                    } else {
                        yPos++;
                        xPos = i;
                    }
                    reminderl = false;
                    reminderl2 = false;
                }
            } else {
                if (byte==0x00)
                {
                    yPos++;
                    xPos = i + xBase;
                } else if (byte > 0x7F) {
                    if (((byte & 0x0F) > 0x07) && ((byte & 0xF0) == 0xE0)) {
                        xPos += 4 * (byte & 0x0F) - 8;
                        byte = s[off_curr++];
                        l = (byte & 0xF0) / 0x10;
                        reminderl = true;
                        if ((byte & 0x0F)) {
                            reminderl2 = true;
                            xPosAdd = (byte & 0x0F) - 8;
                        }
                    } else {
                        l = (byte & 0x0F);
                        xPos += 4 * ((byte & 0xF0) / 0x10 - 0x08);
                    }
                } else {
                    if (!(byte & 0x0F))
                        n = (byte & 0xF0) / 0x10;
                    else if ((byte & 0x0F) < 0x08)
                        m = ((byte & 0x0F) + (byte & 0xF0) / 0x10);
                    else {
                        m = ((byte & 0xF0) / 0x10);
                        xPosAdd += (byte & 0x0F) - 0x08;
                        reminderm = true;
                    }
                }
            }
        }
    }
	// ****** end code for blitting L2SS ****** //
}



void GraphicSetL2::desuckify_section(SectionType sec) {
	// remove the size entries that screw the offsets in L2BA and L2SS
	int amount;
	amount = read_word_le(sty[sec]);
	int off = 2;
	int off_new = 0;
	for (int j = 0; j < amount; ++j) {
		int size = read_word_le(sty[sec], off);
		for (int i = 0; i < size; ++i) {
			sty[sec][off_new + i] = sty[sec][off + i + 2];
		}
		off_new += size;
		off += size + 2;
	}
	sty[sec].resize(off_new);
}



void GraphicSetL2::make_specials()
{
	desuckify_section(L2BF);
	desuckify_section(L2SS);

	// compile the animation data.
	// these are elementary animations,
	// objects can be composed of multiple of these
	std::vector<Anim> anims;
	int amount;
	amount = read_word_le(sty[L2BI]);
	for (int j = 0; j < amount; ++j) {
		int offset = read_word_le(sty[L2BI], 2*j + 2) + 2;

		Anim anim;
		int frames = read_word_le(sty[L2BA], offset);
		anim.width = read_word_le(sty[L2BA], offset + 2);
		anim.height = read_word_le(sty[L2BA], offset + 4);

		for (int i = 0; i < frames; ++i) {
			anim.offsets.push_back(read_word_le(sty[L2BA], offset + 6 + 2 * i));
		}

		anims.push_back(anim);
	}

	// compile the special animation data.
	// these are elementary animations that can be blitted onto
	// sprite frames
	// the positions/size are not included yet
	std::vector<Anim> ssanims;
	amount = read_word_le(sty[L2SI]);
	for (int j = 0; j < amount; ++j) {
		int offset = read_word_le(sty[L2SI], 2*j + 2) + 2;

		Anim anim;
		int frames = read_word_le(sty[L2SA], offset);

		for (int i = 0; i < frames; ++i) {
			anim.offsets.push_back(read_word_le(sty[L2SA], offset + 2 + 2 * i));
		}

		ssanims.push_back(anim);
	}

	// now make the actual bitmaps and objects
	Crunch::Section s = sty[L2OB];
	int  objects      = read_word_le(s);
	int  offset       = 2;
	bool goal_exists  = false; // to skip the exit with fake steel below

	for (int obj = 0; obj < objects; ++obj) {
		int subs = read_word_le(s, offset);
		int type = read_word_le(s, offset + 2);

		Object::Type obj_type;
		int obj_subtype = 0;
		int special_x = 0;
		int special_y = 0;

		switch (type) {
			case 2:	// entrance
				obj_type = Object::HATCH;
				break;
			case 3:	// exit
                // see comment at declaration of goal_exists
                if (goal_exists) obj_type = Object::EMPTY;
				else {
				    obj_type = Object::GOAL;
				    goal_exists = true;
				}
				break;
			case 4: // trampoline
				obj_type = Object::TRAMPOLINE;
				break;
			case 5: // steel or decoration, determine later
				obj_type = Object::DECO;
				break;
			case 6: // water
				obj_type = Object::WATER;
				break;
			case 9: // triggered trap
				obj_type = Object::TRAP;
				break;
			case 10: // constant trap with affected animation
				obj_type = Object::TRAP;
				obj_subtype = 1;
				// animation information
				special_x = read_word_le(s, offset + 8);
				special_y = read_word_le(s, offset + 10);
				break;
			case 11: // constant trap with constant animation
				obj_type = Object::WATER;
				obj_subtype = 1;
				break;
			case 12: // lemming launchers
				obj_type = Object::FLING;
 				// bit0 indicates to invert the x_vel when the lemming is facing left
				// bit1 indicates that the animation is NOT permanent
 				obj_subtype = s[offset + 10];
				// fling velocities
				special_x = read_word_le(s, offset + 6);
				special_y = read_word_le(s, offset + 8);
                break;
			default:
				obj_type = Object::EMPTY;
				break;
		}

		if (obj_type != Object::EMPTY) {

			// determine the final size and amount of animation frames
			// as well as the trigger area
			int xrel = 0;
			int yrel = 0;
			int txmin = 0x3FFF;
			int txmax = -0x4000;
			int tymin = 0x3FFF;
			int tymax = -0x4000;
			int xmin = 0x3FFF;
			int xmax = 0;
			int ymin = 0x3FFF;
			int ymax = 0;
			int anim_frames = 0;
			for (int sub = 0; sub < subs; ++sub) {

				int off_sub = offset + 20 + 12 * sub;

				// steel is of type 5 and has solidity somewhere
				if (type == 5 && (s[off_sub + 9] & 0x10) == 0x10) {
					obj_type = Object::TERRAIN;
					obj_subtype = 1;
				}

				// calc the relative position of the next subpart
				if (sub != 0) {
					int deltax = read_word_le(s, off_sub + 2);
					int deltay = read_word_le(s, off_sub + 4);
					if (s[off_sub + 1] & 0x40) xrel += deltax;
					else xrel = deltax;
					if (s[off_sub + 1] & 0x80) yrel += deltay;
					else yrel = deltay;
				}
				xmin = std::min(xmin, xrel);
				ymin = std::min(ymin, yrel);

				int anim = s[off_sub + 0x0a];
				// not invisible sprite?
				if (!(s[off_sub + 0x0b] & 0x80)) {
					// L2SS data?
					if ((s[off_sub + 0x0b] & 0x20)) {
						int frames = ssanims[anim].offsets.size();

						anim_frames = std::max(anim_frames, frames);
						// determine the largest of these tiles
						for (int i = 0; i < frames; ++i) {
							int l2sf_off = ssanims[anim].offsets[i%frames];
							int xpos = read_word_le(sty[L2SF], l2sf_off + 2);	// might need to add xrel
							int ypos = read_word_le(sty[L2SF], l2sf_off + 4);	// might need to add yrel
							int l2ss_off = read_word_le(sty[L2SF], l2sf_off + 6);

							int w = read_word_le(sty[L2SS], l2ss_off);
							int h = read_word_le(sty[L2SS], l2ss_off + 2);

						    xmax = std::max(xmax, xpos + w);
							ymax = std::max(ymax, ypos + h);
						}
					} else { // normal data
						// update maxima for the bitmap
						xmax = std::max(xmax, xrel + 16 * anims[anim].width);
						ymax = std::max(ymax, yrel +  8 * anims[anim].height);

						// technically it'd have to be the lowest common multiple,
						// however I think there's no instance in L2 where these
						// two estimates would differ
						anim_frames = std::max((int) anim_frames, (int) anims[anim].offsets.size());
					}
				}

				int trigger = read_word_le(s, off_sub + 7);
				// check if we found an active trigger area
				if ((trigger & 0x18) == 0x10
				 || ((trigger & 0x18) == 0x08
				 && s[off_sub] >= 0x06
				 && s[off_sub] <= 0x0C)) {
					int radius = 0;
					switch (trigger & 0x3000) {
						case 0x0000:	// full tile
							radius = 0;
							txmin = std::min(txmin, xrel);
							txmax = std::max(txmax, xrel + 16);
							tymin = std::min(tymin, yrel);
							tymax = std::max(tymax, yrel + 8);
							break;
						case 0x1000:	// 1x1 teeny-tiny area
							radius = 2; // -> we increase it to 4x4
							break;
						case 0x2000:	// 5x5 area -> lets turn it into 6x6
							radius = 3;
							break;
						case 0x3000:	// 9x9 area
							radius = 5;
							break;
					}

					int txoff = (trigger & 0x1E0) >> 5;
					int tyoff = (trigger & 0xA00) >> 9;

					// technically, the trigger area should be cropped off at
					// the 16x8 tile boundaries. We however ignore this here.
					txmin = std::min(txmin, xrel + txoff - radius);
					txmax = std::max(txmax, xrel + txoff + radius);
					tymin = std::min(tymin, yrel + tyoff - radius);
					tymax = std::max(tymax, yrel + tyoff + radius);
				}
			}

			// now make the actual bitvector
			std::vector<BITMAP*> bitvec(anim_frames);
			for (int i = 0; i < anim_frames; ++i) {
			    BITMAP* b = create_bitmap(xmax - xmin + 1, ymax - ymin + 1);
			    clear_to_color(b, color[COL_PINK]);
				bitvec[i] = b;
			}

			xrel = 0;
			yrel = 0;
			for (int sub = 0; sub < subs; ++sub) {
				int off_sub = offset + 20 + 12 * sub;

				// calc the relative position of the next subpart
				if (sub != 0) {
					int deltax = read_word_le(s, off_sub + 2);
					int deltay = read_word_le(s, off_sub + 4);
					if (s[off_sub + 1] & 0x40) xrel += deltax;
					else xrel = deltax;
					if (s[off_sub + 1] & 0x80) yrel += deltay;
					else yrel = deltay;
				}

				int anim = s[off_sub + 0x0a];
				// not invisible sprite?
				if (!(s[off_sub + 0x0b] & 0x80)) {
					// L2SS data?
					if ((s[off_sub + 0x0b] & 0x20)) {
						int frames = ssanims[anim].offsets.size();
						for (int i = 0; i < anim_frames; ++i) {
							int l2sf_off = ssanims[anim].offsets[i%frames];
							int xpos = read_word_le(sty[L2SF], l2sf_off + 2);	// might need to add xrel
							int ypos = read_word_le(sty[L2SF], l2sf_off + 4);	// might need to add yrel
							int l2ss_off = read_word_le(sty[L2SF], l2sf_off + 6);
							l2ss_blit_bitmap(bitvec[i], l2ss_off, xpos - xmin, ypos - ymin);
						}
					} else { // normal data
						int frames = anims[anim].offsets.size();
						for (int i = 0; i < anim_frames; ++i) {
							piece_blit_bitmap(bitvec[i], sty[L2BF], anims[anim].offsets[i%frames],
									anims[anim].width, anims[anim].height,
									xrel - xmin, yrel - ymin);
						}
					}
				}
			}

			for (int i = 0; i < anim_frames; ++i) {
				BITMAP* big = create_bitmap(2*bitvec[i]->w, 2*bitvec[i]->h);
				// Siehe Kommentar "gegen Allegro-Bug (?)" weiter oben
				stretch_blit(bitvec[i], big, 0, 0,
				 bitvec[i]->w, bitvec[i]->h, 0, 0, big->w, big->h);
				bitvec[i] = big;
			}

			Object ob(Cutbit(bitvec), obj_type, obj_subtype);

			// set off by bitmap base position and stretch by 2
            // geoo wrote:
            // I noticed a little issue with the field sized trigger areas
            // being slightly too low, as it seems that a Lemming is
            // positioned at the low-res pixel on the ground, and not at
            // the pixel within the terrain (like in L1 and L2), which made the
            // Sports ball and Space robot trap not work. Thus, we shift up
            // here by 2 high-res pixels.
			txmin = 2 * (txmin - xmin);
			txmax = 2 * (txmax - xmin);
			tymin = 2 * (tymin - ymin) - 2;
			tymax = 2 * (tymax - ymin) - 2;

			// assign trigger area
			if (txmax > txmin && tymax > tymin) {
				ob.trigger_x = txmin;
				ob.trigger_y = tymin;
				ob.trigger_xl = txmax - txmin;
				ob.trigger_yl = tymax - tymin;
			}

            // Assign special_x/y. Multiply by 2 for Flingers.
			ob.special_x = special_x * (obj_type == Object::FLING ? 2 : 1);
			ob.special_y = special_y * (obj_type == Object::FLING ? 2 : 1);

			// steel gets into the terrain list
			if (obj_type == Object::TERRAIN && obj_subtype == 1) {
				push_back_terrain(ob);
                // for consistency with the IDs
                push_back_special(Object(Cutbit(), Object::EMPTY, 0));
			}
			else
				push_back_special(ob);

			// apparently this data can be removed...
		    for (int fr = 0; fr < (int) bitvec.size(); ++fr)
		        destroy_bitmap(bitvec[fr]);

		} else {
			// insert dummy object
			push_back_special(Object(Cutbit(), Object::EMPTY, 0));
		}

		offset += 20 + 12 * subs;
	}

}



void GraphicSetL2::make_terrain()
{
    // Create Cutbit. The cutbit gets to own the BITMAP, so it does not have
    // to be destroyed here via destroy_bitmap(b).
    // We assume that after a non-defined bitmap there are only other null
    // bitmaps.
	Crunch::Section s = sty[L2BE];
	int tiles  = read_word_le(s) - 1;
	int offset = 2;
    for (int i = 0; i < tiles; ++i) {
		//size of the tile arrangement data
		offset += read_word_le(s, offset + 4);
		// section, offset to the tile arrangement data, x, y
        BITMAP* b = new_read_bitmap(s, offset + 6, s[offset+2], s[offset+3]);
        Object ob(Cutbit(b, false)); // false: nicht zuschneiden
        ob.type    = Object::TERRAIN;
        ob.subtype = 0;
        push_back_terrain(ob);
	}
}



// kate: tab-width 4;
