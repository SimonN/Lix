/*
 * other/io.h
 *
 * Einlesen und Ausgeben von Textdateien, etwa Konfigurationsdateien, Levels
 * oder Replays.
 *
 * Der dem Konstruktor von IO::Line uebergebene String, der aus einer
 * Datei eingelesen worden ist, sollte keinen Zeilenumbruch am Ende haben.
 *
 * Es gibt ferner IO::Line-Konstruktoren der Bauart Line(char, std::string...,
 * int...). Diese nehmen die Argumente nicht in der Reihenfolge, in der sie
 * spaeter in der Datei stehen werden! Das liegt daran, dass diese Konstruk-
 * toren fuer beliebige Steuerzeichen (erstes Argument) und damit fuer
 * beliebige Anordnungen in der Datei verwendet werden koennen. Die Anordnung
 * wird einzig im Operator << festgelegt, und zwar anhand des Steuerzeichens.
 *
 * Die ganze Angelegenheit beim Lesen und Schreiben erfordert etwas mehr
 * Aufwand als direktes Schreiben in jeder Klasse: Hier muss erst ein ganzes
 * IO::Line-Objekt initialisiert und befuellt werden. Anschliessend wird es
 * ausgewertet oder aus ihm geschrieben und es wird wieder gekillt. Der Vorteil
 * dieser Vorhergehensweise ist, dass das Auslesen sehr robust durchgefuehrt
 * wird und praktisch keine Abstuerze oder Haenger durch halbe Textdatei-
 * -Zeilen mehr auftreten! Beim Dateiladen und -schreiben haben wir ohnehin
 * genuegend Zeit, da koennen wir diesen Weg ruhig einschlagen.
 *
 * bool fill_vector_from_file(std::vector <Line>&, const std::string&);
 *
 *   Befuellt den Vektor mit den Zeilen aus der Datei, deren Dateiname im
 *   zweiten Argument angegeben ist. Die Rueckgabe ist false, falls die Datei
 *   nicht existiert, ansonsten true - auch bei einer leeren Datei.
 *
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>

#include "date.h"
#include "globals.h"

namespace IO {

struct Line {
    char type;
    std::string text1;
    std::string text2;
    std::string text3;
    long nr1;
    long nr2;
    long nr3;

    // Eingabe: Der eingelesene String hat keinen Zeilenumbruch am Ende.
    Line(const std::string& = gloB->empty_string);

    Line(const char,
         const std::string&, const std::string&, const std::string&,
         const long,         const long,         const long);

private:

    void iter_to_long(std::string::const_iterator&, long&, bool&);

};

// Ausgabe: Generiere eine Zeile aus uebergebenen Programmdaten.
// Diese Dinger sehen genauso aus wie Konstruktoren, das ist gewollt.
// LineDollar kann auch mit einer Kalenderzeit gebastelt werden.
Line LineHash  (const std::string& = gloB->empty_string,
                const long         = 0);
Line LineColon (const std::string& = gloB->empty_string,
                const long         = 0,
                const long         = 0,
                const std::string& = gloB->empty_string);
Line LineDollar(const std::string& = gloB->empty_string,
                const std::string& = gloB->empty_string);
Line LineDollar(const std::string&,
                const Date&);
Line LinePlus  (const std::string& = gloB->empty_string,
                const long         = 0,
                const std::string& = gloB->empty_string,
                const std::string& = gloB->empty_string);
Line LineBang  (const long         = 0,
                const long         = 0,
                const std::string& = gloB->empty_string,
                const long         = 0);
Line LineAngle (const std::string& = gloB->empty_string,
                const long         = 0,
                const long         = 0,
                const long         = 0,
                const std::string& = gloB->empty_string);

// Verarbeiten von Line-Objekten
std::ostream& operator << (std::ostream&, const Line&);

typedef                      std::vector <Line> ::const_iterator LineIt;
bool fill_vector_from_file  (std::vector <Line>&, const std::string&);
bool fill_vector_from_stream(std::vector <Line>&,       std::istream&);

}
// Ende Namensraum IO
