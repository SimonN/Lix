/*
 * lix.h
 *
 * class Lixxie -- the name of this class is chosen to be different from Lix,
 * because Lix appears as a substring of many type names. A Lixxie is a
 * single lix walking around.
 *
 *
 * Im Lemming-Bitmap (auffindbar im internen Ordner) sind in manchen Frames
 * einige Pixel rot. Sie markieren den Kopf des Lemmings, waehrend der Fuss
 * (effektive Koordinaten) immer an der gleichen Stelle im Frame gemalt ist.
 * Der rote Pixel wird ausgelesen, und seine Position pro Frame in der Matrix
 * countdown gespeichert. Anschliessend bekommt der rote Pixel die Farbe
 * des Pixels rechts neben ihm.
 *
 * static void initialize_this_gets_called_from_glob_gfx_cpp()
 *
 *   Wird von glob.gfx.cpp() aufgerufen. Fuellt die Vektoren und Maps
 *   mit Strings und Farben, und bemalt die kopierten Lemming-Bitmaps.
 *   Es gibt keine Funktion deinitialize() oder aehnliches.
 *
 * const int id
 *
 *   Kennziffer, die fuer Replays genutzt wird. Wird dem Lemming per
 *   Konstruktor zugewiesen.
 *
 * int ex
 * int ey
 *
 *   Ein Lemming wird nicht nach den X-/Y-Koordinaten sines Grafikobjekts be-
 *   rechnet, sondern nach den sogenannten effektiven X-/Y-Koordinaten ex, ey.
 *
 *   Diese Koordinaten kennzeichnen die obere linke Ecke des 2-x-2-Quadrates,
 *   welches sich ueblicherweise am Fuss des Lemmings befindet, unterhalb des
 *   vorderen Koerperrandes.
 *
 *   Schaut der Lemming allerdings nach links, so wird fuer alle Berechnungen
 *   vom Punkt (ex-1|ey) ausgegangen, welcher gleich weit vom linken Koerper-
 *   rand entfernt ist wie (ex|ey) bei einem nach rechts Schauenden.
 *
 * int dir
 *
 *   Diese Variable ist 1, wenn der Lemming nach rechts schaut, oder -1, falls
 *   der Lemming nach links schaut. Aus einer Klappe fallende Lemmings schauen
 *   grundsaetzlich nach rechts - das war schon im Original so.
 *
 * bool marked
 * bool get_mark()
 * void mark()
 * void unmark()
 *
 *   Eine Markierung dient lediglich dazu, den Lemming später zu bearbeiten:
 *   Die Hauptschleife bearbeitet zunächchst nur Arbeitstiere und markiert
 *   sie, dann durchläuft sie nochmals alle Lemmings. Hier werden Markierte
 *   entmarkiert, alle anderen aber bearbeitet.
 *
 *   Markierungen sind nicht zu verwechseln mit Highlighting (Hervorhebung)!
 *   Highlighting dient dazu, den Lemming für den Benutzer leichter anklickbar
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
 *   den Lemming mit der niedrigsten Prioritaet, die aber in jedem Fall
 *   mindestens 2 ist, bevorzugen.
 *
 * bool is_solid(int x = 0, int y = 2)
 *
 *   Diese Funktion kontrolliert, ob sich x Pixel vor und y Pixel unter der
 *   effektiven Lemming-Position (ex|ey) ein Stueck Landschaft befindet. Dabei
 *   ist zu beachten, dass die Richtung des Lemmings bereits beruecksichtigt
 *   wird - positive Werte fuer x liegen immer vor dem Lemming. Positive Werte
 *   fuer Y liegen unter, negative Werte ueber dem Fuss des Lemmings.
 *
 *   is_solid();
 *   ...liefert aufgrund der Standardwerte 0 und 2 true, wenn sich unter dem
 *   Fuss des Lemmings Boden befindet. Boden unter den Fuessen ist fuer die
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
 *   tiven Koordinaten des Lemmings in die entsprechende Richtung. Wichtig ist
 *   fuer move_ahead(), dass sie die Richtung des Lemmings beruecksichtigt.
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
 *   x aufgrund der Richtungsbrücksichtigung immer vor den Lemming verweist.
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
 *   werden. Dies war im Original-Lemmings auch nicht anders, da Bauen und
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
 *   Zeigt beim folgenden Zeichnen das naechste Frame des Lemmings an. Die Zahl
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
 *   Liefert true, wenn der Lemming auf Nicht-Walzenkarten oben aus dem
 *   Bildschirm fliegen darf. Passiert etwa bei Tumblern.
 *
 * bool get_nukable()
 *
 *   Liefert true, wenn die Nuke-Abarbeitung der Gameplay-Klasse diesen
 *   Lemming erwischen soll. Ist nur in wenigen Faellen nicht so, etwa, wenn
 *   der Lemming gerade schon stirbt. Auch das Weggeschleudere bei Explosionen
 *   betrifft nur nukbare Lemminge.
 *
 * bool get_aiming()
 *
 *   Liefert, ob die Faehigkeit grundsaetzlich zielen kann && ob der Lemming
 *   es gerade tut. Ob er gerade zielt, haengt per Konvention von special_x
 *   ab! Wenn special_x == 0 ist, so zielt der Lemming bzw. bereitet sich
 *   darauf vor; in jedem Fall ist die Maus nun das Zielgeraet.
 *
 */

#pragma once

#include <map>
#include <string>

#include "lix_enum.h"

#include "../editor/graph_ed.h"
#include "../graphic/map.h" // Map::distance_x/y
#include "../graphic/graph_bg.h"
#include "../graphic/sound.h"

struct UpdateArgs;

class EffectManager;
class Tribe;
class Goal;

class Lixxie : public Graphic {

private:

    static std::map    <LixEn::Ac,    std::string> ac_string;
    static std::map    <LixEn::Style, std::string> style_string;
    static std::vector <std::vector <int> > style_color;

    static Torbit*        land;
    static Torbit*        steel_mask;
    static Map*           ground_map;
    static EffectManager* effect;

    struct AcFunc {
        bool      pass_top;
        bool      nukable;
        bool      aiming;
        int       returns_x;
        Sound::Id aim_sound;
        void (*assign)(Lixxie&);
        void (*update)(Lixxie&, const UpdateArgs&);
        AcFunc();
        ~AcFunc();
    };
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

    char climber;
    char floater;

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

    static void           set_static_maps   (Torbit&, Torbit&, Map&);
    static void           set_effect_manager(EffectManager& e) { effect = &e; }
    static EffectManager* get_ef()                             {return effect;}

    inline bool get_mark() { return marked;  }
    inline void mark()     { marked = true;  }
    inline void unmark()   { marked = false; }

    inline Tribe&     get_tribe() const { return *tribe; }
    inline LixEn::Style get_style() const { return style;   }

    inline int get_ex() const { return ex; }
    inline int get_ey() const { return ey; }
          void set_ex(const int);
          void set_ey(const int);

    void move_ahead(int = 2);
    void move_down(int = 2);
    void move_up(int = 2);

    inline int  get_dir() const      { return dir; }
    inline void set_dir(const int i) { dir = i>0 ? 1 : i<0 ? -1 : dir; }
    void turn();

    // true = double trigger area in x direction (not y direction)
    bool get_in_trigger_area(const EdGraphic&, const bool = false) const;

    inline LixEn::Ac get_ac() const               { return ac;   }
    inline void    set_ac(const LixEn::Ac new_ac) { ac = new_ac; }

    inline bool      get_pass_top () const { return ac_func[ac].pass_top;  }
    inline bool      get_nukable  () const { return ac_func[ac].nukable;   }
    inline Sound::Id get_aim_sound() const { return ac_func[ac].aim_sound; }
    inline bool      get_aiming   () const { return ac_func[ac].aiming
                                                    && special_x == 0;     }

    void        evaluate_click(const LixEn::Ac);
    unsigned    get_priority  (const LixEn::Ac, const unsigned,
                               const std::vector<Goal>&, const bool);

    inline int  get_special_x()      { return special_x; }
    inline int  get_special_y()      { return special_y; }
    inline void set_special_x(int n) { special_x = n;    }
    inline void set_special_y(int i) { special_y = i;    }

    inline int  get_updates_since_bomb()      { return updates_since_bomb; }
    inline void inc_updates_since_bomb()      { ++updates_since_bomb;      }
    inline void set_updates_since_bomb(int i) { updates_since_bomb = i;    }

    inline bool get_exploder_knockback() const { return exploder_knockback; }
    inline void set_exploder_knockback(bool b = true) { exploder_knockback=b; }

    inline bool get_climber() const { return climber; }
    inline bool get_floater() const { return floater; }
    inline void set_climber()       { climber = true; }
    inline void set_floater()       { floater = true; }

    static bool get_steel_absolute(const int,     const int    );
    const  bool get_steel         (const int = 0, const int = 0);
    const  int  get_pixel         (const int = 0, const int = 0);
           void set_pixel         (const int = 0, const int = 0,
                                   const int = color[COL_PINK]);
           bool is_solid          (const int = 0, const int = 2);
           int  solid_wall_height (const int = 0, const int = 0);
           int  count_solid       (const int, const int, const int, const int);
           int  count_steel       (const int, const int, const int, const int);

           bool remove_pixel         (const int, const int);
    static void remove_pixel_absolute(const int, const int);
           bool remove_rectangle     (int, int, int, int);

           void draw_pixel    (int, int, int);
           void draw_rectangle(int, int, int, int, int);
           void draw_brick    (int, int, int, int);

           void play_sound        (const UpdateArgs&, Sound::Id);
           void play_sound_if_trlo(const UpdateArgs&, Sound::Id);//see effect.h

    inline int  get_frame()            { return frame; }
    inline void set_frame(const int i) { frame = i;    }

           bool is_last_frame();
           void next_frame(int = 0);

           void assign        (const LixEn::Ac);
           void assign_default(const LixEn::Ac); // Fuer acfunc

           void update(const UpdateArgs&);

           void draw();

    // Die Offsets fuer den Countdown
    struct XY {
        int x;
        int y;
    };
    typedef std::vector <std::vector <XY> > Matrix;
    static  Matrix countdown;
};
