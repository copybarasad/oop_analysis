//
// Created by Artem on 05.10.2025.
//

#ifndef LABA1_SPAWNENTITYEXCEPTION_H
#define LABA1_SPAWNENTITYEXCEPTION_H

#include <string>
#include "UniversalStringException.h"

class SpawnEntityException : public UniversalStringException{
public:
    explicit SpawnEntityException(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABA1_SPAWNENTITYEXCEPTION_H
