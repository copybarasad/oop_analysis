#pragma once

#include <memory>

#include "EventBus.hpp"
#include "GameEventListener.hpp"
#include "LogSink.hpp"

class GameLogger : public GameEventListener {
public:
	GameLogger(EventBus& bus, std::shared_ptr<LogSink> sink);
	~GameLogger() override;

	void onEvent(const GameEvent& event) override;

private:
	EventBus& bus_;
	std::shared_ptr<LogSink> sink_;
};
