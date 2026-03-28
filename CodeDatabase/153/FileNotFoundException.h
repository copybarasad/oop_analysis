#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include "SaveLoadException.h"

class FileNotFoundException : public SaveLoadException {
public:
    explicit FileNotFoundException(const std::string& filename)
        : SaveLoadException("File not found: " + filename) {
    }
};

#endif