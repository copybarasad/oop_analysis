#pragma once
#include <memory>
#include "GameEventBus.h"
#include "LogSink.h"

class GameLogger : public GameEventBus::Listener {
public:
    GameLogger(GameEventBus& bus, std::unique_ptr<LogSink> sink);
    ~GameLogger() override;

    void OnEvent(const GameEvent& event) override;

private:
    std::unique_ptr<LogSink> sink_;
    GameEventBus& bus_;
    std::string Format(const GameEvent& event) const;
};
