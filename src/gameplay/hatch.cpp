#include "hatch.h"
#include "../graphic/gra_lib.h"

const unsigned long GameHatch::update_lets_go        (15);
const unsigned long GameHatch::update_open           (30);
const unsigned long GameHatch::update_blink_stop     (30);
const unsigned long GameHatch::updates_blink_on      ( 4);
const unsigned long GameHatch::updates_blink_off     ( 2);

GameHatch::GameHatch(Torbit& t, const Object* o, int x, int y)
:
    EdGraphic     (t, o, x, y),
    has_style     (false),
    style         (LixEn::GARDEN),
    update_close  (0),
    blink_now     (0),
    x_frames_open (1),
    x_frames_close(1)
{
    if (get_y_frames() == 1) x_frames_close = 0;
    else while (get_frame_exists(x_frames_close, 1)) ++x_frames_close;
    while      (get_frame_exists(x_frames_open,  0)) ++x_frames_open;
}



GameHatch::~GameHatch()
{
}



void GameHatch::animate(EffectManager& effect, const unsigned long u)
{
    if (u < update_open) {
        set_y_frame(0);
        set_x_frame(0);
    }
    else if (update_close > 0 && u >= update_close) {
        set_y_frame(1);
        set_x_frame(u - update_close < static_cast <unsigned long>
         (x_frames_close) ? u - update_close : x_frames_close - 1);
        if (u == update_close)
            effect.add_sound_general(u, Sound::HATCH_CLOSE);
    }
    else {
        // open or just opening
        set_y_frame(0);
        set_x_frame(u - update_open + 1 < static_cast <unsigned long>
         (x_frames_open) ? u - update_open + 1
         : x_frames_open - 1);
        if (u == update_open) effect.add_sound_general(u, Sound::HATCH_OPEN);
    }
    if (u < update_blink_stop) blink_now
     = (u % (updates_blink_on + updates_blink_off) < updates_blink_on);
    else blink_now = false;

    if (u == update_lets_go) effect.add_sound_general(u, Sound::LETS_GO);
}


void GameHatch::draw()
{
    EdGraphic::draw();
    if (blink_now && has_style) {
        const Cutbit& c = GraLib::get_lix(style);
        c.draw(get_ground(),
         get_x() + get_object()->get_trigger_x() - c.get_xl() / 2,
         get_y() + get_object()->get_trigger_y() - c.get_yl() / 2,
         0, LixEn::WALKER - 1);
    }
}
