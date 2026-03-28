#pragma once
#include "logger/event.h"
#include "logger/ILogger.h"
#include <vector>
#include <memory>

class EventManager
{
    std::vector<std::shared_ptr<ILogger>> loggers;

public:
    void addLogger(std::shared_ptr<ILogger> logger);

    void notify(const Event &event);
};