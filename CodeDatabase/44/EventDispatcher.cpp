#include "EventDispatcher.h"

EventDispatcher& EventDispatcher::Instance() {
    static EventDispatcher instance;
    return instance;
}

void EventDispatcher::SetLogger(std::unique_ptr<IEventLogger> logger) {
    logger_ = std::move(logger);
}

void EventDispatcher::Dispatch(const GameEvent& event) {
    if (logger_) {
        logger_->Log(event);
    }
}

bool EventDispatcher::HasLogger() const {
    return logger_ != nullptr;
}
