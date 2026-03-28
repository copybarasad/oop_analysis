#include "FileWriteException.h"

FileWriteException::FileWriteException(const std::string& filename)
    : SaveLoadException("Cannot write to file: " + filename) {}
