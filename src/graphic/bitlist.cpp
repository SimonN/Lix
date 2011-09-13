/*
 * graphic/bitlist.cpp
 *
 * Alle laengeren Listen, die bei neuem (seltenen) Bedarf erweitert werden.
 *
 */

#include "glob_gfx.h"

#include "../lix/lix_enum.h"

static int make_sepia(const double light)
{
    if      (light <= 0)      return makecol(0,    0,    0   );
    else if (light >= 0x1000) return makecol(0xFF, 0xFF, 0xFF);
    int r = 0xA0;
    int g = 0x94;
    int b = 0x68;
    if      (light == 0x800) return makecol (r, g, b);
    else if (light <  0x800) return makecol (r * light / 0x800,
                                             g * light / 0x800,
                                             b * light / 0x800);
    else return makecol(r + (255 - r) * (light - 0x800) / 0x800,
                        g + (255 - g) * (light - 0x800) / 0x800,
                        b + (255 - b) * (light - 0x800) / 0x800);
}

void make_all_colors()
{
    // Wir brauchen zwei verschiedene Sorten von Transparenz. -1 ist als
    // Argument für Text auf transparentem Hintergrund anzugeben, während
    // das knallige Girlie-Pink für Bitmap-Blitterei erforderlich ist.

    color[COL_TRANSPARENT    ] = -1;                     // Ich seh' nix o_O
    color[COL_PINK           ] = makecol(255,   0, 255); // Immer noch nix O_O

    color[COL_BLACK          ] = makecol(  0,   0,   0); // Echte Männerfarbe!
    color[COL_GREY_FUSE_D    ] = makecol(111, 111, 111); // Etwas dunkler
    color[COL_GREY_FUSE_L    ] = makecol(150, 150, 150); // Ein wenig heller
    color[COL_WHITE          ] = makecol(255, 255, 255); // Weiß
    color[COL_YELLOW         ] = makecol(255, 255,   0); // Gelb
    color[COL_STEEL_MASK     ] = color[COL_YELLOW     ]; // nicht pink

    color[COL_SCREEN_BORDER  ] = makecol(0x20, 0x1C, 0x14); // Wenn Option gew.
    color[COL_EDITOR_DARK    ] = makecol(0x20, 0x20, 0x20); // Loeschobjekte
    color[COL_API_SHADOW     ] = make_sepia(0x300);
    color[COL_API_D          ] = make_sepia(0x7C0 / 1.2);
    color[COL_API_M          ] = make_sepia(0x7C0);
    color[COL_API_L          ] = make_sepia(0x7C0 * 1.2);
    color[COL_API_DOWN_D     ] = make_sepia(0x8C0 / 1.1);
    color[COL_API_DOWN_M     ] = make_sepia(0x8C0);
    color[COL_API_DOWN_L     ] = make_sepia(0x8C0 * 1.1);
    color[COL_API_ON_D       ] = make_sepia(0xB00 / 1.1);
    color[COL_API_ON_M       ] = make_sepia(0xB00);
    color[COL_API_ON_L       ] = make_sepia(0xB00 * 1.1);

    color[COL_API_FILE_SHADOW] = makecol(0x40, 0x40, 0x40);
    color[COL_API_FILE_D     ] = makecol(0x80, 0x80, 0x80);
    color[COL_API_FILE_M     ] = makecol(0xC0, 0xC0, 0xC0);
    color[COL_API_FILE_L     ] = makecol(0xFF, 0xFF, 0xFF);
    color[COL_API_PIC_D      ] = make_sepia(0xB00 / 1.2);
    color[COL_API_PIC_M      ] = make_sepia(0xB00);
    color[COL_API_PIC_L      ] = make_sepia(0xB00 * 1.2);
    color[COL_API_PIC_ON_D   ] = make_sepia(0xE00 / 1.2);
    color[COL_API_PIC_ON_M   ] = make_sepia(0xE00);
    color[COL_API_PIC_ON_L   ] = make_sepia(0x1000);

    color[COL_TEXT           ] = make_sepia(0xE00);
    color[COL_TEXT_ON        ] = color[COL_API_PIC_ON_L];
    color[COL_TEXT_GREEN     ] = makecol(128, 255, 128); // Gewinner-Gruen

    color[COL_LIXFILE_EYE    ] = makecol(0x50, 0x50, 0x50); // For fuse offset
}
