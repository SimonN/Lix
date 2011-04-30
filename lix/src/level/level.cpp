/*
 * level.cpp
 *
 */

#include "level.h"

#include "../graphic/gra_lib.h"
#include "../other/help.h"
#include "../other/language.h"

const int Level::min_xl (160);
const int Level::min_yl (160);
const int Level::max_xl (3200);
const int Level::max_yl (2000);

Level::Level(const std::string& filename)
:
    pos(Object::MAX)
{
    load_from_file(filename);
}

Level::~Level()
{
    clear();
}

void Level::clear()
{
    status = BAD_EMPTY;

    built        = "";
    author       = "";
    name_german  = "";
    name_english = "";
    size_x       = LEMSCR_X;
    size_y       = LEMSCR_Y - gloB->panel_gameplay_yl;
    start_x      =  0;
    start_y      =  0;
    torus_x      = false;
    torus_y      = false;
    seconds      =  0;
    initial      = 30;
    required     = 20;
    rate         = 50;

    count_neutrals_only = false;
    transfer_skills     = false;

    for (int i = Object::TERRAIN; i != Object::MAX; ++i) pos[i].clear();

    skill = std::vector <Skill> (gloB->skill_max);

    for (int i = 0; i < gloB->skill_max; ++i) {
        skill[i].ac = LixEn::NOTHING;
        skill[i].nr = 0;
    }
}



bool Level::operator == (const Level& l) const
{
    if (this->author       != l.author
     || this->name_german  != l.name_german
     || this->name_english != l.name_english

     || this->size_x       != l.size_x
     || this->size_y       != l.size_y
     || this->start_x      != l.start_x
     || this->start_y      != l.start_y
     || this->torus_x      != l.torus_x
     || this->torus_y      != l.torus_y

     || this->seconds      != l.seconds
     || this->initial      != l.initial
     || this->required     != l.required
     || this->rate         != l.rate

     || this->count_neutrals_only != l.count_neutrals_only
     || this->transfer_skills     != l.transfer_skills    ) return false;

    for (int i = Object::TERRAIN; i != Object::MAX; ++i)
     if (pos[i]            != l.pos[i]) return false;

    for (int i = 0; i < (int) gloB->skill_max; ++i)
     if (this->skill[i].ac != l.skill[i].ac
     ||  this->skill[i].nr != l.skill[i].nr) return false;

    return true;
}



bool Level::operator != (const Level& l) const
{
    return !(*this == l);
}



Level::Pos::Pos()
:
    ob  (0),
    x   (0),
    y   (0),
    mirr(false),
    rot (0),
    dark(false),
    noow(false)
{
}



bool Level::Pos::operator == (const Level::Pos& t) const
{
    return this->ob   == t.ob
        && this->x    == t.x
        && this->y    == t.y
        && this->mirr == t.mirr
        && this->rot  == t.rot
        && this->dark == t.dark
        && this->noow == t.noow;
}



const std::string& Level::get_name() const {
    if (Language::get() == Language::GERMAN)
         return name_german .empty() ? name_english : name_german;
    else return name_english.empty() ? name_german  : name_english;
}
