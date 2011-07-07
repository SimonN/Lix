/*
 * network/permu.h
 *
 * A permutation of any given length.
 *
 */

#pragma once

#include <string>
#include <vector>

class Permu {

public:

    Permu() {}                       // The empty permutation;
    Permu(size_t);                   // Generate at random with length (arg)
    Permu(size_t, const char*);      // Read in (1st arg) bytes
    Permu(const std::string&);       // Reads space-spearated string of nrs

    inline size_t get_size()  const  {  return p.size(); }
    char operator [] (size_t) const; // Returns the nth byte

    void shorten_to(size_t);         // Erases all bytes higher than (arg)

    bool operator == (const Permu&) const;

private:

    std::vector <char> p;

};

std::ostream& operator << (std::ostream&, const Permu&);
