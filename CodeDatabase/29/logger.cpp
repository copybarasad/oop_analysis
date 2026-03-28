#include "logger.hpp"

namespace {
std::string EventToString(const Event& ev) {
	switch (ev.type) {
		case EventType::Move:
			return ev.actor + " moved to (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ")";
		case EventType::Damage:
			return ev.actor + " deal " + std::to_string(ev.amount) + " dmg to " + ev.target + " at (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ")";
		case EventType::Kill:
			return ev.actor + " killed " + ev.target + " at (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ")";
		case EventType::SpellGranted:
			return ev.actor + " received spell " + ev.extra;
		case EventType::TrapPlaced:
			return ev.actor + " placed trap at (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ") dmg=" + std::to_string(ev.amount);
		case EventType::SpellCast:
			return ev.actor + " cast " + ev.extra + " at (" + std::to_string(ev.x) + "," + std::to_string(ev.y) + ")";
	}
	return "";
}
}

void StdoutEventObserver::Log(const Event& ev) {
	std::cout << "[LOG] " << EventToString(ev) << "\n";
}

FileEventObserver::FileEventObserver(const std::string& path) : out_(path, std::ios::trunc) {}

void FileEventObserver::Log(const Event& ev) {
	if (!out_) return;
	out_ << EventToString(ev) << "\n";
	out_.flush();
}
