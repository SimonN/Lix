/*
 * other/verify.h
 *
 * Lix replay verification during noninteractive mode
 *
 */

#pragma once

#include <vector>
#include <string>

#include "../gameplay/replay.h"

class Verifier {

public:

    Verifier(const std::vector <std::string>&);
    ~Verifier();

private:

    typedef std::vector <std::string> Vec;
    typedef Vec::const_iterator       VecCIt;

    typedef std::vector <Filename>    FVec;
    typedef FVec::const_iterator      FVecCIt;

    FVec filenames_from_find_tree;

    void examine_string(const std::string&);
    void verify_filename(const Filename&);

    static void static_put_to_vec(const Filename&, void*);

    int nr_empty_dir;        // replay file directory given exists, but empty
    int nr_not_found;        // replay file given by cmd arg not found
    int nr_not_naming_level; // replay doesn't name a level file
    int nr_level_not_found;  // replay names level file that doesn't exist
    int nr_level_bad;        // replay names level file that exists, but bad
    int nr_level_ok_failing; // replay names level file, but doesn't solve it
    int nr_level_ok_solving; // replay names level file, solves that level

};
