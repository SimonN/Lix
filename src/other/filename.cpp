#include "filename.h"
#include "globals.h"

std::string Filename::root;

void Filename::set_root_dir(const std::string& s)
{
    if (root.empty()) root = s;
}



Filename::Filename(const std::string& s)
:
    // All strings start empty.
    pre_extension('\0')
{
    // Possible root dirs are "./" and "../". We erase everything from the
    // start of the filename that is '.' or '/' and call that rootless.
    size_t sos = 0; // start_of_rootless
    while (sos < s.length() && (s[sos] == '.' || s[sos] == '/')) ++sos;
    rootless = s.substr(sos, std::string::npos);
    rootful  = root + rootless;

    // Determine the extension.
    int last_dot = rootless.length() - 1;
    int extension_length = 0;
    while (last_dot >= 0 && rootless[last_dot] != '.') {
        --last_dot;
        ++extension_length;
    }
    // last_dot == -1 if there is no dot in the filename
    // Don't mistake a pre-extension (1 uppercase letter) for an extension
    if (last_dot >= 0 && (extension_length >= 2
        || (extension_length >= 1 &&
            ! (rootless[last_dot + 1] >= 'A' && rootless[last_dot + 1] <= 'Z')
        ))) {
        extension       = rootless.substr(last_dot, std::string::npos);
        rootless_no_ext = rootless.substr(0, last_dot);
    }
    else {
        // extension stays empty
        rootless_no_ext = rootless;
    }
    // Determine the pre-extension or leave it at '\0'
    if (last_dot >= 2 && rootless[last_dot - 2] == '.'
        && (rootless[last_dot - 1] >= 'A' && rootless[last_dot - 1] <= 'Z'))
        pre_extension = rootless[last_dot - 1];

    // Determine the file. This is done similar as finding the extension.
    int last_slash = rootless.length() - 1;
    while (last_slash >= 0 && rootless[last_slash] != '/') --last_slash;
    if (last_slash >= 0) {
        file         = rootless.substr(last_slash + 1, std::string::npos);
        dir_rootless = rootless.substr(0, last_slash + 1);
    }
    else {
        // file stays empty
        dir_rootless = rootless;
    }
    dir_rootful = root + dir_rootless;

    // For file_no_exts, find the first dot in file
    file_no_exts = file;
    for (size_t i = 0; i < file.length(); ++i) {
        if (file[i] == '.') {
            file_no_exts = file.substr(0, i);
            break;
        }
    }
}



bool Filename::operator == (const Filename& rhs) const
{
    return (rootful == rhs.rootful);
}

bool Filename::operator != (const Filename& rhs) const
{
    return (rootful != rhs.rootful);
}



bool Filename::operator <  (const Filename& rhs) const
{
    // I roll my own here instead of using std::string::operator <, since
    // I use the convention throughout the program that file-less directory
    // names end with '/'. The directory "abc-def/" is therefore smaller than
    // "abc/", since '-' < '/' in ASCII, but we want lexicographical sorting
    // in the program's directory listings. Thus, this function here lets
    // '/' behave as being smaller than anything.
    const size_t la = rootful.length();
    const size_t lb = rhs.rootful.length();
    for (size_t i = 0; (i < la && i < lb); ++i) {
        const unsigned char& a = rootful[i];
        const unsigned char& b = rhs.rootful[i];
        if      (a == '/' && b == '/') continue;
        else if (a == '/') return true;
        else if (b == '/') return false;
        else if (a < b)    return true;
        else if (b < a)    return false;
    }
    // If we get here, one string is an initial segment of the other.
    // The shorter string shall be smaller.
    return la < lb;
}
