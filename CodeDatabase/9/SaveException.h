#ifndef SAVE_EXCEPTION_H
#define SAVE_EXCEPTION_H

#include "SaveLoadException.h"

class SaveException : public SaveLoadException {
public:
    explicit SaveException(const std::string& message): SaveLoadException(message) {}
};

#endif