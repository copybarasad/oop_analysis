#ifndef INVALID_SAVE_DATA_EXCEPTION_H
#define INVALID_SAVE_DATA_EXCEPTION_H

#include "LoadException.h"

class InvalidSaveDataException : public LoadException {
public:
    explicit InvalidSaveDataException(const std::string& message): LoadException(message) {}
};

#endif