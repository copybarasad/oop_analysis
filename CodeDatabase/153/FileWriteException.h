#ifndef FILEWRITEEXCEPTION_H
#define FILEWRITEEXCEPTION_H

#include "SaveLoadException.h"

class FileWriteException : public SaveLoadException {
public:
    explicit FileWriteException(const std::string& filename)
        : SaveLoadException("Cannot write to file: " + filename) {
    }
};

#endif