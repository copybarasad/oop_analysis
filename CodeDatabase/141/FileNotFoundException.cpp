#include "FileNotFoundException.h"

FileNotFoundException::FileNotFoundException(const std::string &filename)
    : LoadException("File not found: " + filename) {
}
