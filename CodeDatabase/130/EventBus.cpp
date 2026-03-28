#include "EventBus.h"

void EventBus::subscribe(const std::shared_ptr<ILogger>& logger) {
    if (logger) {
        loggers_.push_back(logger);
    }
}

void EventBus::publish(const GameEvent& event) {
    for (auto& logger : loggers_) {
        if (logger) {
            logger->log(event);
        }
    }
}


