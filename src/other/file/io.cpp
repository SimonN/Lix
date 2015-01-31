/*
 * other/io.cpp
 *
 */

#include "../myalleg.h" // exists
#include <sstream>

#include "io.h"

namespace IO {

Line::Line(const std::string& src)
:
    type(0),
    nr1(0),
    nr2(0),
    nr3(0)
{
    std::string::const_iterator i = src.begin();

    // UTF-8 BOM handling
    //     [see http://en.wikipedia.org/wiki/Byte_order_mark
    //      for explanation of BOM]
    // Certain text editors (Window's Notepad) may add the
    // Unicode BOM character at start of text file to indicate
    // which format the text file is encoded with (eg. ANSI vs
    // UTF-8 vs UTF-16LE etc.).  Since some of Lix's input
    // files (particularly translate.txt) are expected to be
    // UTF-8, we should accomodate this behavior and skip over
    // a UTF-8 BOM character at start of line.
    // (Usually it's just at start of file, but no big deal
    // doing it on every line instead.)

    int firstCharUtf8 = ugetat(src.c_str(), 0);
    if (firstCharUtf8 == 0xFEFF) {
        int numBytes = ucwidth(firstCharUtf8);
        if ((src.end() - src.begin()) >= numBytes) {
            i += numBytes;
        }
    }

    if (i != src.end()) type = *i++;

    bool minus1 = false;
    bool minus2 = false;
    bool minus3 = false;

    switch (type) {
    case '$':
        while (i != src.end() && *i != ' ') text1 += *i++;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end()             ) text2 += *i++;
        break;

    case '#':
        while (i != src.end() && *i != ' ') text1 += *i++;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr1, minus1);
        break;

    case ':':
        while (i != src.end() && *i != ':') text1 += *i++;
        while (i != src.end() && *i == ':') ++i;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr1, minus1);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr2, minus2);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end()             ) text2 += *i++;
        break;

    case '+':
        while (i != src.end() && *i != ' ') text1 += *i++;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr1, minus1);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') text2 += *i++;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end()             ) text3 += *i++;
        break;

    case '!':
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr1, minus1);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr2, minus2);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') text1 += *i++;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr3, minus3);
        break;

    case '<':
        while (i != src.end() && *i != '>') text1 += *i++;
        while (i != src.end() && *i == '>') ++i;
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr1, minus1);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr2, minus2);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end() && *i != ' ') iter_to_long(i, nr3, minus3);
        while (i != src.end() && *i == ' ') ++i;
        while (i != src.end()             ) text2 += *i++;
        break;


    default:
        // Zeile als ungueltig markieren
        type = 0;
        break;
    }

    if (minus1) nr1 *= -1;
    if (minus2) nr2 *= -1;
    if (minus3) nr3 *= -1;
}

void Line::iter_to_long(std::string::const_iterator& i, long& nr, bool& minus)
{
    if (*i == '-') minus = true;
    else {
        nr *= 10;
        nr += *i - '0';
    }
    ++i;
}



// Funtionen, die Zeilen zum Schreiben erzeugen und auf den privaten
// Konstruktor (siehe weiter unten) zurueckgreifen
typedef const std::string& Str;

Line LineHash  (const Str t1, const long n1)
{
    return Line('#', t1, "", "", n1,  0,  0);
}

Line LineColon (const Str t1,  const long n1,
                const long n2, const Str t2)
{
    return Line(':', t1, t2, "", n1, n2,  0);
}

Line LineDollar(const Str t1, const Str t2)
{
    return Line('$', t1, t2, "",  0,  0,  0);
}

Line LineDollar(const Str t1, const Date& d)
{
    std::string t2;
    t2 << d;
    return Line('$', t1, t2, "",  0,  0,  0);
}

Line LinePlus  (const Str t1, const long n1,
                const Str t2, const Str  t3)
{
    return Line('+', t1, t2, t3, n1,  0,  0);
}


Line LineBang  (const long n1, const long n2,
                const Str  t1, const long n3)
{
    return Line('!', t1, "", "", n1, n2, n3);
}

Line LineAngle (const Str t1,  const long n1,
                const long n2, const long n3, const Str t2)
{
    return Line('<', t1, t2, "", n1, n2, n3);
}



// Privater Konstruktor

Line::Line(const char c, const Str  s1, const Str  s2, const Str  s3,
                         const long i1, const long i2, const long i3)
:   type (c),
    text1(s1),
    text2(s2),
    text3(s3),
    nr1  (i1),
    nr2  (i2),
    nr3  (i3)
{
}





// Mindestens ein Leerzeichen, anschliessend auffuellen, damit es schoen ist
static inline void pad(std::ostringstream& o, int i)
{
    // don't pad with spaces just to align, instead save space. The raw format
    // won't enter a beauty contest.
    /*
    do {
        o << (char) ' ';
    } while ((int) o.str().size() < i);
    */
    o << ' ';
    i = i; // against unused variable error
}



// Anzahl Ziffern in der Zahl, inklusive Minuszeichen
static int digits(long nr)
{
    int ret = (nr <= 0);
    while (nr) {
        nr /= 10;
        ++ret;
    }
    return ret;
}



std::ostream& operator << (std::ostream& original_stream, const Line& ld)
{
    std::ostringstream o;
    o << ld.type;

    switch (ld.type) {
    case '$':
        o << ld.text1; pad(o, 70 - ld.text2.size());
        o << ld.text2;
        break;

    case '#':
        o << ld.text1; pad(o, 70 - digits(ld.nr1));
        o << ld.nr1;
        break;

    case ':':
        o << ld.text1;
        o << ':';      pad(o, 60 - digits(ld.nr1));
        o << ld.nr1;   pad(o, 70 - digits(ld.nr2));
        o << ld.nr2;
        if (!ld.text2.empty()) {
            pad(o, 75);
            o << ld.text2;
        }
        break;

    case '+':
        o << ld.text1; pad(o, 20 - digits(ld.nr2));
        o << ld.nr1;
        o << ' ';
        o << ld.text2; pad(o, 70 - ld.text3.size());
        o << ld.text3;
        break;

    case '!':          pad(o, 10 - digits(ld.nr1));
        o << ld.nr1;   pad(o, 20 - digits(ld.nr2));
        o << ld.nr2;
        o << ' ';
        o << ld.text1; pad(o, 70 - digits(ld.nr3));
        o << ld.nr3;
        break;

    case '<':
        o << ld.text1;
        o << '>';      pad(o, 60 - digits(ld.nr1));
        o << ld.nr1;   pad(o, 65 - digits(ld.nr2));
        o << ld.nr2;   pad(o, 72 - digits(ld.nr3));
        o << ld.nr3;   pad(o, 95 - ld.text2.size());
        o << ld.text2;
        break;

    default:
        break;
    }
    return original_stream << o.str() << std::endl;
}



bool fill_vector_from_file(std::vector <Line>& v, const std::string& filename)
{
    if ( ! ::exists(filename.c_str())) return false;
    std::ifstream file(filename.c_str());
    bool was_good = fill_vector_from_stream(v, file);
    file.close();
    return was_good; // even if empty file
}



bool fill_vector_from_stream(std::vector <Line>& v, std::istream& in)
{
    if (! in.good()) return false;
    std::string s;
    char        c;
    // Don't use in >> c since we want unformatted input to process newlines
    while (in.get(c)) {
        // Read characters into string until CR or LF are encountered
        if (c != '\n' && c != '\r') s += c;
        else if (! s.empty()) {
            v.push_back(Line(s));
            s.clear();
        }
    }
    // No newline at end of file shall not matter
    if (! s.empty()) v.push_back(Line(s));
    // Return true even if empty, but if the stream was good to begin with
    return true;
}




// This isn't just an exact copy of the function above, with just the string
// instead of Line(string) pushed back. This actually cares about empty lines,
// and is still compatible with any line ending.
bool fill_vector_from_file_raw(
    std::vector <std::string>& v,
    const std::string&         filename
) {
    std::ifstream file(filename.c_str());
    if (! file.good()) {
        file.close();
        return false;
    }
    std::string s;
    char        c;
    while (file.get(c)) {
        if (c != '\n' && c != '\r') s += c;
        else {
            // do this even if the string is currently empty
            v.push_back(s);
            s.clear();
            const char next = file.peek();
            if ((c == '\n' && next == '\r') || (c == '\r' && next == '\n')) {
                // discard the second line ending character
                file.get(c);
            }
        }
    }
    if (! s.empty()) v.push_back(s);
    file.close();
    return true;
}

}
// Ende Namensraum IO
