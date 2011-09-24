/*
 * map.h
 *
 * Praktisch ein Torbit mit Kamera drauf.
 *
 * Dies ist eine Karte. Level und Editor besitzen solche Objekte.
 * Mit der Erstellung dieses Objektes wird ein neues, großes Bitmap erzeugt,
 * welches bei Zerstörung einer Map-Instanz ebenfalls zerstört wird.
 *
 * void calc_scrolling()
 *
 *   Sollte moeglichst frueh in der Spielschleife aufgerufen werden, da diese
 *   Funktion die Maus anhaelt, wenn die Rechtsscroll-Option genutzt wird!
 *   Ansonsten nutzen ggf. Teile des Codes des Programmteils, das die Map
 *   besitzt, andere Mauskoordinaten als uebrige Codeteile des gleichen
 *   Programmteils.
 *
 * Torus-Einstellung
 *
 *   Diese kann wie gewohnt fuer beide Richtungen separat eingestellt werden.
 *   Ist die Einstellung fuer eine Richtung aktiv, so kann ueber den Rand der
 *   Karte frei hinausgescrollt werden, und das entgegengesetzte Ende der
 *   Karte wird auf den freien Platz geblittet. Die Bildschirmkoordinaten
 *   werden immer zwischen einschliesslich 0 und (Kartenlaenge - 1) gehalten.
 *
 * void set_screen_x(int)
 * void set_screen_y(int)
 *
 *   Diese Funktionen setzen die obere linke Ecke des Bildschirmes an eine
 *   bestimmte Stelle. Dies scrollt also sofort an beliebige Orte. Das ist
 *   hauptsächlich sinnvoll zu Beginn eines Levels. Gewünschte Koordinaten,
 *   bei denen das Bitmap den Bildschirm nicht komplett füllt, werden auto-
 *   matisch angepasst.
 *
 * void scroll_x(int)
 * void scroll_y(int)
 *
 *   Diese Funktionen scrollen die Karte um die genannte Anzahl Pixel weiter.
 *   Soll über die Bitmap-Grenzen hinaus gescrollt werden, wird die Verschie-
 *   bung des sichtbaren Bitmap-Ausschnittes am Rand gestoppt. Bei Torus-Karten
 *   wird modulo Groesse gerechnet und weitergescrollt.
 *
 * int get_mouse_x()
 * int get_mouse_y()
 *
 *   Lesen die Allegro-Variablen hardware.get_mx() oder hardware.get_my() aus. Es wird zurueck
 *   gegeben, wo sich die Maus auf dem Land befindet, unter Beruecksichtigung
 *   des Zooms und der Scroll-Koordinaten. Die Zahl ist immer mindestens 0
 *   und hoechstens (Kartenlaenge - 1), auch bei Toruskarten.
 *
 * void draw(BITMAP*, bool manual_zoom = false)
 *
 *   Dies zeichnet den durch das Scrolling festgelegten Ausschnitt der Karte
 *   auf das angegebene Bitmap. Dieses sollte eines der Video-Bitmaps für das
 *   Pageflipping sein.
 *
 *   Falls manual_zoom = true, so wird unabhaengig von der eigenen Zoom-
 *   -Einstellung so geblittet, als sei der Zoom aus.
 *
 * bool zoom
 *
 *   Wenn falsch, wird der Bereich zwischen (screen_x|screen_y) und
 *   (screen_x + min_w|screen_y + min+h) gezeichnet. Andernfalls wird nur
 *   die Mitte dieses Bereiches mit Streckfaktor 2 in beide Richtungen
 *   gezeichnet.
 *
 *   Aktivierter Zoom erlaubt zudem Scrolling ueber die normalen Grenzen
 *   hinaus. Wird wieder herausgezoomt, werden diese uebermaessigen Scroll-
 *   Koordinaten korrigiert.
 *
 * void load_masked_screen_rectangle(BITMAP*);
 *
 *   Macht nur etwas, wenn das angegebene Quellbitmap dieselbe Groesse hat
 *   wie die Karte. In diesem Fall kopiert ein Rechteck von der Quelle auf
 *   das eigene Bitmap. Das Rechteck ist genau die aktuelle Bildschirmposition,
 *   unter Beruecksichtigung von Scrolling, Zoom und Toruseigenschaften.
 *
 * int distance_x(const int x1, const int x2)
 * int distance_y(const int y1, const int y2)
 *
 *   Berechnet x2 - x1 im Torus mit minimalem Ergebnis-Absolutbetrag. Das
 *   Ergebnis kann also auch negativ sein. Entsprechend fuer y2 - y1.
 *
 * double hypot(4x const int)
 *
 *   Berechnet die Hypothenuse je nach Toruseinstellung. Dabei wird einfach
 *   (distance_x ^ 2   +   distance_y ^ 2)   ^   (1/2) gerechnet.
 *
 */

#pragma once

#include "torbit.h"

class Map : public Torbit {

private:

    const int scroll_speed_edge;
    const int scroll_speed_click;

    const int screen_xl;
    const int screen_yl;

    int  screen_x;
    int  screen_y;
    int  min_screen_x; // Diese vier koennen durch Zoom veraendert werden
    int  min_screen_y;
    int  max_screen_x;
    int  max_screen_y;

    bool zoom;

    int  scroll_click_x;
    int  scroll_click_y;

    void draw_at(Torbit&, const int, const int);

public:

    //  w    h    screen-xl/yl
    Map(int, int, int, int);
    Map(const Map&);

    Map& operator = (const Map&);

    virtual void resize(int, int);

    virtual void set_torus_x(bool = true);
    virtual void set_torus_y(bool = true);

           bool get_scrollable();
           bool get_scrollable_up();
           bool get_scrollable_right();
           bool get_scrollable_left();
           bool get_scrollable_down();

    inline int  get_screen_xl()             { return screen_xl; }
    inline int  get_screen_yl()             { return screen_yl; }

    inline int  get_screen_x()              { return screen_x;  }
    inline int  get_screen_y()              { return screen_y;  }
           void set_screen_x(int);
           void set_screen_y(int);
           void set_screen_center(int, int);

    inline bool get_zoom()                  { return zoom;      }
           void set_zoom(const bool = true);

           int  get_mouse_x();
           int  get_mouse_y();

    void calc_scrolling();

    // Zeichenfunktionen
    virtual void draw(Torbit&);

    void load_masked_screen_rectangle(Torbit&);
    void clear_screen_rectangle(int = 0);

};
