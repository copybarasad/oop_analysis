#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const std::string& filename)
    : SaveLoadException("File not found: " + filename) {}
