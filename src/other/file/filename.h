/*
 * Filename encapsulates a file name, so it can be used either with or
 * without the root directory. It determines automatically whether it's
 * initialized with a string containing the root dir or not.
 *
 * This is not intended to be used for pathless filenames.
 *
 * It can, however, be used for directories, as long as the relative path
 * is included, either with or without the root dir.
 *
 */

#pragma once
#include <string>

class Filename {

private:

    typedef const std::string& StR;

public:

    static void set_root_dir(const std::string&);

    explicit Filename(const std::string&);

    bool operator == (const Filename& rhs)   const;
    bool operator != (const Filename& rhs)   const;
    bool operator <  (const Filename& rhs)   const;

    inline StR  get_rootful ()               const { return rootful;         }
    inline StR  get_rootless()               const { return rootless;        }

    inline StR  get_extension()              const { return extension;       }
    inline StR  get_rootless_no_extension()  const { return rootless_no_ext; }
    inline StR  get_file()                   const { return file;            }
    inline StR  get_file_no_ext_no_pre_ext() const { return file_no_exts;    }
    inline StR  get_dir_rootful()            const { return dir_rootful;     }
    inline StR  get_dir_rootless()           const { return dir_rootless;    }

    inline char get_pre_extension()          const { return pre_extension;   }

private:

    static std::string root;

    std::string rootful,
                rootless,
                extension,
                rootless_no_ext,
                file,
                file_no_exts,
                dir_rootful,
                dir_rootless;

    char pre_extension;

};
