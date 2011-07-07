/*
 * other/date.h
 *
 * Kalenderzeiten als Info, wann ein Level oder ein Replay entstanden ist.
 * Kann zur Ueberpruefung, ob das Replay ggf. nicht mehr dem Level ent-
 * spricht, genutzt werden.
 *
 * Date::Date()
 *
 *   konstruiert ein Objekt mit der aktuellen Kalenderzeit.
 *
 * Date::Date(const std::string&)
 *
 *   Liest ueber den String, der im selben Format vorliegen sollte wie die
 *   Ausgabe von Date::operator <<, und konstruiert damit ein neues Objekt.
 *   Die Funktion ruft einfach Date::operator = (const std::string&) auf.
 *
 */

#pragma once

#include <string>
#include <ostream>

class Date {

private:

    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;

    friend std::ostream& operator << (std::ostream&, const Date&);
    friend std::string&  operator << (std::string&,  const Date&);

public:

    Date();
    Date(const std::string&);
    ~Date();

    Date& operator = (const std::string&);

    bool operator == (const Date&) const;
    bool operator != (const Date&) const;
    bool operator <  (const Date&) const;
    bool operator >  (const Date&) const;
    bool operator <= (const Date&) const;
    bool operator >= (const Date&) const;

};

std::ostream& operator << (std::ostream&, const Date&);
std::string&  operator << (std::string&,  const Date&);
