#include "panel.h"

#include "../../level/level.h" // number of skills per level
#include "../../graphic/gra_lib.h"

#include "../../other/language.h"
#include "../../other/user.h"
#include "../../other/help.h" // timer ticks for the nuke button

static const int xl_tec = 34;
static const int x_tec0 = 14*36; // skill buttons wider than technical buttons
static const int x_tec1 = x_tec0 + 1*xl_tec;
static const int x_tec2 = x_tec0 + 2*xl_tec;
static const int x_tec3 = x_tec0 + 3*xl_tec;

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
    skill      (useR->skill_sort.size(), Api::SkillButton()),

    spawnint_slow (x_tec0, 0),
    spawnint_cur  (x_tec1, 0),
    spawnint_fixed(BMP_PAN_F, x_tec0,  0, 2 * xl_tec, 20),

    pause      (BMP_PAUSE, x_tec3, 20, xl_tec, 60),
    zoom       (BMP_PANEL, x_tec0, 20, xl_tec, 30),
    speed_back (BMP_PANEL, x_tec0, 50, xl_tec, 30),
    speed_ahead(BMP_PANEL, x_tec1, 50, xl_tec, 30),
    speed_fast (BMP_PANEL, x_tec2, 50, xl_tec, 30),
    state_save (BMP_PAN_2, x_tec2,  0, xl_tec, 20),
    state_load (BMP_PAN_2, x_tec3,  0, xl_tec, 20),
    restart    (BMP_PANEL, x_tec1, 20, xl_tec, 30),
    nuke_single(BMP_PANEL, x_tec2, 20, xl_tec, 30),
    nuke_multi (BMP_NUKE , x_tec0, 60, 4 * xl_tec, 20),
    spec_tribe (x_tec0, 60, 4 * xl_tec), // will be reset below
    stats      (),
    rate_fixed (spawnint_fixed.get_x() + spawnint_fixed.get_xl() - 18, 0),

    on_hint_change(0),
    on_hint_change_where(0),

    gapamode   (GM_NONE),

    nuke_doubleclicked(false),
    timer_tick_nuke_single(0),

    tooltip_scrolling(false),
    tooltip_force_dir(false),
    tooltip_priority (false),

    hint_size  (0),
    hint_cur   (0),

    hint_big   (BMP_PANEL, x_tec0,            20, xl_tec,   30),
    hint_plus  (BMP_HINTS, x_tec0 + xl_tec/2, 20, xl_tec/2, 30),
    hint_minus (BMP_HINTS, x_tec0,            20, xl_tec/2, 30)
{
    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr) add_child(*itr);
    add_child(spawnint_slow);
    add_child(spawnint_cur);
    add_child(spawnint_fixed);
    add_child(pause);
    add_child(zoom);
    add_child(speed_back);
    add_child(speed_ahead);
    add_child(speed_fast);
    add_child(state_save);
    add_child(state_load);
    add_child(restart);
    add_child(nuke_single);
    add_child(nuke_multi);
    add_child(spec_tribe);

    add_child(stats);
    add_child(rate_fixed);

    add_child(hint_big);
    add_child(hint_plus);
    add_child(hint_minus);

    for (unsigned i = 0; i < skill.size(); ++i) {
        skill[i].set_x(i * skill[i].get_xl());
        skill[i].set_y(20);
    }

    spawnint_slow.set_x_frame(0);
    spawnint_cur .set_x_frame(1);
    state_save.set_x_frame(2);
    state_load.set_x_frame(3);

    zoom       .set_x_frame(2);
    speed_back .set_x_frame(10);
    speed_ahead.set_x_frame(3);
    speed_fast .set_x_frame(4); // 5 if turbo is on
    restart    .set_x_frame(8);
    nuke_single.set_x_frame(9);
    nuke_multi .set_x_frame(0);

    hint_big   .set_x_frame(1);
    hint_plus  .set_x_frame(1);
    hint_minus .set_x_frame(0);

    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr) itr->set_hot();

    speed_fast .set_warm(); // don't fire continuously on LMB held
    pause      .set_warm(); // activate like a TwoTasksButton on first click

    // We will need to be careful when checking for clicked buttons,
    // as there can be more than one skill with the same hotkey in play.
    // This is done in Gameplay's calculcate code for these buttons.
    pause      .set_hotkey(useR->key_pause);
    zoom       .set_hotkey(useR->key_zoom);
    speed_back .set_hotkey(useR->key_speed_back_one);
    speed_ahead.set_hotkey(useR->key_speed_ahead_one);
    speed_fast .set_hotkey(useR->key_speed_fast);
    state_save .set_hotkey(useR->key_state_save);
    state_load .set_hotkey(useR->key_state_load);
    restart    .set_hotkey(useR->key_restart);
    nuke_single.set_hotkey(useR->key_nuke);
    nuke_multi .set_hotkey(useR->key_nuke);
    spec_tribe .set_hotkey(useR->key_spec_tribe);

    speed_back .set_hotkey_right(useR->key_speed_back_many);
    speed_ahead.set_hotkey_right(useR->key_speed_ahead_many);
    speed_fast .set_hotkey_right(useR->key_speed_turbo);

    spawnint_slow.set_hotkey(useR->key_rate_minus);
    spawnint_cur .set_hotkey(useR->key_rate_plus);
    rate_fixed.set_align(Api::Label::CENTERED);
    rate_fixed.set_undraw_color(0);
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
        if (spawnint_slow.get_spawnint() == rate_fixed.get_number()) {
            spawnint_slow .hide();
            spawnint_cur  .hide();
        }
        else {
            spawnint_fixed.hide();
            rate_fixed    .hide();
        }
    }
    else if (gapamode == GM_REPLAY_MULTI) {
        spawnint_slow .hide();
        spawnint_cur  .hide();
        spawnint_fixed.hide();
        rate_fixed    .hide();
        nuke_multi.hide(); // we use most of the singleplayer interface
        spec_tribe.set_x(x_tec0);
        spec_tribe.set_y(0);
        spec_tribe.set_xl(2*xl_tec);
    }
    else if (gapamode == GM_PLAY_MULTI
     ||      gapamode == GM_SPEC_MULTI) {
        spawnint_slow .hide();
        spawnint_cur  .hide();
        spawnint_fixed.hide();
        rate_fixed    .hide();
        pause      .hide();
        zoom       .hide();
        speed_back .hide();
        speed_ahead.hide();
        speed_fast .hide();
        state_save .hide();
        state_load .hide();
        restart    .hide();
        nuke_single.hide();
        if (gapamode == GM_PLAY_MULTI) spec_tribe.hide();
        else {
            nuke_multi.hide();
            spec_tribe.set_x(x_tec0);
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



void GameplayPanel::set_like_tribe(const Tribe* tr)
{
    if (!tr) return;

    // instead of clearing all panels, set them to the greyed-out skill,
    // even if 0 skills will be available throughout the level
    for (size_t i = 0; i < skill.size() && i < useR->skill_sort.size(); ++i) {
        skill[i].set_skill(useR->skill_sort[i]);
    }

    // Create, then sort buttons. Hotkeys will be done after sorting
    for (Level::CSkIt itr =  tr->skills.begin();
                      itr != tr->skills.end(); ++itr
    ) {
        size_t j = 0;
        while (j < skill.size() && j < useR->skill_sort.size()
               && useR->skill_sort[j] != itr->first
               && ! (useR->skill_sort[j] == LixEn::EXPLODER2
                           && itr->first == LixEn::EXPLODER))
            // Exploder and Exploder2 are both defined by Exploder2 in
            // the user's sorted skill vector
            ++j;

        if (j != skill.size()) {
            skill[j].set_style (tr->style);
            skill[j].set_skill (itr->first);
            skill[j].set_number(itr->second);
        }
    }

    // Hotkeys depend on the shown order, not on the replay-internal order.
    // So, we don't have to examine (skill button).replay_id here.
    for (size_t i = 0; i < skill.size(); ++i) {
        const int key = useR->key_skill[skill[i].get_skill()];
        skill[i].set_hotkey(key);
        if (useR->game_show_skill_keys && key != 0)
            skill[i].set_hotkey_label(Help::scancode_to_string(key));
        skill[i].set_draw_required();
    }

    stats.set_tribe_local(tr);

    spawnint_slow.set_spawnint(tr->spawnint_slow);
    spawnint_cur .set_spawnint(tr->spawnint);
    rate_fixed   .set_number  (tr->spawnint_fast);

    nuke_single.set_on  (tr->nuke);
    nuke_multi .set_on  (tr->nuke);
    spec_tribe .set_text(tr->get_name());

    set_skill_on(skill_last_set_on);

    set_draw_required();
}



void GameplayPanel::set_speed(Speed s)
{
    pause.set_off();
    speed_fast.set_off();
    speed_fast.set_x_frame(4);

    switch (s) {
    case SPEED_NORMAL:
        break;
    case SPEED_FAST:
        speed_fast.set_on();
        break;
    case SPEED_TURBO:
        speed_fast.set_on();
        speed_fast.set_x_frame(5);
        break;
    case SPEED_PAUSE:
        pause.set_on();
        break;
    }
}



GameplayPanel::Speed GameplayPanel::get_speed()
{
    if      (pause     .get_on())           return SPEED_PAUSE;
    else if (speed_fast.get_x_frame() == 5) return SPEED_TURBO;
    else if (speed_fast.get_on())           return SPEED_FAST;
    else                                    return SPEED_NORMAL;
}





GameplayPanel::SkBIt GameplayPanel::button_by_skill(const LixEn::Ac ac)
{
    for (SkBIt itr = skill.begin(); itr != skill.end(); ++itr)
        if (itr->get_skill() == ac)
            return itr;
    return skill.end();
}



void GameplayPanel::set_skill_on(const LixEn::Ac ac)
{
    if (ac == LixEn::NOTHING)
        return;
    for (size_t i = 0; i < skill.size(); ++i)
        skill[i].set_off();

    SkBIt b = button_by_skill(ac);
    if (b != skill.end() && b->get_number() != 0) {
        b->set_on();
        skill_last_set_on = ac;
    }
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
    std::string& str,
    const Api::Element& e, const std::string& s,
    const int& first_hotkey = 0, const int& second_hotkey = 0
) {
    if (e.is_mouse_here()) {
        str = s;

        if (first_hotkey != 0) {
            str += " ";
            str += second_hotkey != 0 ? ::Language::editor_hotkeys
                                      : ::Language::editor_hotkey;
            str += " [";
            str += Help::scancode_to_string(first_hotkey);
            str += "]";
            if (second_hotkey != 0) {
                str = Language::gameplay_left_right_click + " " + str;
                str += "/[";
                str += Help::scancode_to_string(second_hotkey);
                str += "]";
            }
        }
        // end if first_hotkey != 0
    }
}



void GameplayPanel::calc_self()
{
    spawnint_fixed.set_down(false); // this sets draw_required, therefore
    rate_fixed.set_draw_required(); // this label must be required to be drawn

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

    if (useR->game_show_tooltips) {
        std::string str;
        using namespace Language;
        // fhs is defined 50 lines above
        fhs(str, spawnint_slow, gameplay_rate_minus,  useR->key_rate_minus);
        fhs(str, spawnint_cur,  gameplay_rate_plus,   useR->key_rate_plus);
        fhs(str, pause,       gameplay_pause,       useR->key_pause);
        fhs(str, zoom,        gameplay_zoom,        useR->key_zoom);
        fhs(str, state_save,  gameplay_state_save,  useR->key_state_save);
        fhs(str, state_load,  gameplay_state_load,  useR->key_state_load);
        fhs(str, restart,     gameplay_restart,     useR->key_restart);
        fhs(str, nuke_single, gameplay_nuke,        useR->key_nuke);
        fhs(str, nuke_multi,  gameplay_nuke,        useR->key_nuke);

        fhs(str, hint_big,    hint_cur == 0 ? gameplay_hint_first
                            : hint_cur == 1 ? gameplay_hint_off
                            :                 gameplay_hint_prev);
        fhs(str, hint_minus,  hint_cur == 1 ? gameplay_hint_off
                                            : gameplay_hint_prev);
        fhs(str, hint_plus,                   gameplay_hint_next);

        fhs(str, speed_back,  gameplay_speed_back, useR->key_speed_back_one,
                                                   useR->key_speed_back_many);
        fhs(str, speed_ahead, gameplay_speed_ahead,useR->key_speed_ahead_one,
                                                   useR->key_speed_ahead_many);
        fhs(str, speed_fast,  gameplay_speed_fast, useR->key_speed_fast,
                                                   useR->key_speed_turbo);

        if (! str.empty())
            // do nothing, but don't consider printing a suggested tooltip
            ;
        else if (tooltip_scrolling
         && (useR->scroll_right || useR->scroll_middle)) {
            // these non-button tooltips are less important than the button
            // descriptions, therefore they're in the else
            str += ::Language::gameplay_scroll_right_1;
            str += useR->scroll_right ? gameplay_rmb : gameplay_mmb;
            str += ::Language::gameplay_scroll_right_2;
        }
        else if (tooltip_force_dir) {
            str += ::Language::gameplay_force_dir_1;
            str += Help::scancode_to_string(useR->key_force_left);
            str += ::Language::gameplay_force_dir_2;
            str += Help::scancode_to_string(useR->key_force_right);
            str += ::Language::gameplay_force_dir_3;
        }
        else if (tooltip_priority) {
            str += ::Language::gameplay_priority_1;
            if      (useR->prioinv_right ) str += gameplay_rmb;
            else if (useR->prioinv_middle) str += gameplay_mmb;
            else {
                str += "[";
                str += Help::scancode_to_string(useR->key_priority);
                str += "]";
            }
            str += ::Language::gameplay_priority_2;
        }
        else if (tooltip_builders && useR->allow_builder_queuing) {
            str += ::Language::gameplay_queue_builders;
        }
        else if (tooltip_platformers && useR->allow_builder_queuing) {
            str += ::Language::gameplay_queue_platformers;
        }

        if (str.empty())
            stats.set_help();
        else {
            stats.set_help(str);
            tooltip_scrolling   = false;
            tooltip_force_dir   = false;
            tooltip_priority    = false;
            tooltip_builders    = false;
            tooltip_platformers = false;
        }
    }
}



void GameplayPanel::draw_self()
{
}
