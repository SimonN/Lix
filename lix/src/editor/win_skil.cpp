/*
 * ./editor/win_skil.cpp
 *
 */

#include "win_skil.h"

#include "../graphic/gra_lib.h"
#include "../other/language.h"

namespace Api {

const int WindowSkill::y_sbwb(140);
const int WindowSkill::this_xl(520);
const int WindowSkill::this_yl(y_sbwb + 210);

WindowSkill::WindowSkill(Level& l)
:
    Window(LEMSCR_X/2 - this_xl/2, LEMSCR_Y/2 - this_yl/2 - 30,
     this_xl, this_yl, Language::win_skill_title),

    level(&l),
    sbwb (gloB->skill_max),

    cur_pos (0),
    cur_sbwb(0),

    clear    ( 20, this_yl - 70, 180),
    classic8 ( 20, this_yl - 40,  80),
    classic12(100, this_yl - 40, 100),
    all_to  (220, this_yl - 70, 160),
    all_to_n(220, this_yl - 40, 160, 3, -1, LixEn::skill_nr_max, 0, true),
    ok      (this_xl - 120, this_yl - 70, 100),
    cancel  (this_xl - 120, this_yl - 40, 100)
{
    // Moegliche Faehigkeiten
    unsigned int i = 0;
    typedef PossibleSkill PosSk;

    possible.push_back(PosSk(px(i), py(i), LixEn::WALKER));     ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::JUMPER));     ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::EXPLODER));   ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::NOTHING));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::PLATFORMER)); ++i;
    // possible.push_back(PosSk(px(i), py(i), LixEn::ROPER));      ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::NOTHING));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::NOTHING));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::NOTHING));    ++i;

    possible.push_back(PosSk(px(i), py(i), LixEn::CLIMBER));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::FLOATER));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::EXPLODER2));  ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::BLOCKER));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::BUILDER));    ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::BASHER));     ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::MINER));      ++i;
    possible.push_back(PosSk(px(i), py(i), LixEn::DIGGER));     ++i;

    // Tatsaechliche Faehigkeiten
    for (int i = 0; i < gloB->skill_max; ++i) {
        sbwb[i] = new SkillButtonWithButtons;
        sbwb[i]->set_x(20 + i*40);
        sbwb[i]->set_y(y_sbwb);
        sbwb[i]->skill.set_skill (level->skill[i].ac);
        sbwb[i]->skill.set_number(level->skill[i].nr);
        add_child(*sbwb[i]);
    }

    // Add children
    for (std::vector <PossibleSkill> ::iterator i = possible.begin();
     i != possible.end(); ++i) add_child(*i);
    add_child(clear);
    add_child(classic8);
    add_child(classic12);
    add_child(all_to);
    add_child(all_to_n);
    add_child(ok);
    add_child(cancel);

    // Eventuell bei den moeglichen was ausgrauen?
    for (PosIt i = possible.begin(); i != possible.end(); ++i)
     for (SbwbIt j = sbwb.begin();   j != sbwb    .end(); ++j)
      if (i->ac == (**j).skill.get_skill()) i->set_color(false);

    all_to_n.set_minus_one_char('*');

    clear    .set_text(Language::win_skill_clear);
    classic8 .set_text(Language::win_skill_classic_8);
    classic12.set_text(Language::win_skill_classic_12);
    all_to   .set_text(Language::win_skill_all_to);
    ok       .set_text(Language::ok);
    cancel   .set_text(Language::cancel);

    ok       .set_hotkey(KEY_ENTER);
    cancel   .set_hotkey(KEY_ESC);
}

WindowSkill::~WindowSkill()
{
    for (int i = 0; i < gloB->skill_max; ++i) delete sbwb[i];
}



void WindowSkill::swap_sbwbs(SkillButtonWithButtons* a,
                             SkillButtonWithButtons* b)
{
    if (a == b) return;
    LixEn::Ac tempa = a->skill.get_skill ();
    unsigned    tempi = a->skill.get_number();
    a->skill.set_skill (b->skill.get_skill ());
    a->skill.set_number(b->skill.get_number());
    b->skill.set_skill (tempa);
    b->skill.set_number(tempi);
}

void WindowSkill::assign_sbwb_pos(SkillButtonWithButtons* b,
                                  PossibleSkill* p)
{
    LixEn::Ac bac = b->skill.get_skill();
    if (bac == p->ac || p->ac == LixEn::NOTHING) {
        for (PosIt i = possible.begin(); i != possible.end(); ++i)
         if (i->ac == bac) {
            i->set_color(true);
            break;
        }
        b->skill.set_skill(LixEn::NOTHING);
    }
    else {
        bool swapped = false;
        for (SbwbIt i = sbwb.begin(); i != sbwb.end(); ++i)
         if (*i != b && (**i).skill.get_skill() == p->ac) {
            swapped = true;
            swap_sbwbs(b, *i);
            break;
        }
        if (!swapped) {
            // Bisherigen Inhalt des Sbwbs oben wieder farbig machen
            for (PosIt i = possible.begin(); i != possible.end(); ++i)
             if (i->ac == bac) {
                i->set_color(true);
                break;
            }
            b->skill.set_skill(p->ac);
            p->set_color(false);
        }
    }
}



void WindowSkill::calc_self()
{
    for (PosIt  i = possible.begin(); i != possible.end(); ++i) {
        if (i->get_clicked()) {
            if (cur_sbwb != 0) {
                assign_sbwb_pos(cur_sbwb, &*i);
                cur_sbwb->skill.set_off();
                cur_sbwb = 0;
            }
            else if (cur_pos != 0) {
                cur_pos->set_off();
                cur_pos = (&*i != cur_pos ? &*i : 0);
                if (cur_pos) cur_pos->set_on();
            }
            else {
                cur_pos = &*i;
                cur_pos->set_on();
            }
        }
    }
    for (SbwbIt i = sbwb.begin(); i != sbwb.end(); ++i) {
        if ((**i).skill.get_clicked()) {
            if (cur_pos != 0) {
                assign_sbwb_pos(*i, cur_pos);
                cur_pos->set_off();
                cur_pos = 0;
            }
            else if (cur_sbwb != 0
             && cur_sbwb->skill.get_skill() == LixEn::NOTHING
             && (**i)    .skill.get_skill() == LixEn::NOTHING) {
                cur_sbwb->skill.set_off();
                if (cur_sbwb != *i) {
                    cur_sbwb = *i;
                    cur_sbwb->skill.set_on();
                }
                else cur_sbwb = 0;
            }
            else if (cur_sbwb != 0) {
                swap_sbwbs(*i, cur_sbwb);
                cur_sbwb->skill.set_off();
                cur_sbwb = 0;
            }
            else {
                cur_sbwb = *i;
                cur_sbwb->skill.set_on();
            }
        }
    }
    // Ende vom Faehigkeitenzuweisen

    if (ok.get_clicked() || hardware.get_mr()) {
        // Daten in das Levelobjekt des Editors schreiben
        for (int i = 0; i < gloB->skill_max; ++i) {
            level->skill[i].ac = sbwb[i]->skill.get_skill();
            level->skill[i].nr = sbwb[i]->skill.get_number();
        }
        set_exit();
    }
    else if (cancel.get_clicked()) {
        set_exit();
    }
    else if (clear.get_clicked()) {
        for (SbwbIt i = sbwb.begin(); i != sbwb.end(); ++i)
         (**i).skill.set_skill(LixEn::NOTHING);
        for (PosIt i = possible.begin(); i != possible.end(); ++i)
         i->set_color(true);
        if (cur_sbwb) {
            cur_sbwb->skill.set_off();
            cur_sbwb = 0;
        }
        if (cur_pos) {
            cur_pos->set_off();
            cur_pos = 0;
        }
    }
    else if (classic8.get_clicked()) {
        sbwb[0]->skill.set_skill(LixEn::CLIMBER);
        sbwb[1]->skill.set_skill(LixEn::FLOATER);
        sbwb[2]->skill.set_skill(LixEn::EXPLODER);
        sbwb[3]->skill.set_skill(LixEn::BLOCKER);
        sbwb[4]->skill.set_skill(LixEn::BUILDER);
        sbwb[5]->skill.set_skill(LixEn::BASHER);
        sbwb[6]->skill.set_skill(LixEn::MINER);
        sbwb[7]->skill.set_skill(LixEn::DIGGER);
        sbwb[8]->skill.set_skill(LixEn::NOTHING);
        sbwb[9]->skill.set_skill(LixEn::NOTHING);
        sbwb[10]->skill.set_skill(LixEn::NOTHING);
        sbwb[11]->skill.set_skill(LixEn::NOTHING);
        for (PosIt i = possible.begin(); i != possible.end(); ++i) {
            if (i->ac == LixEn::CLIMBER
             || i->ac == LixEn::FLOATER
             || i->ac == LixEn::EXPLODER
             || i->ac == LixEn::BLOCKER
             || i->ac == LixEn::BUILDER
             || i->ac == LixEn::BASHER
             || i->ac == LixEn::MINER
             || i->ac == LixEn::DIGGER) i->set_color(false);
            else                        i->set_color(true);
        }
    }
    else if (classic12.get_clicked()) {
        sbwb[0]->skill.set_skill(LixEn::WALKER);
        sbwb[1]->skill.set_skill(LixEn::JUMPER);
        sbwb[2]->skill.set_skill(LixEn::CLIMBER);
        sbwb[3]->skill.set_skill(LixEn::FLOATER);
        sbwb[4]->skill.set_skill(LixEn::EXPLODER2);
        sbwb[5]->skill.set_skill(LixEn::BLOCKER);
        sbwb[6]->skill.set_skill(LixEn::NOTHING);
        sbwb[7]->skill.set_skill(LixEn::BUILDER);
        sbwb[8]->skill.set_skill(LixEn::PLATFORMER);
        sbwb[9]->skill.set_skill(LixEn::BASHER);
        sbwb[10]->skill.set_skill(LixEn::MINER);
        sbwb[11]->skill.set_skill(LixEn::DIGGER);
        for (PosIt i = possible.begin(); i != possible.end(); ++i) {
            if (i->ac == LixEn::WALKER
             || i->ac == LixEn::JUMPER
             || i->ac == LixEn::CLIMBER
             || i->ac == LixEn::FLOATER
             || i->ac == LixEn::EXPLODER2
             || i->ac == LixEn::BLOCKER
             || i->ac == LixEn::BUILDER
             || i->ac == LixEn::PLATFORMER
             || i->ac == LixEn::BASHER
             || i->ac == LixEn::MINER
             || i->ac == LixEn::DIGGER) i->set_color(false);
            else                        i->set_color(true);
        }
    }
    else if (all_to.get_clicked()) {
        for (int i = 0; i < gloB->skill_max; ++i)
         sbwb[i]->skill.set_number(all_to_n.get_number());
    }
}



// ############################################################################
// ############################################################################
// ############################################################################



const unsigned PossibleSkill::this_length(40);

PossibleSkill::PossibleSkill(
 const int nx, const int ny, const LixEn::Ac nac, const LixEn::Style sk)
:
    Button(nx, ny, this_length, this_length),
    lem   (GraLib::get_lix(sk), get_ground()),
    ac    (nac)
{
    if (ac != LixEn::NOTHING) lem.set_y_frame(ac - 1);
    else                    lem.set_y_frame(0);
}

PossibleSkill::~PossibleSkill()
{
}



bool PossibleSkill::get_color()
{
    set_draw_required();
    return !lem.get_x_frame();
}

void PossibleSkill::set_color(const bool b)
{
    lem.set_x_frame(b ? 0 : 1);
    set_draw_required();
}



void PossibleSkill::draw_self()
{
    Button::draw_self();
    lem.set_x(get_x_here() + 2);
    lem.set_y(get_y_here() + 3);
    lem.draw();
}



// ############################################################################
// ############################################################################
// ############################################################################



SkillButtonWithButtons::SkillButtonWithButtons(const unsigned nx,
 const unsigned ny, const LixEn::Ac ac, const int nr)
:
    Element (nx, ny, 40, 60+60),
    skill   (0, 60, ac, nr, LixEn::GARDEN),
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
