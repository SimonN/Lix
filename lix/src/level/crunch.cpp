/*
 * level/crunch.cpp
 *
 * Kompression wurde von ccexplore beschrieben unter:
 *
 *   http://camanis.net/lemmings/files/docs/lix_dat_file_format.txt
 *
 * Danke, ohne diese Info waer es nie gegangen. :-)
 *
 */

#include "crunch.h"

Crunch Crunch::cr;



int Crunch::read_byte(std::ifstream& file)
{
    if (!file.good()) return 0;
    unsigned char c;
    file.read((char*) &c, 1);
    return c;
}

int Crunch::read_word(std::ifstream& file)
{
    unsigned char c1 = read_byte(file);
    unsigned char c2 = read_byte(file);
    // Big Endian
    return 256 * c1 + c2;
}



void Crunch::save_section(const Section& section, const std::string& filename)
{
    std::ofstream file(filename.c_str(), std::ios::binary);
    for (Section::const_iterator
     itr = section.begin(); itr != section.end(); ++itr)
     file.write((const char*) &*itr, 1);
    file.close();
}



const Crunch::File* Crunch::get_file(const std::string& filename)
{
    // Wird aehnlich gemacht wie in ObjLib::get()
    const std::map <std::string, File> ::const_iterator
     itr = cr.data.find(filename);
    if (itr != cr.data.end()) return &itr->second;

    // Wenn wir hier ankommen, dann gibt es noch keine fertig entcrunchte
    // Datei mit dem Namen. Also eine suchen und, falls Suche erfolgreich,
    // entcrunchen.
    std::ifstream file(filename.c_str(), std::ios::binary);
    if (!file.good()) return 0;

    File crunchfile; // Dies kommt in die Map, wenn es fertig ist
    int  start_of_next_header = 0;

    // In jedem Schleifendurchlauf wird eine Sektion entcruncht.
    while (true) {
        file.seekg(start_of_next_header);

        // Header auslesen
        int num_bits_in_first_byte = read_byte(file);
        Byte header_checksum       = read_byte(file);
                                     read_word(file); // wegwerfen
        int decompressed_data_size = read_word(file);
                                     read_word(file); // wegwerfen
        int compressed_data_size   = read_word(file);

        // Dateiende
        if (!file.good()) break;

        // Dateiende nicht erreicht
        start_of_next_header += compressed_data_size;

        std::vector <bool> bitstream;
        Byte               bitstream_checksum = 0;

        for (int pos = start_of_next_header - 1;
         pos >= start_of_next_header - compressed_data_size + 10; --pos) {
            file.seekg(pos);
            Byte byte = read_byte(file);
            bitstream_checksum ^= byte;
            for (int bit = 0; bit < 8; ++bit) {
                // Sonderfall im ersten Bit
                if (pos == start_of_next_header - 1
                 && bit >= num_bits_in_first_byte) break;
                bitstream.push_back((int) byte % (1 << bit+1) / (1 << bit));
            }
        }
        // Sektion korrumpiert?
        if (bitstream_checksum != header_checksum) continue;

        // Bitstrom ist nun fertig hergestellt.
        // In der folgenden Section-Variable wird das Ergebnis gespeichert.
        // Der Bitstrom kodiert die Sektion von hinten nach vorne; ich werde
        // alles von vorne nach hinten in die Sektion schreiben und am Ende
        // section.reverse() aufrufen.
        Section decoded;

        std::vector <bool> ::const_iterator itr = bitstream.begin();

        while ((int) decoded.size() < decompressed_data_size) {
            // Wir machen hier keine Kontrollen, ob ++itr eventuell ueber
            // den Rand hinausgeht. Wir gehen davon aus, dass das nicht
            // passiert, da wir noch nicht genug Bytes ausgelesen haben.
            // Die Algorithmen sind durchnummeriert wie in der Datei,
            // die im einleitenden Kommentar dieser Datei angegeben ist.
            int algo = 0;
            if      (*itr == 0 && *(itr+1) == 0)                  algo = 1;
            else if (*itr == 0 && *(itr+1) == 1)                  algo = 2;
            else if (*itr == 1 && *(itr+1) == 0 && *(itr+2) == 0) algo = 3;
            else if (*itr == 1 && *(itr+1) == 0 && *(itr+2) == 1) algo = 4;
            else if (*itr == 1 && *(itr+1) == 1 && *(itr+2) == 0) algo = 5;
            else if (*itr == 1 && *(itr+1) == 1 && *(itr+2) == 1) algo = 6;

            if (algo == 1 || algo == 2) itr += 2;
            else                        itr += 3;

            if (algo == 1 || algo == 6) {
                // Klartext-Bytes. Drei Bits, die sagen, wie viele Bytes
                // kommen. Es kommt genau ein Byte mehr, als die Bits angeben,
                // also beginnt length von vornherein mit 1.
                // Im Falle von algo == 6 sind es acht statt drei Bits,
                // und es muss 9 statt 1 addiert werden.
                const int bits_for_length = (algo == 1 ? 3 : 8);
                int       length          = (algo == 1 ? 1 : 9);

                for (int i = bits_for_length - 1; i >= 0; --i) {
                    length += *itr << i;
                    ++itr;
                }
                for (int byte = 0; byte < length; ++byte) {
                    Byte result = 0;
                    for (int i = 7; i >= 0; --i) {
                        result += *itr << i;
                        ++itr;
                    }
                    decoded.push_back(result);
                }
            }
            else if (algo >= 2 && algo <= 5) {
                // Wiederholung von einigen Bytes, die geschrieben wurden.
                // Der Offset ist eine von algo abhaengige Anzahl Bits, zu
                // der noch 1 addiert werden muss, um den Abstand vom Ende
                // des noch nicht .reverse()-ten Vektor <Byte> zu finden.
                const int bits_for_offset = algo == 2 ?  8
                                          : algo == 3 ?  9
                                          : algo == 4 ? 10 : 12;
                int       bytes_to_repeat = algo == 2 ?  2
                                          : algo == 3 ?  3
                                          : algo == 4 ?  4 :  1; // einlesen
                if (algo == 5) {
                    // 8 Bytes geben die Laenge der Wiederholung an, plus 1
                    bytes_to_repeat = 1;
                    for (int i = 7; i >= 0; --i) {
                        bytes_to_repeat += *itr << i;
                        ++itr;
                    }
                }
                int offset = 1;
                for (int i = bits_for_offset - 1; i >= 0; --i) {
                    offset += *itr << i;
                    ++itr;
                }
                // Nun die Bytes kopieren
                for (int i = 0; i < bytes_to_repeat; ++i)
                 decoded.push_back(*(decoded.end() - offset));
            }
            // Unterscheidung des Algorithmus fuer ein paar Bytes ist fertig.
            // Wieder zum Anfang dieser Schleife, um mehr Bytes der aktuellen
            // Sektion zu entcrunchen.
        }
        // Es wurden hinreichend viele Bytes ausgelesen und die Sektion ist so
        // lang wie noetig. Jetzt Reihenfolge umdrehen.
        Section section;
        for (Section::const_iterator itr =  --decoded.end();
                                     itr != --decoded.begin(); --itr)
         section.push_back(*itr);
        crunchfile.push_back(section);
    }
    // Es wurden alle Sektionen ausgelesen.

    file.close();

    if (!crunchfile.empty()) {
        cr.data.insert(std::pair <std::string, File> (filename, crunchfile));
        return &cr.data[filename];
    }
    else return 0;

}
