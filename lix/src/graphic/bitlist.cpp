/*
 * graphic/bitlist.cpp
 *
 * Alle laengeren Listen, die bei neuem (seltenen) Bedarf erweitert werden.
 *
 */

#include "glob_gfx.h"

#include "../lix/lix_enum.h"

void make_all_colors()
{
    // Wir brauchen zwei verschiedene Sorten von Transparenz. -1 ist als
    // Argument für Text auf transparentem Hintergrund anzugeben, während
    // das knallige Girlie-Pink für Bitmap-Blitterei erforderlich ist.

    color[COL_TRANSPARENT  ] = -1;                     // Ich seh' nix o_O
    color[COL_PINK         ] = makecol(255,   0, 255); // Immer noch nix O_O

    color[COL_BLACK        ] = makecol(  0,   0,   0); // Echte Männerfarbe!
    color[COL_GREY_FUSE_D  ] = makecol(111, 111, 111); // Etwas dunkleres Grau
    color[COL_GREY_FUSE_L  ] = makecol(150, 150, 150); // Ein wenig heller
    color[COL_WHITE        ] = makecol(255, 255, 255); // Weiß
    color[COL_YELLOW       ] = makecol(255, 255,   0); // Gelb
    color[COL_STEEL_MASK   ] = color[COL_YELLOW     ]; // lediglich nicht pink

    color[COL_SCREEN_BORDER] = makecol( 20,  16,  12); // Wenn Option gewaehlt
    color[COL_EDITOR_DARK  ] = makecol( 32,  32,  32); // Loeschobjekte
    color[COL_API_SHADOW   ] = makecol(108, 100,  77); // Schattentext
    color[COL_API_D        ] = makecol(160, 143, 116); // Normal
    color[COL_API_M        ] = makecol(169, 158, 133); //
    color[COL_API_L        ] = makecol(180, 168, 146); //
    color[COL_API_DOWN_D   ] = makecol(175, 163, 140); // Halten
    color[COL_API_DOWN_M   ] = makecol(180, 168, 146); //
    color[COL_API_DOWN_L   ] = makecol(185, 174, 153); //
    color[COL_API_ON_D     ] = makecol(192, 183, 165); // Aktiv
    color[COL_API_ON_M     ] = makecol(204, 199, 181); //
    color[COL_API_ON_L     ] = makecol(215, 209, 198); //

    color[COL_TEXT         ] = makecol(223, 219, 210);
    color[COL_TEXT_ON      ] = color[COL_WHITE      ];
    color[COL_TEXT_OFF     ] = color[COL_API_ON_M   ];
    color[COL_TEXT_GREEN   ] = makecol(128, 255, 128); // Gewinner-Gruen

    color[COL_LIXFILE_EYE  ] = makecol(0x50, 0x50, 0x50); // For fuse offset
}
