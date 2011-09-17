/*
 * gameplay/replay.h
 *
 * Struktur ReplayData
 *
 * Klasse Replay
 *
 *   Aufzeichnung aller Spielgeschehenisse, um eindeutig den Spielverlauf zu
 *   rekonstruieren. Es werden viele verschiedene ReplayData-Instanzen
 *   verwaltet, die abgerufen werden koennen. Immer, wenn etwas Notwendiges
 *   passiert, wird eine neue Instanz von ReplayData gespeichert.
 *
 *   Es kann per Funktion ein kleiner ReVec abgerufen werden, der alle
 *   Ereignisse fuer ein Update enthaelt.
 *
 * bool holds_level
 *
 *   Wenn kein Ort mit einer Leveldatei angegeben wurde, gehen wir davon aus,
 *   dass wir soeben die Leveldaten in der Replaydatei mitgeparst haben. Dann
 *   soll der, der die Replaydatei laedt, anhand dieser Variable sehen, dass
 *   zum Level-Laden auch der Replay-Dateiname verwendet werden muss.
 *
 *   Die Replay-Speicherfunktion erkennt, ob diese Variable gesetzt wird oder
 *   aber auch, ob wir den temporaeren Netzwerklevel spielen! In beiden Faellen
 *   wird der Level mitgespeichert.
 *
 * ULong what
 *
 *   Beschreibt, welche/r Lix/Skill/Rate gewaehlt wurde.
 *   Im Falle von AIM (Zielen mit der Maus) wird folgendes Format angewandt:
 *
 *     what = (y * xl * maxlix) + (x * maxlem) + lixid
 *
 *   Hier sind (x, y) die Koordinaten des Punktes, auf den gezielt wird,
 *   und xl ist die Breite des Levels. lixid ist die ID der Lix, der
 *   zielen soll, und maxlix ist die Anfangsanzahl an Lixen des Spielers.
 *   Das passt auf jeden Fall in einen 32-Bit-Unsigned.
 *
 */

#pragma once

#include <fstream>
#include <set>
#include <vector>

#include "../lix/lix_enum.h"
#include "../network/net_t.h" // work with struct ReplayData
#include "../network/permu.h"
#include "../other/date.h"
#include "../other/filename.h"
#include "../other/types.h"

class Level;

class Replay {

public:

    typedef ReplayData Data;

    enum Action {
        NOTHING,
        RATE,
        SKILL,
        ASSIGN,
        AIM,
        NUKE
    };

    struct Player {
        PlNr        number;
        LixEn::Style  style;
        std::string name;
        Player() : number(0), style(LixEn::GARDEN) {}
        Player(PlNr nr, LixEn::Style s, const std::string& n)
         : number(nr), style(s), name(n) {}
        bool operator < (const Player& rhs) const
         { return number < rhs.number; }
    };

    typedef std::vector <Data>                  Vec;
    typedef std::vector <Data> ::iterator       It;
    typedef std::vector <Data> ::const_iterator ConstIt;

    typedef std::set <Player>  ::const_iterator PlayerCIt;

private:

    bool              file_not_found; // Stammt aus nichtext. Datei?
    bool              holds_level;    // Level im Replay?

    Ulng              version_min;
    Date              built_required; // Welche Version vom Level ist OK?
    Filename          level_filename;

    std::set <Player> players;
    Permu             permu;

    Vec               data;
    Uint              max_updates;

    PlNr              player_local;

public:

    Replay();
    Replay(const Filename&);
    ~Replay();

    inline void set_level_filename(const Filename& s) { level_filename = s;
                                                        holds_level = false;}
    inline const Filename& get_level_filename()      { return level_filename;}
    inline const std::set <Player>& get_players()    { return players;       }
           void  add_player(Uint pos, LixEn::Style s, const std::string& name);
    const  std::string& get_player_local_name();
    inline Ulng         get_version_min()          { return version_min;     }
    inline const Date&  get_built_required()       { return built_required;  }

    inline const Permu& get_permu() const          { return permu;           }
    inline void         set_permu(const Permu& p)  { permu = p;              }
    inline void         shorten_permu_to(size_t n) { permu.shorten_to(n);    }

    inline PlNr get_player_local   ()              { return player_local;    }
    inline void set_player_local   (PlNr u)        { player_local = u;       }
    inline void set_max_updates    (const Uint i)  { max_updates = i;        }
    inline Uint get_max_updates    () const        { return max_updates;     }

    inline bool get_file_not_found () const        { return file_not_found;  }
    inline bool get_holds_level    () const        { return holds_level;     }

    bool        equal_before                   (const Replay&, Ulng) const;

    void        clear                          ();
    void        erase_data_after_update        (const Ulng);
    bool        get_on_update_lix_clicked      (const Ulng, const Uint);
    Vec         get_and_erase_data_until_update(const Ulng);
    Vec         get_data_for_update            (const Ulng) const;
    inline const Vec& get_data                 () { return data; }
    void        add                            (const Data&);
    void        add                            (const Vec&);

    void        save_as_auto_replay            (const Level* const = 0);
    void        save_to_file                   (const Filename&,
                                                const Level* const = 0);
    void        load_from_file                 (const Filename&);


};
