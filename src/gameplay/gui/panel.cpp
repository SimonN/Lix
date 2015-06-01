#include "panel.h"

#include "../../level/level.h" // number of skills per level
#include "../../graphic/gra_lib.h"

#include "../../other/language.h"
#include "../../other/user.h"
#include "../../other/help.h" // timer ticks for the nuke button

static const int xl_tec = 34;
static const int x_tec = 14*36; // skill buttons wider than technical buttons

static const int x_pau = x_tec + 3*xl_tec;
static const int x_zzz = x_tec + 0*xl_tec;
static const int x_fwd = x_tec + 1*xl_tec;
static const int x_nuk = x_tec + 2*xl_tec;

#define BMP_PANEL GraLib::get(gloB->file_bitmap_game_panel)
#define BMP_PAN_2 GraLib::get(gloB->file_bitmap_game_panel_2)
#define BMP_PAN_F GraLib::get(gloB->file_bitmap_game_spi_fix)
#define BMP_PAUSE GraLib::get(gloB->file_bitmap_game_pause)
#define BMP_NUKE  GraLib::get(gloB->file_bitmap_game_nuke)
#define BMP_HINTS GraLib::get(gloB->file_bitmap_game_panel_hints)

GameplayPanel::GameplayPanel()
:
    Element    (0, LEMSCR_Y - gloB->panel_gameplay_yl,
                   LEMSCR_X,  gloB->panel_gameplay_yl),
    skill      (14, Api::SkillButton()), // TODO: allow for 14 skills generally
    rate_minus (BMP_PAN_2, x_zzz,  0, xl_tec, 20),
    rate_plus  (BMP_PAN_2, x_fwd,  0, xl_tec, 20),
    rate_fixed (BMP_PAN_F, x_zzz,  0, 2 * xl_tec, 20),
    pause      (BMP_PAUSE, x_pau, 20, xl_tec, 60),
    zoom       (BMP_PANEL, x_zzz, 20, xl_tec, 30),
    speed_slow (BMP_PANEL, x_zzz, 50, xl_tec, 30),
    speed_fast (BMP_PANEL, x_fwd, 20, xl_tec, 30),
    speed_turbo(BMP_PANEL, x_fwd, 50, xl_tec, 30),
    state_save (BMP_PAN_2, x_nuk,  0, xl_tec, 20),
    state_load (BMP_PAN_2, x_pau,  0, xl_tec, 20),
    restart    (BMP_PANEL, x_nuk, 20, xl_tec, 30),
    nuke_single(BMP_PANEL, x_nuk, 50, xl_tec, 30),
    nuke_multi (BMP_NUKE , x_tec, 60, 4 * xl_tec, 20),
    spec_tribe (x_pau, 60, 4 * xl_tec), // will be reset below
    stats      (),
    rate_slow  (rate_minus.get_x()+rate_minus.get_xl()-13, rate_minus.get_y()),
    rate_cur   (rate_plus .get_x()+rate_minus.get_xl()-13, rate_plus .get_y()),
    rate_fast  (rate_fixed.get_x()+rate_fixed.get_xl()-18, rate_fixed.get_y()),

    on_hint_change(0),
    on_hint_change_where(0),

    gapamode   (GM_NONE),

    nuke_doubleclicked(false),
    timer_tick_nuke_single(0),

    hint_size  (0),
    hint_cur   (0),
    hint_big   (BMP_PANEL, x_zzz,            20, xl_tec, 20),
    hint_plus  (BMP_HINTS, x_zzz + xl_tec/2, 20, xl_tec, 20),
    hint_minus (BMP_HINTS, x_zzz,            20, xl_tec, 20)
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
        skill[i].set_x(i * skill[i].get_xl());
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
        spec_tribe.set_x(x_tec);
        spec_tribe.set_y(0);
        spec_tribe.set_xl(2*xl_tec);
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
            spec_tribe.set_x(x_tec);
            spec_tribe.set_y(60);
            spec_tribe.set_xl(4*xl_tec);
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

    // clear all panels
    for (size_t i = 0; i < skill.size(); ++i) {
        skill[i].set_skill(LixEn::NOTHING);
        skill[i].set_replay_id(-1);
    }

    // Create, then sort buttons. Hotkeys will be done after sorting
    for (size_t i = 0; i < tr->skill.size(); ++i) {
        size_t j = skill.size();
        switch (tr->skill[i].ac) {
            case LixEn::WALKER: j = 0; break;
            case LixEn::JUMPER: j = 1; break;
            case LixEn::RUNNER: j = 2; break;
            case LixEn::CLIMBER: j = 3; break;
            case LixEn::FLOATER: j = 4; break;
            case LixEn::EXPLODER: j = 5; break; // panel 5
            case LixEn::EXPLODER2: j = 5; break; // same panel 5
            case LixEn::BATTER: j = 6; break;
            case LixEn::BLOCKER: j = 7; break;
            case LixEn::CUBER: j = 8; break;
            case LixEn::BUILDER: j = 9; break;
            case LixEn::PLATFORMER: j = 10; break;
            case LixEn::BASHER: j = 11; break;
            case LixEn::MINER: j = 12; break;
            case LixEn::DIGGER: j = 13; break;
            default: break;
        }
        if (j != skill.size()) {
            skill[j].set_style (tr->style);
            skill[j].set_skill (tr->skill[i].ac);
            skill[j].set_number(tr->skill[i].nr);
            skill[j].set_replay_id(i);
        }
    }

    // Hotkeys depend on the shown order, not on the replay-internal order.
    // So, we don't have to examine (skill button).replay_id here.
    for (size_t i = 0; i < skill.size(); ++i) {
        const int key = useR->key_skill[skill[i].get_skill()];
        skill[i].set_hotkey(key);
        if (useR->gameplay_help && key != 0)
            skill[i].set_hotkey_label(Help::scancode_to_string(key));
        skill[i].set_draw_required();
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



GameplayPanel::SkBIt GameplayPanel::button_by_replay_id(const int id)
{
    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr)
        if (itr->get_replay_id() == id)
            return itr;
    return skill.end();
}



void GameplayPanel::set_skill_numbers(const Tribe& tr)
{
    for (size_t i = 0; i < skill.size(); ++i) {
        int repid = skill[i].get_replay_id();
        if (repid >= 0 && repid < static_cast <int> (tr.skill.size()))
            skill[i].set_number(tr.skill[repid].nr);
    }
}



void GameplayPanel::set_skill_on(const int id)
{
    for (size_t i = 0; i < skill.size(); ++i)
        skill[i].set_off();

    SkBIt b = button_by_replay_id(id);
    if (b != skill.end() && b->get_number() != 0)
        b->set_on();
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

    // the nuke button
    // Gameplay will try to not check the nuke button directly for clicking,
    // but only call get_nuke_doubleclicked();
    nuke_doubleclicked = false;

    if (nuke_single.get_on() || nuke_multi.get_on()) {
        // never report anything new
        nuke_doubleclicked = false;
    }
    else {
        // .get_clicked() guarantees that the button is visible upon true
        if (nuke_single.get_clicked() || nuke_multi.get_clicked()) {
            if (Help::timer_ticks - timer_tick_nuke_single
             <= hardware.doubleclick_speed) {
                nuke_doubleclicked = true;
            }
            else timer_tick_nuke_single = Help::timer_ticks;
        }
        // if not yet activated, show the button as pressed down, this is
        // nice graphical feedback. get_clicked() may have been triggered
        // by hotkey, which doesn't press the button down, so do it here.
        if (hardware.key_hold(nuke_single.get_hotkey())) {
            nuke_single.set_down();
            nuke_multi .set_down();
        }
    }
    // end of nuke

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
