// All filename replacements (deprecated names into current names)
// Currently, this is a member of GraLib, not ObjLib.

#include "../graphic/gra_lib.h"

void GraLib::load_all_file_replacements()
{
    add_replace("Universal/water.W",  "matt/water.W"  );
    add_replace("Universal/10tons.T", "matt/10tons.T" );

    add_replace("./bitmap/Rebuilds/06 - Oriental/hatch.H",
                "matt/oriental/Hatch.H");
    add_replace("./bitmap/Rebuilds/04 - Sandy Beach/hatch.H",
                "matt/beach/Hatch.H");

    add_substr_replace("bitmap/", "");
    add_substr_replace("Universal/", "matt/steel/");
    add_substr_replace("Rebuilds/",  "matt/");

    add_substr_replace("01 - Earth", "earth");
    add_substr_replace("02 - Gold Mine", "goldmine");
    add_substr_replace("03 - Carnival", "carnival");
    add_substr_replace("04 - Sandy Beach", "beach");
    add_substr_replace("05 - Winter", "winter");
    add_substr_replace("06 - Oriental", "oriental");
    add_substr_replace("07 - Underworld", "underworld");
    add_substr_replace("08 - Bricks", "bricks");
    add_substr_replace("09 - Marble", "marble");

    add_substr_replace("arnival/Balloons", "arnival/balloons");

    add_substr_replace("/ExplosiveCrate", "/explosivecrate");
    add_substr_replace("/mine cart", "/minecart");
    add_substr_replace("/plank diag",  "/plankdiag");
    add_substr_replace("/plankdiag 2", "/plankdiag2");
    add_substr_replace("/plank support",  "/planksupport");
    add_substr_replace("/planksupport 2", "/planksupport2");
    add_substr_replace("/planksupport 3", "/planksupport3");

    add_substr_replace("iental/asian ", "iental/asian");
    add_substr_replace("iental/bonsai ", "iental/bonsai");
    add_substr_replace("iental/bamboo ", "iental/bamboo_");

    add_substr_replace("Daytime Sand", "daytime");
    add_substr_replace("Nighttime Sand", "nighttime");
    add_substr_replace("Sand Decor", "decor");
}
