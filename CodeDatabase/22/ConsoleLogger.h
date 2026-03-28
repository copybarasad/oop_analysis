#pragma once
#include "ILogger.h"
#include <iostream>

class ConsoleLogger : public ILogger
{
public:
    void log(const Event &event) override;
};