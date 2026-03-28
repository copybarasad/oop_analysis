#include "file.hpp"

saveFile::saveFile(std::string& path) {
        file.open(path);
        if (!file.is_open())
            throw GameException("Cannot open file: " + path);
    }

std::ifstream& saveFile::get() { 
    return file;
}