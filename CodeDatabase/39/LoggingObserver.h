#pragma once
#include "IGameObserver.h"
#include "ILogger.h"
#include <memory>

class LoggingObserver : public IGameObserver {
public:
    explicit LoggingObserver(std::shared_ptr<ILogger> logger);

    void OnGameEvent(const GameEvent& event) override;

private:
    std::shared_ptr<ILogger> logger_;
};
