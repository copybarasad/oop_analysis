#include "GameLogger.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace {
std::string timestamp() {
	auto now = std::chrono::system_clock::now();
	std::time_t tt = std::chrono::system_clock::to_time_t(now);
	std::tm tm{};
#if defined(_WIN32)
	localtime_s(&tm, &tt);
#else
	localtime_r(&tt, &tm);
#endif
	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
	return oss.str();
}
} // namespace

GameLogger::GameLogger(EventBus& bus, std::shared_ptr<LogSink> sink)
	: bus_(bus),
	  sink_(std::move(sink)) {
	bus_.subscribe(*this);
}

GameLogger::~GameLogger() {
	bus_.unsubscribe(*this);
}

void GameLogger::onEvent(const GameEvent& event) {
	if (!sink_) {
		return;
	}
	if (event.type() == GameEvent::Type::StateChanged) {
		return;
	}
	std::ostringstream oss;
	oss << "[" << timestamp() << "] ";
	if (!event.message().empty()) {
		oss << event.message();
	} else {
		oss << "Event " << static_cast<int>(event.type());
	}
	sink_->write(oss.str());
}
