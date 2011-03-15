#pragma once

#include "../editor/graph_ed.h"

#include "effect.h"
#include "../lix/lix_enum.h"

class GameHatch : public EdGraphic {

private:

    bool          has_style;
    LixEn::Style  style;

    unsigned long update_close;
    bool          blink_now;

    int           x_frames_open;
    int           x_frames_close;

public:

    GameHatch(Torbit&, const Object*, int = 0, int = 0);
    ~GameHatch();

    static const unsigned long update_lets_go;
    static const unsigned long update_open;
    static const unsigned long update_blink_stop;
    static const unsigned long updates_blink_on;
    static const unsigned long updates_blink_off;

    inline bool get_style()     const     { return style;     }
    inline bool get_has_style() const     { return has_style; }
    inline void set_style(LixEn::Style s) { style = s; has_style = true; }
    inline void set_no_style()            { style = LixEn::GARDEN;
                                             has_style = false; }

    inline void set_update_close(unsigned long u) { update_close = u; }

            void animate(EffectManager&, unsigned long);
    virtual void draw();

};
