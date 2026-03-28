#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include "GameEvent.h"
#include "Logger.h"
#include <vector>
#include <memory>

class EventBus {
public:
    void subscribe(const std::shared_ptr<ILogger>& logger);
    void publish(const GameEvent& event);

private:
    std::vector<std::shared_ptr<ILogger>> loggers_;
};

#endif


