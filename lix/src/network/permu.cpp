/*
 * network/permu.cpp
 *
 */

#include <cstdlib> // rand
#include <sstream>

#include "permu.h"

Permu::Permu(size_t n)
:
    p(n)
{
    // Put numbers 0, ..., (n-1) into the vector, then shuffle the vector.
    for (size_t i = 0; i < n; ++i) p[i] = i;
    for (size_t i = 0; i < n; ++i) std::swap(p[i], p[i + ::rand() % (n-i)]);
}



Permu::Permu(size_t n, const char* ptr)
:
    p(n)
{
    for (size_t i = 0; i < n; ++i) {
        p[i] = *ptr;
        ptr++;
    }
}



Permu::Permu(const std::string& str)
{
    bool last_was_digit = false;
    int  buffer         = 0;
    // Read in a string that is separated by any non-digit characters
    for (std::string::const_iterator
     itr = str.begin(); itr != str.end(); ++itr) {
        if (*itr >= '0' && *itr <= '9') {
            last_was_digit = true;
            buffer *= 10;
            buffer += *itr - '0';
        }
        else if (last_was_digit) {
            last_was_digit = false;
            p.push_back(buffer);
            buffer = 0;
        }
    }
    // Add the last number in case no non-digit followed
    if (last_was_digit) p.push_back(buffer);
}



char Permu::operator [] (size_t n) const
{
    if (n >= p.size()) return n;
    return p[n];
}



void Permu::shorten_to(size_t n)
{
    if (p.size() <= n) return;

    Permu permu_new;
    for (std::vector <char> ::const_iterator
     itr = p.begin(); itr != p.end(); ++itr)
     if (*itr < (int) n) permu_new.p.push_back(*itr);

    *this = permu_new;
}



bool Permu::operator == (const Permu& rhs) const
{
    return p == rhs.p;
}



std::ostream& operator << (std::ostream& o, const Permu& p)
{
    for (size_t i = 0; i < p.get_size(); ++i) {
        o << (int) p[i];
        if (i < p.get_size() - 1) o << ' ';
    }
    return o;
}
