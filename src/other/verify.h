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

    // ConvertAndOverwrite
    // If yes: read replay from disk, read whatever level the replay
    // verifier would read, convert the replay to the post-August-2015 replay
    // format, save the replay back to disk.
    enum ConvertAndOverwrite { no = false, yes = true };

    Verifier(const std::vector <std::string>&, ConvertAndOverwrite);
    ~Verifier();

private:

    typedef std::vector <std::string> Vec;
    typedef Vec::const_iterator       VecCIt;

    typedef std::vector <Filename>    FVec;
    typedef FVec::const_iterator      FVecCIt;

    FVec filenames_from_find_tree;

    void examine_string(const std::string&);
    void verify_filename(const Filename&);
    void convert_and_overwrite(const Filename&);

    static void static_put_to_vec(const Filename&, void*);

    ConvertAndOverwrite _cao;

    int nr_empty_dir;        // replay file directory given exists, but empty
    int nr_not_found;        // replay file given by cmd arg not found
    int nr_not_naming_level; // replay doesn't name a level file
    int nr_level_not_found;  // replay names level file that doesn't exist
    int nr_level_bad;        // replay names level file that exists, but bad
    int nr_level_ok_failing; // replay names level file, but doesn't solve it
    int nr_level_ok_solving; // replay names level file, solves that level

};
