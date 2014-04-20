#include "panel.h"

#include "../../level/level.h" // number of skills per level
#include "../../graphic/gra_lib.h"

#include "../../other/language.h"
#include "../../other/user.h"

GameplayPanel::GameplayPanel()
:
    Element    (0, LEMSCR_Y - gloB->panel_gameplay_yl,
                   LEMSCR_X,  gloB->panel_gameplay_yl),
    skill      (gloB->skill_max, Api::SkillButton()),
    rate_minus (GraLib::get(gloB->file_bitmap_game_panel_2), 40*12,  0),
    rate_plus  (GraLib::get(gloB->file_bitmap_game_panel_2), 40*13,  0),
    rate_fixed (GraLib::get(gloB->file_bitmap_game_spi_fix), 40*12,  0),
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
    spec_tribe (40*12, 60, 40*4),
    stats      (),
    rate_slow  (rate_minus.get_x() + 27, rate_minus.get_y()),
    rate_cur   (rate_plus .get_x() + 27, rate_plus .get_y()),
    rate_fast  (rate_fixed.get_x() + 57, rate_fixed.get_y()),

    on_hint_change(0),
    on_hint_change_where(0),

    gapamode   (GM_NONE),
    hint_size  (0),
    hint_cur   (0),
    hint_big   (GraLib::get(gloB->file_bitmap_game_panel),       40*13,    20),
    hint_plus  (GraLib::get(gloB->file_bitmap_game_panel_hints), 40*13+20, 20),
    hint_minus (GraLib::get(gloB->file_bitmap_game_panel_hints), 40*13,    20)
{
    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr) add_child(*itr);
    add_child(rate_minus);
    add_child(rate_plus);
    add_child(rate_fixed);
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
    add_child(spec_tribe);

    add_child(stats);
    add_child(rate_slow);
    add_child(rate_cur);
    add_child(rate_fast);

    add_child(hint_big);
    add_child(hint_plus);
    add_child(hint_minus);

    for (unsigned i = 0; i < skill.size(); ++i) {
        skill[i].set_x(i * 40);
        skill[i].set_y(20);
    }

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

    hint_big   .set_x_frame(1);
    hint_plus  .set_x_frame(1);
    hint_minus .set_x_frame(0);

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
    spec_tribe .set_hotkey(useR->key_spec_tribe);

    rate_slow.set_align(Api::Label::CENTERED);
    rate_cur .set_align(Api::Label::CENTERED);
    rate_fast.set_align(Api::Label::CENTERED);
    rate_slow.set_undraw_color(0);
    rate_cur .set_undraw_color(0);
    rate_fast.set_undraw_color(0);


}



void GameplayPanel::set_gapamode_and_hints(const GapaMode m, const int hs)
{
    if (m == gapamode || m == GM_NONE) return;

    gapamode = m;
    hide_all_children(false);
    stats.set_gapamode(gapamode);

    if (gapamode == GM_PLAY_SINGLE
     || gapamode == GM_REPLAY_SINGLE) {
        nuke_multi.hide();
        spec_tribe.hide();
        if (rate_slow.get_text() == rate_fast.get_text()) {
            rate_minus.hide();
            rate_plus .hide();
            rate_slow .hide();
            rate_cur  .hide();
        }
        else {
            rate_fixed.hide();
            rate_fast .hide();
        }
    }
    else if (gapamode == GM_REPLAY_MULTI) {
        rate_minus.hide();
        rate_plus .hide();
        rate_fixed.hide();
        rate_slow .hide();
        rate_cur  .hide();
        rate_fast .hide();
        nuke_multi.hide(); // we use most of the singleplayer interface
        spec_tribe.set_x(40*12);
        spec_tribe.set_y(0);
        spec_tribe.set_xl(40*2);
    }
    else if (gapamode == GM_PLAY_MULTI
     ||      gapamode == GM_SPEC_MULTI) {
        rate_minus .hide();
        rate_plus  .hide();
        rate_fixed .hide();
        rate_slow  .hide();
        rate_cur   .hide();
        rate_fast  .hide();
        pause      .hide();
        zoom       .hide();
        speed_slow .hide();
        speed_fast .hide();
        speed_turbo.hide();
        state_save .hide();
        state_load .hide();
        restart    .hide();
        nuke_single.hide();
        if (gapamode == GM_PLAY_MULTI) spec_tribe.hide();
        else {
            nuke_multi.hide();
            spec_tribe.set_x(40*12);
            spec_tribe.set_y(60);
            spec_tribe.set_xl(40*4);
        }
    }

    // Hint size
    if (hs < 0) hint_size = 0;
    else        hint_size = hs;
    if (hint_size > 0 && hint_cur >= hint_size) hint_cur = hint_size - 1;
    set_hint_cur(hint_cur);
}



void GameplayPanel::set_like_tribe(const Tribe* tr, const Tribe::Master* ma)
{
    if (!tr) return;
    for (size_t i = 0; i < skill.size(); ++i) {
        const int key = useR->key_skill[tr->skill[i].ac];
        skill[i].set_style (tr->style);
        skill[i].set_skill (tr->skill[i].ac);
        skill[i].set_number(tr->skill[i].nr);
        skill[i].set_hotkey(key);

        if (useR->gameplay_help && key != 0)
            skill[i].set_hotkey_label(Help::scancode_to_string(key));
    }
    if (ma) set_skill_on(ma->skill_sel);

    stats.set_tribe_local(tr);

    rate_slow  .set_number(tr->spawnint_slow);
    rate_cur   .set_number(tr->spawnint);
    rate_fast  .set_number(tr->spawnint_fast);
    nuke_single.set_on    (tr->nuke);
    nuke_multi .set_on    (tr->nuke);
    spec_tribe .set_text  (tr->get_name());

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



void GameplayPanel::set_hint_cur(const int i)
{
    if      (i < 0 || hint_size == 0) hint_cur = 0;
    else if (i >= hint_size)          hint_cur = hint_size - 1;
    else                              hint_cur = i;

    if (gapamode == GM_PLAY_SINGLE || gapamode == GM_REPLAY_SINGLE
                                   || gapamode == GM_REPLAY_MULTI) {
        // if there are no hints, maybe only the tutorial
        if (hint_size < 2) {
            zoom.show();
            hint_big.hide();
            hint_plus.hide();
            hint_minus.hide();
        }
        // with hints, if the hint is either deactivated or the last one
        else if (hint_cur == 0 || hint_cur == hint_size - 1) {
            zoom.hide();
            hint_big.show();
            hint_plus.hide();
            hint_minus.hide();
            hint_big.set_on(hint_cur != 0);
        }
        // with hints, showing an intermediate hint
        else {
            zoom.hide();
            hint_big.hide();
            hint_plus.show();
            hint_minus.show();
        }
    }
    // gapamode is GM_PLAY_MULTI or GM_SPEC_MULTI
    else {
        zoom.hide();
        hint_big.hide();
        hint_plus.hide();
        hint_minus.hide();
    }

    if (on_hint_change) on_hint_change(on_hint_change_where, hint_cur);

    set_draw_required();
}



// ############################################################################
// ############################################################################
// ############################################################################



// helper function for calc_self()
static void fhs(
    std::string& target_s, int& target_i,
    const Api::Element& e, const std::string& s, const int& i = 0
) {
    if (e.is_mouse_here()) {
        target_s = s;
        target_i = i;
    }
}



void GameplayPanel::calc_self()
{
    rate_fixed.set_down(false);

    rate_minus.set_draw_required();
    rate_plus .set_draw_required();
    rate_fixed.set_draw_required();
    rate_slow .set_draw_required();
    rate_cur  .set_draw_required();
    rate_fast .set_draw_required();

    if      (hint_big  .get_clicked()) set_hint_cur(hint_cur==0?1:hint_size-2);
    else if (hint_minus.get_clicked()) set_hint_cur(hint_cur - 1);
    else if (hint_plus .get_clicked()) set_hint_cur(hint_cur + 1);

    if (useR->gameplay_help) {
        std::string str;
        int         key = 0;
        using namespace Language;
        fhs(str, key, rate_minus,  gameplay_rate_minus,  useR->key_rate_minus);
        fhs(str, key, rate_plus,   gameplay_rate_plus,   useR->key_rate_plus);
        fhs(str, key, pause,       gameplay_pause,       useR->key_pause);
        fhs(str, key, zoom,        gameplay_zoom,        useR->key_zoom);
        fhs(str, key, speed_slow,  gameplay_speed_slow,  useR->key_speed_slow);
        fhs(str, key, speed_fast,  gameplay_speed_fast,  useR->key_speed_fast);
        fhs(str, key, speed_turbo, gameplay_speed_turbo, useR->key_speed_turbo);
        fhs(str, key, state_save,  gameplay_state_save,  useR->key_state_save);
        fhs(str, key, state_load,  gameplay_state_load,  useR->key_state_load);
        fhs(str, key, restart,     gameplay_restart,     useR->key_restart);
        fhs(str, key, nuke_single, gameplay_nuke,        useR->key_nuke);
        fhs(str, key, nuke_multi,  gameplay_nuke,        useR->key_nuke);
        fhs(str, key, hint_big,    hint_cur == 0 ? gameplay_hint_first
                                 : hint_cur == 1 ? gameplay_hint_off
                                 :                 gameplay_hint_prev);
        fhs(str, key, hint_minus,  hint_cur == 1 ? gameplay_hint_off
                                                 : gameplay_hint_prev);
        fhs(str, key, hint_plus,                   gameplay_hint_next);
        // some code copied from editor/editor_d.cpp
        if (key) {
            str += " ";
            str += ::Language::editor_hotkey;
            str += " [";
            str += Help::scancode_to_string(key);
            str += "]";
        }
        if (! str.empty()) stats.set_help(str);
        else stats.set_help();
    }
}



void GameplayPanel::draw_self()
{
}
