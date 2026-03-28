#ifndef LOAD_EXCEPTION_H
#define LOAD_EXCEPTION_H

#include "SaveLoadException.h"

class LoadException : public SaveLoadException {
public:
    explicit LoadException(const std::string& message): SaveLoadException(message) {}
};

#endif