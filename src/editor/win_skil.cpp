/*
 * ./editor/win_skil.cpp
 *
 */

#include "win_skil.h"

#include "../graphic/gra_lib.h"
#include "../other/language.h"
#include "../other/user.h"

namespace Api {

const int WindowSkill::y_sbwb(40);
const int WindowSkill::this_xl(40*14 + 40);
const int WindowSkill::this_yl(y_sbwb + 210);

static int x_a  = 20;
static int x_a2 = 150;
static int xl_a = 110;
static int x_b  = 280;
static int xl_b = 180;
static int x_c  = 600 - 120;
static int xl_c = 100;

WindowSkill::WindowSkill(Level& l)
:
    Window(LEMSCR_X/2 - this_xl/2, LEMSCR_Y/2 - this_yl/2 - 40,
     this_xl, this_yl, Language::win_skill_title),

    level(&l),
    sbwb (useR->skill_sort.size()),

    clear      (x_a,  this_yl - 40, xl_a),
    classic8_to(x_b,  this_yl - 70, xl_b),
    all_to     (x_a2, this_yl - 40, xl_a),
    all_to_n   (x_b,  this_yl - 40, xl_b, 3, -1, LixEn::skill_nr_max, 0, true),

    use_fling     (20, this_yl - 70),
    desc_use_fling(60, this_yl - 70, Language::win_skill_use_fling),

    ok      (x_c, this_yl - 70, xl_c),
    cancel  (x_c, this_yl - 40, xl_c)
{
    // choose which exploder to use
    use_fling.set_checked(true);
    for (size_t lv_sk = 0; lv_sk < level->skill.size(); ++lv_sk) {
        if (level->skill[lv_sk].ac == LixEn::EXPLODER) {
            use_fling.set_checked(false);
            break;
        }
    }

    // make skill buttons accordingly
    for (size_t i = 0; i < sbwb.size(); ++i) {
        sbwb[i] = new SkillButtonWithButtons;
        sbwb[i]->set_x(20 + i*40);
        sbwb[i]->set_y(y_sbwb);
        sbwb[i]->skill.set_hot(); // kludge, this is so it's never down
        LixEn::Ac ac = useR->skill_sort[i];
        if (ac == LixEn::EXPLODER2 && ! use_fling.get_checked())
            ac = LixEn::EXPLODER;
        sbwb[i]->skill.set_skill(ac);
        add_child(*sbwb[i]);

        for (size_t lv_sk = 0; lv_sk < level->skill.size(); ++lv_sk) {
            if (level->skill[lv_sk].ac == sbwb[i]->skill.get_skill()) {
                sbwb[i]->skill.set_number(level->skill[lv_sk].nr);
                break;
            }
        }
    }

    add_child(clear);
    add_child(classic8_to);
    add_child(all_to);
    add_child(all_to_n);
    add_child(use_fling);
    add_child(desc_use_fling);
    add_child(ok);
    add_child(cancel);

    all_to_n.set_minus_one_char('*');

    clear      .set_text(Language::win_skill_clear);
    classic8_to.set_text(Language::win_skill_classic_8_to);
    all_to     .set_text(Language::win_skill_all_to);
    ok         .set_text(Language::common_ok);
    cancel     .set_text(Language::common_cancel);

    ok     .set_hotkey(useR->key_me_okay);
    cancel .set_hotkey(useR->key_me_exit);
}



WindowSkill::~WindowSkill()
{
    for (size_t i = 0; i < sbwb.size(); ++i) delete sbwb[i];
}



void WindowSkill::calc_self()
{
    if (ok.get_clicked() || hardware.get_mr()) {
        // write data into the level struct that's loaded in the editor
        size_t level_skill_id = 0;
        for (size_t i = 0; i < sbwb.size()
         &&   level_skill_id < level->skill.size(); ++i) {
            if (sbwb[i]->skill.get_number() != 0) {
                level->skill[level_skill_id].ac = sbwb[i]->skill.get_skill();
                level->skill[level_skill_id].nr = sbwb[i]->skill.get_number();
                ++level_skill_id;
            }
        }
        set_exit();
    }
    else if (cancel.get_clicked()) {
        set_exit();
    }
    else if (clear.get_clicked()) {
        for (SbwbIt itr = sbwb.begin(); itr != sbwb.end(); ++itr) {
            (**itr).skill.set_number(0);
            if ((**itr).skill.get_skill() == LixEn::EXPLODER) {
                (**itr).skill.set_skill(LixEn::EXPLODER2);
                use_fling.set_checked(true);
            }
        }
    }
    else if (use_fling.get_clicked()) {
        for (SbwbIt itr = sbwb.begin(); itr != sbwb.end(); ++itr) {
            LixEn::Ac ac = (**itr).skill.get_skill();
            if (ac == LixEn::EXPLODER || ac == LixEn::EXPLODER2) {
                (**itr).skill.set_skill(use_fling.get_checked()
                    ? LixEn::EXPLODER2 : LixEn::EXPLODER);
                break;
            }
        }
    }
    else if (classic8_to.get_clicked()) {
        for (SbwbIt itr = sbwb.begin(); itr != sbwb.end(); ++itr) {
            switch ((**itr).skill.get_skill()) {
            case LixEn::EXPLODER2:
                (**itr).skill.set_skill(LixEn::EXPLODER);
                use_fling.set_checked(false);
                // falls through
            case LixEn::CLIMBER:
            case LixEn::FLOATER:
            case LixEn::EXPLODER:
            case LixEn::BLOCKER:
            case LixEn::BUILDER:
            case LixEn::BASHER:
            case LixEn::MINER:
            case LixEn::DIGGER:
                (**itr).skill.set_number(all_to_n.get_number());
                break;
            default:
                (**itr).skill.set_number(0);
                break;
            }
        }
    }
    else if (all_to.get_clicked()) {
        for (SbwbIt itr = sbwb.begin(); itr != sbwb.end(); ++itr)
            (**itr).skill.set_number(all_to_n.get_number());
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



SkillButtonWithButtons::SkillButtonWithButtons(
    const unsigned nx,
    const unsigned ny
) :
    Element (nx, ny, 40, 60+60),
    skill   (0, 60, 40),
    up_big  (GraLib::get(gloB->file_bitmap_api_number),  0,  0),
    up_med  (GraLib::get(gloB->file_bitmap_api_number),  0, 20),
    up_sml  (GraLib::get(gloB->file_bitmap_api_number),  0, 40),
    down_big(GraLib::get(gloB->file_bitmap_api_number), 20,  0),
    down_med(GraLib::get(gloB->file_bitmap_api_number), 20, 20),
    down_sml(GraLib::get(gloB->file_bitmap_api_number), 20, 40)
{
    add_child(skill);
    add_child(up_big);
    add_child(up_med);
    add_child(up_sml);
    add_child(down_big);
    add_child(down_med);
    add_child(down_sml);

    up_big  .set_x_frame(6);
    up_med  .set_x_frame(7);
    up_sml  .set_x_frame(8);
    down_big.set_x_frame(9);
    down_med.set_x_frame(10);
    down_sml.set_x_frame(11);
}

SkillButtonWithButtons::~SkillButtonWithButtons()
{
}






void SkillButtonWithButtons::calc_self()
{
    // Wenn gesetzt, haben Klicks keine Wirkung.
    bool t = skill.get_skill() != LixEn::NOTHING;
    const int& infty = LixEn::infinity;
    const int& max   = LixEn::skill_nr_max;

    int       change = 0;

    if (up_big.get_clicked() && t) change = 100;
    if (up_med.get_clicked() && t) change =  10;
    if (up_sml.get_clicked() && t) change =   1;

    if (down_big.get_clicked() && t) change = -100;
    if (down_med.get_clicked() && t) change =  -10;
    if (down_sml.get_clicked() && t) change =   -1;

    if (change != 0) {
        if (skill.get_number() == infty) {
            if (change > 0) skill.set_number(0);
            else            skill.set_number(max);
        }
        else if (skill.get_number() == 0 && change < 0) {
            skill.set_number(infty);
        }
        else if (skill.get_number() == max && change > 0) {
            skill.set_number(infty);
        }
        else if (skill.get_number() + change > max) {
            skill.set_number(max);
        }
        else if (skill.get_number() + change < 0) {
            skill.set_number(0);
        }
        else skill.set_number(skill.get_number() + change);
    }
}

} // Ende Namensraum
