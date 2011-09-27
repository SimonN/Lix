/*
 * lix.h
 *
 * class Lixxie -- the name of this class is chosen to be different from Lix,
 * because Lix appears as a substring of many type names. A Lixxie is a
 * single lix walking around.
 *
 * static void initialize_this_gets_called_from_glob_gfx_cpp()
 *
 *   Wird von glob.gfx.cpp() aufgerufen. Fuellt die Vektoren und Maps
 *   mit Strings und Farben, und bemalt die kopierten Lix-Bitmaps.
 *   Es gibt keine Funktion deinitialize() oder aehnliches.
 *
 * const int id
 *
 *   Kennziffer, die fuer Replays genutzt wird. Wird der Lix per
 *   Konstruktor zugewiesen.
 *
 * int ex
 * int ey
 *
 *   Eine Lix wird nicht nach den X-/Y-Koordinaten sines Grafikobjekts be-
 *   rechnet, sondern nach den sogenannten effektiven X-/Y-Koordinaten ex, ey.
 *
 *   Diese Koordinaten kennzeichnen die obere linke Ecke des 2-x-2-Quadrates,
 *   welches sich ueblicherweise am Fuss der Lix befindet, unterhalb des
 *   vorderen Koerperrandes.
 *
 *   Schaut die Lix allerdings nach links, so wird fuer alle Berechnungen
 *   vom Punkt (ex-1|ey) ausgegangen, welcher gleich weit vom linken Koerper-
 *   rand entfernt ist wie (ex|ey) bei einem nach rechts Schauenden.
 *
 * int dir
 *
 *   Diese Variable ist 1, wenn die Lix nach rechts schaut, oder -1, falls
 *   sie nach links schaut. Is initialized with 1.
 *
 * bool marked
 * bool get_mark()
 * void mark()
 * void unmark()
 *
 *   Eine Markierung dient lediglich dazu, die Lix später zu bearbeiten:
 *   Die Hauptschleife bearbeitet zunächchst nur Arbeitstiere und markiert
 *   sie, dann durchläuft sie nochmals alle Lixen. Hier werden Markierte
 *   entmarkiert, alle anderen aber bearbeitet.
 *
 *   Markierungen sind nicht zu verwechseln mit Highlighting (Hervorhebung)!
 *   Highlighting dient dazu, die Lix für den Benutzer leichter anklickbar
 *   zu machen. Ich habe es jedoch nicht eingebaut.
 *
 * unsigned get_priority(const Ac = NOTHING)
 *
 *   Argument:  Die Faehigkeit, die beim Klick gegeben wuerde.
 *   Rueckgabe: 0, falls nicht anklickbar und Cursor geschlossen.
 *              1, falls nicht anklickbar und Cursor dennoch offen.
 *              2 bis 99.998, falls anklickbar. Hoehere Zahl = hoehere Prior.
 *
 *   Das Spiel sollte auf gehaltene rechte Maustaste pruefen und in dem Fall
 *   der Lix mit der niedrigsten Prioritaet, die aber in jedem Fall
 *   mindestens 2 ist, bevorzugen.
 *
 * bool is_solid(int x = 0, int y = 2)
 *
 *   Diese Funktion kontrolliert, ob sich x Pixel vor und y Pixel unter der
 *   effektiven Lix-Position (ex|ey) ein Stueck Landschaft befindet. Dabei
 *   ist zu beachten, dass die Richtung der Lix bereits beruecksichtigt
 *   wird - positive Werte fuer x liegen immer vor der Lix. Positive Werte
 *   fuer Y liegen unter, negative Werte ueber dem Fuss der Lix.
 *
 *   is_solid();
 *   ...liefert aufgrund der Standardwerte 0 und 2 true, wenn sich unter dem
 *   Fuss der Lix Boden befindet. Boden unter den Fuessen ist fuer die
 *   meisten Aktionen notwendig :-)
 *
 * int solid_wall_height(int x = 0, int y = 1)
 *
 *   Prueft, ob sich x|y Pixel vor ex|ey in Blickrichtung eine unbesteig-
 *   bare Mauer befindet. Hierzu wird mehrmals is_solid() aufgerufen. Zurueck-
 *   gegeben wird die Hoehe der Mauer in Pixeln.
 *
 * void move_ahead(int x = 2)
 * void move_down(int y = 2)
 * void move_up(int y = 2)
 *
 *   Diese Funktionen bewegen sowohl das Bildobjekt als auch eine der effek-
 *   tiven Koordinaten der Lix in die entsprechende Richtung. Wichtig ist
 *   fuer move_ahead(), dass sie die Richtung der Lix beruecksichtigt.
 *
 * int count_solid(int x1, int y1, int x2, int y2)
 *
 *   Kontrolliert, wie viele Erd- oder Stahlpixel sich im befragten Rechteck,
 *   ausgegangen von (ex|ey) mit Richtungsberücksichtigung, befinden. Die Zahl
 *   der gefundenen Nicht-Luftpixel wird zurückgegeben.
 *
 * int count_steel(int x1, int y1, int x2, int y2)
 *
 *   Zaehlt die Anzahl der Stahlpixel im Rechteck relativ zu (ex|ey) und gibt
 *   sie zurueck.
 *
 * bool remove_pixel(int x, int y)
 *
 *   x und y sind relativ zu (ex|ey) anzugeben, wobei ein positiver Wert für
 *   x aufgrund der Richtungsbrücksichtigung immer vor der Lix verweist.
 *   Entfernt den Pixel, falls es abbaubares Material ist. In diesem Fall oder
 *   im Fall, dass der Pixel bereits Luft war, wird false zurückgegeben.
 *   Ist es allerdings unabbaubarer Stahl, wird true zurückgegeben.
 *
 * int remove_rectangle(int x1, int y1, int x2, int y2)
 *
 *   Alle vier Werte sind relativ zu (ex|ey) anzugeben, die Richtung des Lem-
 *   mings wird ebenfalls berücksichtigt. Alle Punkte, die innerhalb oder auf
 *   dem Rand des angegebenen Rechtecks liegen, werden mit remove_pixel() be-
 *   arbeitet. War mindestens ein Stahlpixel dabei, wird true zurückgegeben,
 *   sonst false.
 *
 * void draw_pixel(int x, int y, int col)
 *
 *   Kontrolliert, ob sich an der angegebenen Stelle relativ zu (ex|ey) ein
 *   Stück Landschaft befindet, und erschafft, falls dies nicht so ist, einen
 *   abbaubaren Landschaftspixel in der angegebenen Farbe.
 *
 *   Im Gegensatz zu den Landschaftsentfernungs-Funktionen hat draw_pixel()
 *   keinen Rückgabewert! Hindernisse beim Bauen müssen anderweitig erkannt
 *   werden. Dies war in Lemmings auch nicht anders, da Bauen und
 *   Landschaftsprüfungen in verschiedenen Frames stattfanden.
 *
 * void draw_rectangle(int x1, int y1, int x2, int y2, int col)
 *
 *   Ruft nacheinander draw_pixel() für alle im Rechteck liegenden Pixel auf.
 *
 * int get_frame()
 *
 *   Liefert x_frame - 2, denn die beiden Icons für die Steuerleiste müssen
 *   abgezogen werden, die sich links von der Animation befinden.
 *
 * bool is_last_frame()
 *
 *   Liefert true, wenn nur noch Leere hinter dem aktiven Frame ist. Dies ist
 *   nüztlich bei Abfragen, die etwas Bestimmtes beim letzten Frame machen.
 *   So muss keine Zahl angepasst werden, wenn sich die Anzahl der Frames nach-
 *   träglich ändert.
 *
 * void next_frame(int = 0)
 *
 *   Zeigt beim folgenden Zeichnen das naechste Frame der Lix an. Die Zahl
 *   des Frames wird auf 0 zurueck gesetzt, wenn sie so gross geworden ist wie
 *   das angegebene Argument oder aber das letzte Frame im gemalten Bitmap ist.
 *
 * void assign(const Ac)
 *
 *   Wechselt die Aktion, macht auch noch mehr als nur set_ac(const Ac).
 *
 *   Dass der Roper den Spieler dazu bringt, zu zielen, wird hier nicht
 *   geregelt. Dazu ist evaluate_click() gut.
 *
 * bool get_pass_top()
 *
 *   Liefert true, wenn die Lix auf Nicht-Walzenkarten oben aus dem
 *   Bildschirm fliegen darf. Passiert etwa bei Tumblern.
 *
 * bool get_nukable()
 *
 *   Liefert true, wenn die Nuke-Abarbeitung der Gameplay-Klasse diese Lix
 *   erwischen soll. Ist nur in wenigen Faellen nicht so, etwa, wenn
 *   die Lix gerade schon stirbt. Auch das Weggeschleudere bei Explosionen
 *   betrifft nur nukbare Lix.
 *
 * bool get_aiming()
 *
 *   Liefert, ob die Faehigkeit grundsaetzlich zielen kann && ob die Lix
 *   es gerade tut. Ob sie gerade zielt, haengt per Konvention von special_x
 *   ab! Wenn special_x == 0 ist, so zielt die Lix bzw. bereitet sich
 *   darauf vor; in jedem Fall ist die Maus nun das Zielgeraet.
 *
 */

#pragma once

#include <map>
#include <string>

#include "lix_enum.h"

#include "../editor/graph_ed.h"
#include "../gameplay/lookup.h"
#include "../graphic/map.h" // Map::distance_x/y
#include "../graphic/graph_bg.h"
#include "../graphic/sound.h"
#include "../other/types.h" // AlCol

struct UpdateArgs;

class EffectManager;
class Tribe;
class Goal;

class Lixxie : public Graphic {

public:

    struct AcFunc {
        bool      pass_top;
        bool      leaving;
        bool      blockable;
        bool      aiming;
        int       returns_x;
        Sound::Id sound_assign;
        Sound::Id sound_become;
        Sound::Id sound_aim;
        void (*assclk)(Lixxie&);
        void (*become)(Lixxie&);
        void (*update)(Lixxie&, const UpdateArgs&);
        AcFunc();
        ~AcFunc();
    };

private:

    static std::map    <LixEn::Ac,    std::string> ac_string;
    static std::map    <LixEn::Style, std::string> style_string;
    static std::vector <std::vector <int> > style_color;

    static Torbit*        land;
    static Lookup*        lookup;
    static Map*           ground_map;
    static EffectManager* effect;

    static std::vector <AcFunc> ac_func;

    Tribe* tribe;
    char   marked;

    int  ex;
    int  ey;
    int  dir;
    int  special_x;
    int  special_y;

    char frame;
    char updates_since_bomb;
    char exploder_knockback;

    char runner;
    char climber;
    char floater;

    Lookup::LoNr enc_body;
    Lookup::LoNr enc_foot;

    LixEn::Style style;
    LixEn::Ac    ac;

    void draw_at(const int, const int);

public:

    static const int distance_safe_fall;
    static const int distance_float;
    static const int updates_for_bomb;
    static const int updates_roper_before;
    static const int updates_roper_after;

    Lixxie(Tribe* = 0, int = 0, int = 0); // tribe == 0 ? NOTHING : FALLER
    ~Lixxie();

    static void initialize_this_gets_called_from_glob_gfx_cpp();

    static void           set_static_maps   (Torbit&, Lookup&, Map&);
    static void           set_effect_manager(EffectManager& e) { effect = &e; }
    static EffectManager* get_ef()                             {return effect;}
    static const Torbit&  get_land() { return *land; }

    inline bool get_mark() { return marked;  }
    inline void mark()     { marked = true;  }
    inline void unmark()   { marked = false; }

    inline Tribe&     get_tribe() const { return *tribe; }
    inline LixEn::Style get_style() const { return style;   }

    inline int  get_ex() const { return ex; }
    inline int  get_ey() const { return ey; }
           void set_ex(const int);
           void set_ey(const int);

           void move_ahead(int = 2);
           void move_down(int = 2);
           void move_up(int = 2);

    inline int  get_dir() const      { return dir; }
    inline void set_dir(const int i) { dir = i>0 ? 1 : i<0 ? -1 : dir; }
           void turn();

           bool get_in_trigger_area(const EdGraphic&) const;

    inline LixEn::Ac get_ac() const               { return ac;   }
    inline void    set_ac(const LixEn::Ac new_ac) { ac = new_ac; }

    inline bool      get_pass_top () const { return ac_func[ac].pass_top;  }
    inline bool      get_leaving  () const { return ac_func[ac].leaving;   }
    inline bool      get_blockable() const { return ac_func[ac].blockable; }
    inline bool      get_aiming   () const { return ac_func[ac].aiming
                                                    && special_x == 0;     }
    inline Sound::Id get_sound_assign() const{return ac_func[ac].sound_assign;}
    inline Sound::Id get_sound_become() const{return ac_func[ac].sound_become;}
    inline Sound::Id get_sound_aim   () const{return ac_func[ac].sound_aim;   }

    inline static const AcFunc& get_ac_func(LixEn::Ac a) { return ac_func[a]; }

    void        evaluate_click(const LixEn::Ac);
    unsigned    get_priority  (const LixEn::Ac, int, bool);

    inline int  get_special_x()      { return special_x; }
    inline int  get_special_y()      { return special_y; }
    inline void set_special_x(int n) { special_x = n;    }
    inline void set_special_y(int i) { special_y = i;    }

    inline int  get_updates_since_bomb()      { return updates_since_bomb; }
    inline void inc_updates_since_bomb()      { ++updates_since_bomb;      }
    inline void set_updates_since_bomb(int i) { updates_since_bomb = i;    }

    inline bool get_exploder_knockback() const { return exploder_knockback; }
    inline void set_exploder_knockback(bool b = true) { exploder_knockback=b; }

    inline bool get_runner () const { return runner;  }
    inline bool get_climber() const { return climber; }
    inline bool get_floater() const { return floater; }
    inline void set_runner ()       { runner  = true; }
    inline void set_climber()       { climber = true; }
    inline void set_floater()       { floater = true; }

    static bool get_steel_absolute(const int,     const int    );
           bool get_steel         (const int = 0, const int = 0);
           void add_land          (int = 0, int = 0, AlCol = color[COL_PINK]);
           void add_land_absolute (int = 0, int = 0, AlCol = color[COL_PINK]);
           bool is_solid          (const int = 0, const int = 2);
           bool is_solid_single   (const int = 0, const int = 2);
           int  solid_wall_height (const int = 0, const int = 0);
           int  count_solid       (const int, const int, const int, const int);
           int  count_steel       (const int, const int, const int, const int);

           bool remove_pixel         (const int, const int);
    static void remove_pixel_absolute(const int, const int);
           bool remove_rectangle     (int, int, int, int);

           void draw_pixel       (int, int, int);
           void draw_rectangle   (int, int, int, int, int);
           void draw_brick       (int, int, int, int);
           void draw_frame_to_map(int, int, int, int, int, int, int, int);

           void play_sound        (const UpdateArgs&, Sound::Id);
           void play_sound_if_trlo(const UpdateArgs&, Sound::Id);//see effect.h

    inline int  get_frame() const      { return frame; }
    inline void set_frame(const int i) { frame = i;    }

           bool is_last_frame();
           void next_frame(int = 0);

    inline Lookup::LoNr get_body_encounters()    { return enc_body;         }
    inline Lookup::LoNr get_foot_encounters()    { return enc_foot;         }
    inline void         set_no_encounters()      { enc_body = enc_foot = 0; }
    inline void         set_body_encounters(Lookup::LoNr n) { enc_body = n; }
    inline void         set_foot_encounters(Lookup::LoNr n) { enc_foot = n; }

           void assclk        (const LixEn::Ac);
           void become        (const LixEn::Ac);
           void become_default(const LixEn::Ac);
           void update        (const UpdateArgs&);

           void draw();

    // Die Offsets fuer den Countdown
    struct XY {
        int x;
        int y;
    };
    typedef std::vector <std::vector <XY> > Matrix;
    static  Matrix countdown;
};
