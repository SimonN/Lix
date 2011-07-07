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
    // Read in a string that is separated by any non-digit characters
    for (std::string::const_iterator
        itr = str.begin(); itr != str.end(); ++itr)
        if (*itr >= '0' && *itr <= '9') p.push_back(*itr - '0');
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
        o << static_cast <int> (p[i]);
        if (i < p.get_size() - 1) o << ' ';
    }
    return o;
}
