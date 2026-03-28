#ifndef INVALID_SAVE_FILE_EXCEPTION_H
#define INVALID_SAVE_FILE_EXCEPTION_H

#include "LoadException.h"
#include <string>

class InvalidSaveFileException : public LoadException {
public:
    explicit InvalidSaveFileException(const std::string &reason);
};

#endif
