/*
 * gameplay/replay.cpp
 *
 */

#include "../other/myalleg.h"
#include <sstream>

#include "replay.h"

#include "../level/level.h"

#include "../other/io.h"
#include "../other/language.h"
#include "../other/log.h"     // Datei nicht gefunden

Replay::Replay()
:
    file_not_found(false),
    holds_level   (false),
    version_min   (gloB->version_min),
    built_required(gloB->empty_string),
    max_updates   (0),
    player_local  (0)
{
}

Replay::Replay(const std::string& filename)
{
    load_from_file(filename);
}

Replay::~Replay()
{
}



void Replay::clear()
{
    file_not_found = false;
    holds_level    = false;
    version_min    = gloB->version_min;
    built_required = gloB->empty_string;
    level_filename .clear();
    player_local   = 0;
    max_updates    = 0;
    players        .clear();
    permu          = Permu();
    data           .clear();
}



void Replay::add_player(Uint nr, LixEn::Style s, const std::string& name)
{
    players.insert(Player(nr, s, name));
}



void Replay::erase_data_after_update(const unsigned long i)
{
    max_updates = 0;
    It itr = data.begin();
    while (itr != data.end() && itr->update <= i) {
        max_updates = itr->update;
        ++itr;
    }
    if (itr != data.end()) {
        file_not_found = false;
        version_min    = gloB->version_min;
        data.erase(itr, data.end());
    }
}



Replay::Vec Replay::get_and_erase_data_until_update(const unsigned long i)
{
    file_not_found = false;
    version_min    = gloB->version_min;
    Vec return_vector;
    It it = data.begin();
    while (it != data.end() && it->update <= i) {
        return_vector.push_back(*it);
        ++it;
    }
    data.erase(data.begin(), it);
    max_updates = data.empty() ? 0 : i;
    return return_vector;
}



Replay::Vec Replay::get_data_for_update(const unsigned long i) const
{
    Vec return_vector;
    for (ConstIt it = data.begin(); it != data.end(); ++it) {
        if      (it->update <  i) continue; // Fuer Geschwindigkeit
        else if (it->update == i) return_vector.push_back(*it);
        else break;
    }
    return return_vector;
}



bool Replay::get_on_update_lemming_clicked(const unsigned long u,
                                           const unsigned      lem_id)
{   Replay::Vec vec = get_data_for_update(u);
    for (ConstIt it = vec.begin(); it != vec.end(); ++it)
     if (it->action == ASSIGN && it->what == lem_id) return true;
    return false;
}



void Replay::add(const Replay::Data& d)
{
    // Veraendert ist es natuerlich nicht mehr geladen, aber die Tatsache, ob
    // der Level in der Datei steckt, darf nicht verloren gehen.
    file_not_found = false;
    version_min    = gloB->version_min;

    // Daten verarbeiten
    if (data.size() == 0 || d.update >= max_updates) {
        data.push_back(d);
        max_updates = d.update;
    }
    else if (data.begin()->update > d.update) {
        data.insert(data.begin(), d);
    }
    else {
        It i = data.begin();
        It j = data.begin();
        while (j != data.end()) {
            j++;
            if (i->update <= d.update && (j == data.end()
                                       || j->update > d.update)) {
                data.insert(j, d);
                break;
            }
            i++;
        }
    }
}



void Replay::add(const Replay::Vec& v)
{
    for (ConstIt i = v.begin(); i != v.end(); ++i) add(*i);
}



const std::string& Replay::get_player_local_name()
{
    for (std::set <Player> ::const_iterator itr = players.begin();
     itr != players.end(); ++itr) {
        if (itr->number == player_local) return itr->name;
    }
    return gloB->empty_string;
}



// ############################################################################
// ######################################################## Speichern und Laden
// ############################################################################



void Replay::save_to_file(const std::string& s, const Level* const lev)
{
    bool save_level_into_file = holds_level
                             || level_filename == gloB->empty_string
                             || lev != 0;

    std::ofstream file(s.c_str());

    if (!save_level_into_file) {
        built_required = Level::get_built(level_filename);
        file << IO::LineDollar(gloB->replay_level_filename, level_filename)
             << IO::LineDollar(gloB->replay_built_required, built_required);
    }
    file << IO::LineHash(gloB->replay_version_min, version_min)
         << std::endl;

    // Alle Spieler notieren.
    for (std::set <Player> ::const_iterator itr = players.begin();
     itr != players.end(); ++itr)
     file << IO::LinePlus(itr->number == player_local
             ? gloB->replay_player : gloB->replay_friend,
             itr->number, LixEn::style_to_string(itr->style), itr->name);

    if (players.size() > 1) {
        std::ostringstream pstr;
        pstr << permu;
        file << IO::LineDollar(gloB->replay_permu, pstr.str()) << std::endl;
    }

    file << std::endl;

    // Die einzelnen Aktionen schreiben
    for (It itr = data.begin(); itr != data.end(); ++itr) {
        file << IO::LineBang(itr->update, itr->player,
           itr->action == Replay::RATE   ? gloB->replay_rate
         : itr->action == Replay::SKILL  ? gloB->replay_skill
         : itr->action == Replay::ASSIGN ? gloB->replay_assign
         : itr->action == Replay::AIM    ? gloB->replay_aim
         : itr->action == Replay::NUKE   ? gloB->replay_nuke
                                         : Language::cancel,
         itr->what);
    }

    if (save_level_into_file) {
        file << std::endl;
        file << (lev ? *lev : Level(level_filename));
    }

    file.close();
}





void Replay::load_from_file(const std::string& filename)
{
    clear();

    level_filename   = filename; // Standardwert: Annahme, Level in Replaydatei
    unsigned long vm = 0;        // version_min erst spaeter setzen wegen add()

    std::vector <IO::Line> lines;
    if (!IO::fill_vector_from_file(lines, filename)) {
        file_not_found = true;
        holds_level    = false;
        return;
    }

    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) switch(i->type) {
    case '$':
        if      (i->text1 == gloB->replay_built_required) built_required = i->text2;
        else if (i->text1 == gloB->replay_level_filename) level_filename = i->text2;
        else if (i->text1 == gloB->replay_permu         ) permu = Permu  (i->text2);
        break;

    case '#':
        if      (i->text1 == gloB->replay_version_min   ) vm             = i->nr1;
        break;

    case '+':
        if (i->text1 == gloB->replay_player
         || i->text1 == gloB->replay_friend) {
            add_player(i->nr1, LixEn::string_to_style(i->text2), i->text3);
            if (i->text1 == gloB->replay_player) player_local = i->nr1;
        }
        break;

    case '!': {
        Data d;
        d.update = i->nr1; // d.player ist zwar ein char, aber wir lesen ja
        d.player = i->nr2; // nicht aktiv longs ein, sondern weisen nur zu.
        d.what   = i->nr3;
        if      (i->text1 == gloB->replay_rate         ) d.action = RATE;
        else if (i->text1 == gloB->replay_skill        ) d.action = SKILL;
        else if (i->text1 == gloB->replay_assign       ) d.action = ASSIGN;
        else if (i->text1 == gloB->replay_assign_legacy) d.action = ASSIGN;
        else if (i->text1 == gloB->replay_aim          ) d.action = AIM;
        else if (i->text1 == gloB->replay_nuke         ) d.action = NUKE;
        add(d);
        break; }

    default:
        break;
    }

    // Variablen nach dem Laden zuweisen, damit add() nichts kaputtmacht
    version_min = vm;
    if (level_filename == filename) {
        holds_level    = true;
        built_required = Level::get_built(level_filename);
    }
}
