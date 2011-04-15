/*
 * gameplay/arrow.h
 *
 * Die Pfeile mit Faehigkeitssymbolen, die ueber einzelnen Lemmingen beim
 * Replay eingeblendet werden. Verwaltet werden sie in der Klasse ArrowList,
 * von denen Gameplay eine Instanz besitzen sollte.
 *
 * Aehnlich wie Explosionen sollte bei diesen Pfeilen einmal pro Gameplay-
 * Berechnung calc() aufgerufen werden. Das erhoeht das Alter der Pfeile.
 * Dies wird uebernommen durch ArrowList.
 *
 * bool Arrow::operator == (const Arrow&)
 *
 *   Vergleicht x, update, lem_id, Style und Skill. Vergleicht nicht y, denn y
 *   aendert sich ja im Laufe der Zeit. Dies dient dazu, bei vielen Neurech-
 *   nungen aufgrund eintreffender Pakete nicht einen Pfeil zweimal zu zeigen.
 *   Ist etwas hackerisch, die Loesung - aber sonst muesste man nochmal die
 *   Verarbeitungsstrukturen des Replays aufbrechen, najanaja.
 *
 * class Debris
 *
 *   Muss eine Map als Grundlage bekommen, nicht nur ein Torbit. Der Sinn
 *   ist, dass auf kleinen Karten die Sterne nicht so lange leben, wenn die
 *   Karte senkrecht torusscrollt. Dazu ist Map::min_h abzufragen.
 *
 *   Entweder ist das Debris eine Grafik (Hacke oder Sterne usw., oder
 *   aber ein Pixel (in Wirklichkeit 2 x 2 Pixel), die ohne Grafik direkt
 *   primitiv aufs Torbit gezeichnet werden.
 *
 */

#pragma once

#include <list>

#include "../graphic/graphic.h"
#include "../graphic/map.h"
#include "../lix/lix_enum.h"

struct Arrow : public Graphic {

    static const int offset_x;  // Dies abziehen von den Konstruktor-
    static const int offset_y;  // Koordinaten
    static const int offset_sx; // Skill-Offset gegenueber Pfeil
    static const int offset_sy;

    Graphic       skill;
    int           age;
    unsigned long update;
    int           lem_id;

    Arrow(Torbit&, LixEn::Style = LixEn::GARDEN,
          const int = 0, const int = 0, LixEn::Ac = LixEn::NOTHING,
          const unsigned long = 0, const int = 0);
    ~Arrow();

    bool operator == (const Arrow&);

    void set_x (const int);
    void set_y (const int);

    void draw  ();

};



struct Debris {

    static const int ttl_min;
    static const int ttl_max;
    static const int ttl_pickaxe;
    static const int x_speed_max;
    static const int y_speed_min;
    static const int y_speed_max;

    static const int pixel_ttl_min;
    static const int pixel_ttl_max;
    static const int pixel_x_speed_max;
    static const int pixel_y_speed_min;
    static const int pixel_y_speed_max;

    static const int explosion_y_frame_min;

    Map&          map;
    const Cutbit* cutbit; // Wenn 0, so ist es Pixel.

    int    color; // bei Cutbits ist es y_frame.
    int    ttl;

    double x;
    double y;
    double x_speed;
    double y_speed;
    double rot;

    Debris(Map&, const int, const int); // Stern oder Wolke
    Debris(Map&, const int, const int, const LixEn::Style); // Pixel
    Debris(Map&, const int, const int, const int, const int); // y-Frame,rot
    ~Debris();

    inline int get_ttl() { return ttl; }

    void calc();
    void draw();

private:

    void common_constructor();

};
