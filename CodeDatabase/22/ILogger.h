#pragma once
#include "event.h"
#include <memory>

class ILogger
{
public:
    virtual void log(const Event &event) = 0;
    virtual ~ILogger() = default;
};