#include "panel.h"

#include "../level/level.h" // number of skills per level
#include "../graphic/gra_lib.h"
#include "../other/user.h"

GameplayPanel::GameplayPanel()
:
    Element    (0, LEMSCR_Y - gloB->panel_gameplay_yl,
                   LEMSCR_X,  gloB->panel_gameplay_yl),
    skill      (gloB->skill_max, Api::SkillButton()),
    rate_minus (GraLib::get(gloB->file_bitmap_game_panel_2), 40*12,  0),
    rate_plus  (GraLib::get(gloB->file_bitmap_game_panel_2), 40*13,  0),
    pause      (GraLib::get(gloB->file_bitmap_game_pause  ), 40*12, 20),
    zoom       (GraLib::get(gloB->file_bitmap_game_panel  ), 40*13, 20),
    speed_slow (GraLib::get(gloB->file_bitmap_game_panel  ), 40*13, 50),
    speed_fast (GraLib::get(gloB->file_bitmap_game_panel  ), 40*14, 20),
    speed_turbo(GraLib::get(gloB->file_bitmap_game_panel  ), 40*14, 50),
    state_save (GraLib::get(gloB->file_bitmap_game_panel_2), 40*14,  0),
    state_load (GraLib::get(gloB->file_bitmap_game_panel_2), 40*15,  0),
    restart    (GraLib::get(gloB->file_bitmap_game_panel  ), 40*15, 20),
    nuke_single(GraLib::get(gloB->file_bitmap_game_panel  ), 40*15, 50),
    nuke_multi (GraLib::get(gloB->file_bitmap_game_nuke   ), 40*12, 60),

    stats      (),

    rate_min(rate_minus.get_x() + 27, rate_minus.get_y()),
    rate    (rate_plus .get_x() + 27, rate_plus .get_y())
{
    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr) add_child(*itr);
    add_child(rate_minus);
    add_child(rate_plus);
    add_child(pause);
    add_child(zoom);
    add_child(speed_slow);
    add_child(speed_fast);
    add_child(speed_turbo);
    add_child(state_save);
    add_child(state_load);
    add_child(restart);
    add_child(nuke_single);
    add_child(nuke_multi);

    add_child(stats);
    add_child(rate_min);
    add_child(rate);

    rate_minus.set_x_frame(0);
    rate_plus .set_x_frame(1);
    state_save.set_x_frame(2);
    state_load.set_x_frame(3);

    zoom       .set_x_frame(2);
    speed_slow .set_x_frame(3);
    speed_fast .set_x_frame(4);
    speed_turbo.set_x_frame(5);
    restart    .set_x_frame(8);
    nuke_single.set_x_frame(9);
    nuke_multi .set_x_frame(0);

    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr) itr->set_hot();
    speed_slow.set_warm();
    pause     .set_warm();

    // We will need to be careful when checking for clicked buttons,
    // as there can be more than one skill with the same hotkey in play.
    // This is done in Gameplay's calculcate code for these buttons.
    pause      .set_hotkey(useR->key_pause);
    zoom       .set_hotkey(useR->key_zoom);
    speed_slow .set_hotkey(useR->key_speed_slow);
    speed_fast .set_hotkey(useR->key_speed_fast);
    speed_turbo.set_hotkey(useR->key_speed_turbo);
    state_save .set_hotkey(useR->key_state_save);
    state_load .set_hotkey(useR->key_state_load);
    restart    .set_hotkey(useR->key_restart);
    nuke_single.set_hotkey(useR->key_nuke);
    nuke_multi .set_hotkey(useR->key_nuke);

    rate_min.set_align(Api::Label::CENTERED);
    rate    .set_align(Api::Label::CENTERED);
    rate_min.set_undraw_color(0);
    rate    .set_undraw_color(0);
}



void GameplayPanel::set_mode_single()
{
    mode_single = true;
    hide_all_children(false);
    for (unsigned i = 0; i < skill.size(); ++i) {
        skill[i].set_x(i * 40);
        skill[i].set_y(20);
    }
    nuke_multi.hide();
}



void GameplayPanel::set_mode_network()
{
    mode_single = false;
    rate_minus .hide();
    rate_plus  .hide();
    rate_min   .hide();
    rate       .hide();
    pause      .hide();
    zoom       .hide();
    speed_slow .hide();
    speed_fast .hide();
    speed_turbo.hide();
    state_save .hide();
    state_load .hide();
    restart    .hide();
    nuke_single.hide();
    nuke_multi .show();
    for (unsigned i = 0; i < skill.size(); ++i) {
        skill[i].set_x(i * 40);
        skill[i].set_y(20);
    }
}



void GameplayPanel::set_like_tribe(const Tribe* tr, const Tribe::Master* ma)
{
    if (!tr) return;
    for (size_t i = 0; i < skill.size(); ++i) {
        skill[i].set_style (tr->style);
        skill[i].set_skill (tr->skill[i].ac);
        skill[i].set_number(tr->skill[i].nr);
        skill[i].set_hotkey(useR->key_skill[tr->skill[i].ac]);
    }
    if (ma) set_skill_on(ma->skill_sel);

    rate_min   .set_number(tr->rate_min);
    rate       .set_number(tr->rate);
    nuke_single.set_on    (tr->nuke);
    nuke_multi .set_on    (tr->nuke);

    set_draw_required();
}



void GameplayPanel::set_skill_numbers(const Tribe& tr)
{
    for (size_t i = 0; i < skill.size(); ++i)
     skill[i].set_number(tr.skill[i].nr);
}



void GameplayPanel::set_skill_on(const int which)
{
    if (which < 0 || which >= (int) skill.size()) return;
    for (size_t i = 0; i < skill.size(); ++i) skill[i].set_off();
    if (skill[which].get_number() != 0) skill[which].set_on();
}



void GameplayPanel::calc_self()
{
    rate_minus.set_draw_required();
    rate_plus .set_draw_required();
    rate_min  .set_draw_required();
    rate      .set_draw_required();
}



void GameplayPanel::draw_self()
{
}
