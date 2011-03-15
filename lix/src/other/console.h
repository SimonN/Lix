/*
 * other/console.h
 *
 * Die Konsole zeigt Chat-Nachrichten an, aber manchmal auch Nachrichten
 * im Einzelspieler-Modus. In der Lobby erscheint eine vergroesserte Version
 * des Chats.
 *
 * Dies ist eine Singleton-Klasse, aehnlich wie Log. Das Objekt wird privat
 * und statisch erschaffen. Zugriff erfolgt ueber statische Funktionen der
 * Log-Klasse.
 *
 * static void push_back(std::string, bool)
 *
 *   In dieser Funktion sind einige Zeilenumbruch-Regeln hartkodiert! Wir
 *   nehmen an, dass die Konsole nur so viele Zeichen nebeneinander haben will,
 *   wie in den Lobby-Chat bei der dortigen Schriftgroesse passen.
 *
 */

#pragma once

#include "help.h" // Timer

#include <list>
#include <string>

class Console {

public:

    static const int lines_max_number;
    static const int lines_max_ticks;

    struct Line {
        unsigned long time;
        std::string   text;
        bool          white;
        inline        Line(const std::string& t, const bool w = false)
                      : time(Help::timer_ticks), text(t), white(w) {}
    };
    typedef std::list <Line> ::const_iterator LiIt;

private:

    static Console con;

    // Alle wesentlichen anderen Konstanten sind nicht statisch
    const int      lines_maxl;

    std::list <Line> line;

    // Erzeugungs- und Kopierverbot
    Console();
    Console(const Console&);
    ~Console();
    void operator = (const Console&);

public:

    static const std::list <Line>& get_lines_all();
    static const std::list <Line>& get_lines_recent();

    static        void push_back(const std::string& t, const bool w = false);
    static inline void clear()  { con.line.clear(); }

};
