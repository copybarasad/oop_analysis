#include "managment/eventManager.h"

void EventManager::addLogger(std::shared_ptr<ILogger> logger)
{
    loggers.push_back(logger);
}

void EventManager::notify(const Event &event)
{
    for (auto &logger : loggers)
    {
        logger->log(event);
    }
}