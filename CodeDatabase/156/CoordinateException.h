//
// Created by Artem on 24.09.2025.
//

#ifndef LABA1_COORDINATEEXCEPTION_H
#define LABA1_COORDINATEEXCEPTION_H

#include <string>
#include "UniversalStringException.h"

class CoordinateException : public UniversalStringException{
public:
    explicit CoordinateException(const std::string &message) : UniversalStringException(message) {}
};

#endif //LABA1_COORDINATEEXCEPTION_H
