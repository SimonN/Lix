/*
 * other/date.cpp
 *
 */

#include <ctime>
#include <sstream>

#include "date.h"

Date::Date()
{
    time_t timestamp = time(0);
	tm*    now       = localtime(&timestamp);

	year   = now->tm_year + 1900;
	month  = now->tm_mon  +    1;
    day    = now->tm_mday;
    hour   = now->tm_hour;
    minute = now->tm_min;
    second = now->tm_sec;
}



Date::Date(const std::string& s)
{
    *this = s;
}



Date::~Date()
{
}



Date& Date::operator = (const std::string& s)
{
    year = month = day = hour = minute = second = 0;
    int what = 0;
    for (std::string::const_iterator itr = s.begin();
     itr      != s.end() && what <  14;  ++itr)
     if (*itr >= '0'     && *itr <= '9') {
        const int digit = *itr - '0';
        switch (what++) {
        case  0: year   += digit * 1000; break;
        case  1: year   += digit * 100;  break;
        case  2: year   += digit * 10;   break;
        case  3: year   += digit;        break;
        case  4: month  += digit * 10;   break;
        case  5: month  += digit;        break;
        case  6: day    += digit * 10;   break;
        case  7: day    += digit;        break;

        case  8: hour   += digit * 10;   break;
        case  9: hour   += digit;        break;
        case 10: minute += digit * 10;   break;
        case 11: minute += digit;        break;
        case 12: second += digit * 10;   break;
        case 13: second += digit;        break;
        default:                         break;
        }
    }
    return *this;
}



std::ostream& operator << (std::ostream& o, const Date& d)
{
    if (d.year == 0) o << "000";
                                 o << d.year   << '-';
    if (d.month  < 10) o << '0'; o << d.month  << '-';
    if (d.day    < 10) o << '0'; o << d.day    << ' ';
    if (d.hour   < 10) o << '0'; o << d.hour   << ':';
    if (d.minute < 10) o << '0'; o << d.minute << ':';
    if (d.second < 10) o << '0'; o << d.second;
    return o;
}



std::string& operator << (std::string& s, const Date& d)
{
    std::ostringstream o;
    o << d;
    s += o.str();
    return s;
}



bool Date::operator == (const Date& d) const
{
    return year   == d.year
     &&    month  == d.month
     &&    day    == d.day
     &&    hour   == d.hour
     &&    minute == d.minute
     &&    second == d.second;
}



bool Date::operator != (const Date& d) const
{
    return ! (*this == d);
}



bool Date::operator < (const Date& d) const
{
    return year   != d.year   ? year   < d.year
     :     month  != d.month  ? month  < d.month
     :     day    != d.day    ? day    < d.day
     :     hour   != d.hour   ? hour   < d.hour
     :     minute != d.minute ? minute < d.minute
     :     second != d.second ? second < d.second
     :     false; // alle gleich
}



bool Date::operator > (const Date& d) const
{
    return *this != d
     && ! (*this <  d);
}



bool Date::operator <= (const Date& d) const
{
    return *this == d
     ||    *this <  d;
}



bool Date::operator >= (const Date& d) const
{
    return *this == d
     ||    *this >  d;
}
