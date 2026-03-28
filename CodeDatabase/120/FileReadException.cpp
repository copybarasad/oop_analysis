#include "FileReadException.h"

FileReadException::FileReadException(const std::string& filename)
    : SaveLoadException("Cannot read file: " + filename) {}
