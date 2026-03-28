//
// Created by Artem on 08.10.2025.
//

#ifndef LABA1_CHOOSEENEMYPRIORITYEXCEPTION_H
#define LABA1_CHOOSEENEMYPRIORITYEXCEPTION_H

#include <string>
#include "UniversalStringException.h"

class UnexpectedBehaviorException : public UniversalStringException{
public:
    explicit UnexpectedBehaviorException(const std::string &message) : UniversalStringException(message) {}
};


#endif //LABA1_CHOOSEENEMYPRIORITYEXCEPTION_H
