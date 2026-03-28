#pragma once
#include "GameEvent.h"
#include "EventLogger.h"
#include <vector>
#include <memory>
#include <functional>

class EventDispatcher {
public:
    static EventDispatcher& Instance();

    void SetLogger(std::unique_ptr<IEventLogger> logger);

    void Dispatch(const GameEvent& event);

    bool HasLogger() const;

private:
    EventDispatcher() = default;
    EventDispatcher(const EventDispatcher&) = delete;
    EventDispatcher& operator=(const EventDispatcher&) = delete;

    std::unique_ptr<IEventLogger> logger_;
};

#define DISPATCH_EVENT(event) EventDispatcher::Instance().Dispatch(event)
