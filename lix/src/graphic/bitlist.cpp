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

    color[COL_TRANSPARENT  ] = al_map_rgba(255,   0, 255,   0);
    color[COL_PINK         ] = al_map_rgba(255,   0, 255,   0);
    color[COL_PINKAF       ] = al_map_rgba(255,   0, 255,   0);

    color[COL_BLACK        ] = al_map_rgb(  0,   0,   0); // Echte Männerfarbe!
    color[COL_GREY_FUSE_D  ] = al_map_rgb(111, 111, 111); // Etwas dunkl. Grau
    color[COL_GREY_FUSE_L  ] = al_map_rgb(150, 150, 150); // Ein wenig heller
    color[COL_WHITE        ] = al_map_rgb(255, 255, 255); // Weiß
    color[COL_YELLOW       ] = al_map_rgb(255, 255,   0); // Gelb
    color[COL_STEEL_MASK   ] = color[COL_YELLOW ];        // just not pink

    color[COL_SCREEN_BORDER] = al_map_rgb( 20,  16,  12); // Wenn Option gew.
    color[COL_EDITOR_DARK  ] = al_map_rgb( 32,  32,  32); // Loeschobjekte
    color[COL_API_SHADOW   ] = al_map_rgb(108, 100,  77); // Schattentext
    color[COL_API_D        ] = al_map_rgb(160, 143, 116); // Normal
    color[COL_API_M        ] = al_map_rgb(169, 158, 133); //
    color[COL_API_L        ] = al_map_rgb(180, 168, 146); //
    color[COL_API_DOWN_D   ] = al_map_rgb(175, 163, 140); // Halten
    color[COL_API_DOWN_M   ] = al_map_rgb(180, 168, 146); //
    color[COL_API_DOWN_L   ] = al_map_rgb(185, 174, 153); //
    color[COL_API_ON_D     ] = al_map_rgb(192, 183, 165); // Aktiv
    color[COL_API_ON_M     ] = al_map_rgb(204, 199, 181); //
    color[COL_API_ON_L     ] = al_map_rgb(215, 209, 198); //

    color[COL_TEXT         ] = al_map_rgb(223, 219, 210);
    color[COL_TEXT_ON      ] = color[COL_WHITE      ];
    color[COL_TEXT_OFF     ] = color[COL_API_ON_M   ];
    color[COL_TEXT_GREEN   ] = al_map_rgb(128, 255, 128); // winner green

    color[COL_LIXFILE_EYE  ] = al_map_rgb(0x50, 0x50, 0x50); // For fuse offset
}
