#ifndef BINARY_FILE_H
#define BINARY_FILE_H

#include <fstream>
#include "game_exceptions.h"

class BinaryFile {
    std::fstream file;
    std::string filename;
public:
    BinaryFile(const std::string& name, std::ios::openmode mode)
        : filename(name) {
        file.open(name, std::ios::binary | mode);
        if (!file.is_open()) {
            throw FileOpenException(name, "Check permissions or disk space");
        }
    }
    ~BinaryFile() { if (file.is_open()) file.close(); }

    std::fstream& stream() { return file; }
    const std::string& getFilename() const { return filename; }
};

#endif


