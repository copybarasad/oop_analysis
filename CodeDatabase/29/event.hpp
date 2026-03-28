#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>

enum class EventType {
	Move,
	Damage,
	Kill,
	SpellGranted,
	TrapPlaced,
	SpellCast
};

struct Event {
	EventType type{EventType::Move};
	std::string actor;
	std::string target;
	int amount{0};
	int x{0};
	int y{0};
	std::string extra;
};

#endif
