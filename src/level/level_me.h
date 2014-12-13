// level/level_me.h
// quickly extract only the name, build time, and save requirement
// from a level file in any format

#pragma once

#include "../other/file/date.h"
#include "../other/file/filename.h"

struct LevelMetaData {

public:

    enum FileFormat {
        FORMAT_NOTHING,
        FORMAT_LIX,
        FORMAT_BINARY,
        FORMAT_LEMMINI
    };

    FileFormat  format;
    Date        built;
    int         required;
    std::string name_german;
    std::string name_english;

    LevelMetaData();
    LevelMetaData(const Filename&);

    const std::string& get_name() const;
          bool         get_file_exists() const;

private:

    void read_metadata_lix(const Filename&);
    void read_metadata_binary(const Filename&);
    void read_metadata_lemmini(const Filename&);

};
