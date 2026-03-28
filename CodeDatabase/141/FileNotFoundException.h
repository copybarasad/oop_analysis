#ifndef FILE_NOT_FOUND_EXCEPTION_H
#define FILE_NOT_FOUND_EXCEPTION_H

#include "LoadException.h"
#include <string>

class FileNotFoundException : public LoadException {
public:
    explicit FileNotFoundException(const std::string &filename);
};

#endif
