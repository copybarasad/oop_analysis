#pragma once
#include "SaveFileRAII.h"


class Serializable {
public:
    virtual void saveTo(SaveFileRAII& out) const = 0;
    virtual void loadFrom(SaveFileRAII& in) = 0;
    virtual ~Serializable() = default;
};