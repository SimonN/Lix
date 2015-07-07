/*
 * level.cpp
 *
 */

#include "level.h"

#include "../graphic/gra_lib.h"
#include "../other/help.h"
#include "../other/language.h"
#include "../other/user.h" // length of skill vector

const int Level::min_xl (160);
const int Level::min_yl (160);
const int Level::max_xl (3200);
const int Level::max_yl (2000);
const int Level::spawnint_min( 1);
const int Level::spawnint_max(96);



Level::Level(const Filename& filename)
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

    built            = "";
    players_intended = 1;

    author        = "";
    name_german   = "";
    name_english  = "";

    hints_german  . clear();
    hints_english . clear();
    size_x        = LEMSCR_X;
    size_y        = LEMSCR_Y - gloB->panel_gameplay_yl;
    torus_x       = false;
    torus_y       = false;
    start_manual  = false;
    start_x       = 0;
    start_y       = 0;
    bg_red        = 0;
    bg_green      = 0;
    bg_blue       = 0;
    seconds       = 0;
    initial       = 30;
    required      = 20;
    spawnint_slow = 32;
    spawnint_fast =  4;

    nuke_delayed = false;
    nuke_skill   = LixEn::NOTHING;

    count_neutrals_only = false;
    transfer_skills     = false;

    for (int i = Object::TERRAIN; i != Object::MAX; ++i)
        pos[i].clear();

    skills.clear();
}



bool Level::operator == (const Level& l) const
{
    // "built" is ignored for equality
    if (this->players_intended != l.players_intended
     || this->author        != l.author
     || this->name_german   != l.name_german
     || this->name_english  != l.name_english
     || this->hints_german  != l.hints_german
     || this->hints_english != l.hints_english

     || this->size_x        != l.size_x
     || this->size_y        != l.size_y
     || this->torus_x       != l.torus_x
     || this->torus_y       != l.torus_y
     || this->start_manual  != l.start_manual
     || ( this->start_x     != l.start_x && l.start_manual)
     || ( this->start_y     != l.start_y && l.start_manual)
     || this->bg_red        != l.bg_red
     || this->bg_green      != l.bg_green
     || this->bg_blue       != l.bg_blue

     || this->seconds       != l.seconds
     || this->initial       != l.initial
     || this->required      != l.required
     || this->spawnint_slow != l.spawnint_slow
     || this->spawnint_fast != l.spawnint_fast

     || this->nuke_delayed  != l.nuke_delayed
     || this->nuke_skill    != l.nuke_skill

     || this->count_neutrals_only != l.count_neutrals_only
     || this->transfer_skills     != l.transfer_skills    ) return false;

    for (int i = Object::TERRAIN; i != Object::MAX; ++i)
     if (pos[i]            != l.pos[i]) return false;

    if (this->skills != l.skills) return false;

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



const std::vector <std::string>& Level::get_hints() const {
    if (Language::get() == Language::GERMAN)
         return hints_german .empty() ? hints_english : hints_german;
    else return hints_english.empty() ? hints_german  : hints_english;
}
