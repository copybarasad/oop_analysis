#ifndef CORRUPTEDSAVEEXCEPTION_H
#define CORRUPTEDSAVEEXCEPTION_H

#include "LoadGameException.h"

class CorruptedSaveException : public LoadGameException {
public:
    explicit CorruptedSaveException(const std::string& msg) : LoadGameException(msg) {}
};

#endif