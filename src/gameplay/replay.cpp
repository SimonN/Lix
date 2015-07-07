/*
 * gameplay/replay.cpp
 *
 */

#include "../other/myalleg.h"
#include <sstream>

#include "replay.h"

#include "state.h" // for legacy replay fixing
#include "../lix/lix.h" // for legacy replay fixing

#include "../level/level.h"
#include "../level/level_me.h"
#include "../other/file/io.h"
#include "../other/language.h"

Replay::Replay()
:
    file_not_found(false),
    version_min   (gloB->version_min),
    built_required(gloB->empty_string),
    level_filename(gloB->empty_filename),
    max_updates   (0),
    first_skill_bc(LixEn::NOTHING),
    player_local  (-1)
{
}

Replay::Replay(const Filename& fn)
:
    level_filename(fn)
{
    load_from_file(fn);
}

Replay::~Replay()
{
}



void Replay::clear()
{
    file_not_found = false;
    version_min    = gloB->version_min;
    built_required = gloB->empty_string;
    level_filename = gloB->empty_filename;
    player_local   = -1;
    max_updates    = 0;
    first_skill_bc = LixEn::NOTHING;
    players        .clear();
    permu          = Permu();
    data           .clear();
}



void Replay::add_player(Uint nr, LixEn::Style s, const std::string& name)
{
    players.insert(Player(nr, s, name));
}



bool Replay::equal_before(const Replay& rhs, const Ulng before_update) const
{
    // We don't check whether the metadata/general data is the same.
    // We assume the gameplay class only uses for replays of the same level
    // with the same players.
    ConstIt lit = data.begin();
    ConstIt rit = rhs.data.begin();
    while (true) {
        if      (lit == data.end() || rit == rhs.data.end()) return true;
        else if (lit->update >= before_update
              && rit->update >= before_update) return true;
        else if (*lit != *rit) return false;
        // Still equal? Increment and repeat.
        ++lit;
        ++rit;
    }
}



// this function is necessary to keep old replays working in new versions
// that skip the first 30 or so updates, to get into the action faster.
// The first spawn is still at update 60.
void Replay::increase_early_data_to_update(const Ulng i)
{
    for (It itr = data.begin(); itr != data.end(); ++itr) {
        if (itr->update < i) itr->update = i;
        else break;
    }
    if (! data.empty()) max_updates = data.rbegin()->update;
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



void Replay::erase_early_singleplayer_nukes()
{
    if (players.size() != 1) return;

    file_not_found = false;
    version_min    = gloB->version_min;

    // Erase nukes in singleplayer before the first spawned lix.
    // The number of the spawn update is 60, it's hardcoded here and in
    // gamepl_u.cpp. The nuke is performed before the lix spawns in the same
    // update, so <= 60 is correct instead of < 60.
    It it = data.begin();
    while (it != data.end() && it->update <= 60) {
        if (it->action == NUKE) {
            data.erase(it);
            it = data.begin();
        }
        else ++it;
    }
    max_updates = data.empty() ? 0 : (--data.end())->update;
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



bool Replay::get_on_update_lix_clicked(const unsigned long u,
                                       const unsigned      lem_id,
                                       const LixEn::Ac     ac
) {
    Replay::Vec vec = get_data_for_update(u);
    for (ConstIt it = vec.begin(); it != vec.end(); ++it)
     if (  (it->action == ASSIGN
        ||  it->action == ASSIGN_LEFT
        ||  it->action == ASSIGN_RIGHT)
        && it->what  == lem_id
        && it->skill == ac) return true;
    return false;
}



inline static int compare_data(const Replay::Data& a, const Replay::Data& b)
{
    if (a.update < b.update) return -1;
    if (a.update > b.update) return 1;
    if (a.player < b.player) return -1;
    if (a.player > b.player) return 1;
    // do not order by action:
    // assign, force, nuke -- all of these are equal, and the sort must be
    // stable later. Keep these in the replay in whatever order the player
    // has input them.
    return 0;
}



void Replay::add(const Replay::Data& d)
{
    // Veraendert ist es natuerlich nicht mehr geladen, aber die Tatsache, ob
    // der Level in der Datei steckt, darf nicht verloren gehen.
    file_not_found = false;
    version_min    = gloB->version_min;

    // Daten verarbeiten
    if (data.size() == 0 || d.update > max_updates) {
        data.push_back(d);
        max_updates = d.update;
    }
    else {
        It itr = data.begin();
        while (itr != data.end()) {
            // compare_data is a static funcition defined right above this
            if (compare_data(d, *itr) == -1) {
                data.insert(itr, d);
                break;
            }
            ++itr;
        }
        if (itr == data.end()) data.push_back(d);
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



std::string Replay::get_canonical_save_filename()
{
    std::string ret = level_filename.get_file_no_ext_no_pre_ext();

    for (std::set <Player> ::const_iterator itr = players.begin();
     itr != players.end(); ++itr) if (itr->number == player_local) {
        ret += "-";
        ret += itr->name;
        break;
    }
    return ret;
}



void Replay::save_as_auto_replay(const Level* const lev)
{
    bool multi = (players.size() > 1);

    if ((  multi && ! gloB->replay_auto_multi)
     || (! multi && ! gloB->replay_auto_single)) return;

    std::ostringstream outfile;
    outfile << (multi ? gloB->file_replay_auto_multi.get_rootful()
                      : gloB->file_replay_auto_single.get_rootful());

    int       nr    = 0;
    const int nrmax = gloB->replay_auto_max;

    if (multi) nr = gloB->replay_auto_next_m;
    else       nr = gloB->replay_auto_next_s;
    if (nr >= nrmax) nr = 0;
    if (nr < 100) outfile << "0";
    if (nr <  10) outfile << "0";
    outfile << nr << gloB->ext_replay;

    // Done putting the number into 'outfile', now increasing global variable
    if (multi) gloB->replay_auto_next_m = (nr + 1 > nrmax ? 0 : nr + 1);
    else       gloB->replay_auto_next_s = (nr + 1 > nrmax ? 0 : nr + 1);

    Filename outfile_filename(outfile.str());
    save_to_file(outfile_filename, lev);
}



void Replay::save_to_file(const Filename& s, const Level* const lev)
{
    bool save_level_into_file = level_filename == gloB->empty_filename
                             || lev != 0;

    // We currently override the above check, and will always save a level
    // into the replay, thus have the replay never point back into the level
    // tree.
    save_level_into_file = true;

    std::ofstream file(s.get_rootful().c_str());

    // Also override NOT saving the filename. Always save the filename right
    // now, and use the level in the replay as a fallback if there is nothing
    // at the pointed-to level position.
    if (true || !save_level_into_file) {
        LevelMetaData lmd(level_filename);
        if (lmd.get_file_exists()) {
            built_required = lmd.built;
        }
        // Write the path to the level, but omit the leading (dir-levels)/
        // This "if" is just against a crash in networking games.
        if (level_filename.get_rootless().size()
         >  gloB->dir_levels.get_dir_rootless().size())
        {
             file << IO::LineDollar(gloB->replay_level_filename,
                               level_filename.get_rootless().substr(
                               gloB->dir_levels.get_dir_rootless().size()))
             << IO::LineDollar(gloB->replay_built_required, built_required);
        }
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
        std::string word
         = itr->action == Replay::SPAWNINT     ? gloB->replay_spawnint
         : itr->action == Replay::NUKE         ? gloB->replay_nuke
         : itr->action == Replay::ASSIGN       ? gloB->replay_assign_any
         : itr->action == Replay::ASSIGN_LEFT  ? gloB->replay_assign_left
         : itr->action == Replay::ASSIGN_RIGHT ? gloB->replay_assign_right
                                               : Language::common_cancel;
        if (itr->action == ASSIGN || itr->action == ASSIGN_LEFT
                                  || itr->action == ASSIGN_RIGHT) {
            word += "=";
            word += LixEn::ac_to_string(static_cast <LixEn::Ac> (itr->skill));
        }
        file << IO::LineBang(itr->update, itr->player, word, itr->what);
    }

    if (save_level_into_file) {
        file << std::endl;
        file << (lev ? *lev : Level(level_filename));
    }

    file.close();
}



void Replay::load_from_file(const Filename& fn)
{
    clear();

    level_filename   = fn; // Standardwert: Annahme, Level in Replaydatei
    unsigned long vm = 0;  // version_min erst spaeter setzen wegen add()

    std::vector <IO::Line> lines;
    if (!IO::fill_vector_from_file(lines, fn.get_rootful())) {
        file_not_found = true;
        return;
    }

    for (IO::LineIt i = lines.begin(); i != lines.end(); ++i) switch(i->type) {
    case '$':
        if      (i->text1 == gloB->replay_built_required) built_required = i->text2;
        else if (i->text1 == gloB->replay_permu         ) permu          = Permu   (i->text2);
        else if (i->text1 == gloB->replay_level_filename) {
            // We changed the names of the directories on 2012-04-12. Probably
            // a year from this time on, there shouldn't be any important
            // replays with the old path anymore. Then, remove this erase()
            // to finally allow a directory (levels-dir)/levels/ in theory.
            std::string filestr = i->text2;
            if (filestr.substr(0, 7) == "levels/") filestr.erase(0, 7);
            level_filename = Filename(
                gloB->dir_levels.get_dir_rootless() + filestr);
        }
        break;

    case '#':
        if      (i->text1 == gloB->replay_version_min   ) vm = i->nr1;
        break;

    case '+':
        if (i->text1 == gloB->replay_player
         || i->text1 == gloB->replay_friend) {
            add_player(i->nr1, LixEn::string_to_style(i->text2), i->text3);
            if (i->text1 == gloB->replay_player) player_local = i->nr1;
        }
        break;

    case '!': {
        // replays contain ASSIGN=BASHER or ASSIGN_RIGHT=BUILDER.
        // cut these strings into a left and right part, none contains '='.
        std::string part1 = i->text1;
        std::string part2 = "";
        size_t pos   = 0;
        while (pos < part1.size() && part1[pos] != '=')
            ++pos;
        if (pos < part1.size()) {
            part1 = i->text1.substr(0, pos);
            part2 = i->text1.substr(pos + 1, std::string::npos);
        }

        Data d;
        d.update = i->nr1; // d.player ist zwar ein char, aber wir lesen ja
        d.player = i->nr2; // nicht aktiv longs ein, sondern weisen nur zu.
        d.what   = i->nr3;
        d.action = part1 == gloB->replay_spawnint     ? SPAWNINT
                 : part1 == gloB->replay_skill        ? SKILL_LEGACY_SUPPORT
                 : part1 == gloB->replay_assign_any   ? ASSIGN
                 : part1 == gloB->replay_assign_left  ? ASSIGN_LEFT
                 : part1 == gloB->replay_assign_right ? ASSIGN_RIGHT
                 : part1 == gloB->replay_nuke         ? NUKE : NOTHING;
        if (part2.size() > 0) {
            d.skill = LixEn::string_to_ac(part2);
            if (d.skill == LixEn::AC_MAX)
                d.skill =  LixEn::NOTHING; // some default value, but important
                                           // for legacy fixing, see func below
        }
        add(d);
        break; }

    default:
        break;
    }

    // Variablen nach dem Laden zuweisen, damit add() nichts kaputtmacht
    version_min = vm;
}




void Replay::fix_legacy_replays_according_to_current_state(
    const GameState& cs,
    const std::vector <LixEn::Ac>& legacy_ac_vec
) {
    if (cs.tribes.empty())
        return;

    LixEn::Ac cur_skill = LixEn::NOTHING;

    for (size_t i = 0; i < legacy_ac_vec.size(); ++i) {
        LixEn::Ac ac = legacy_ac_vec[i];
        if (ac == LixEn::NOTHING)
            continue;
        Level::CSkIt itr = cs.tribes[0].skills.find(ac);
        if (itr == cs.tribes[0].skills.end() || itr->second == 0)
            // got 0 of that skill
            continue;
        // this was the first skill back in 2015-05
        cur_skill = ac;
        break;
    }

    for (size_t i = 0; i < data.size(); ++i) {

        bool move_to_future = false;

        if (data[i].action == SKILL_LEGACY_SUPPORT) {
            if (data[i].what < legacy_ac_vec.size())
                // in the old format, (what) held the skill ID, not (skill)
                cur_skill = legacy_ac_vec[data[i].what];
            data.erase(data.begin() + i);
            --i;
        }
        else if (data[i].action == ASSIGN || data[i].action == ASSIGN_LEFT
                                          || data[i].action == ASSIGN_RIGHT
        ) {
            if (data[i].skill != LixEn::NOTHING) {
                // This is a replay recorded with 2015-06-17 or newer.
                // Don't mess with any assignments. Each assignment has its
                // skill information in the same packet.
                continue;
            }
            data[i].skill = cur_skill;

            if ((cur_skill == LixEn::EXPLODER || cur_skill == LixEn::EXPLODER2)
                && cs.tribes.size() == 1
            ) {
                // Singleplayer was played with timed exploders until
                // 2015-06-17. Every old singleplayer replay must have the
                // exploder assignment delayed to match the current version.
                move_to_future = true;
            }
        }
        else if (data[i].action == NUKE && cs.tribes.size() == 1)
            move_to_future = true; // like singleplayer exploders above

        if (move_to_future) {
            Data d = data[i];
            d.update += Lixxie::updates_for_bomb - 1; // see lix_upd.cpp
            data.erase(data.begin() + i);             // for why 74, not 75
            add(d);
            --i;
        }
    }
    if (! data.empty()) max_updates = data.rbegin()->update;
}
