#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>

enum class UserCommandType {
	MoveUp,
	MoveDown,
	MoveLeft,
	MoveRight,
	AttackUp,
	AttackDown,
	AttackLeft,
	AttackRight,
	ToggleMode,
	Cast,
	Save,
	Load,
	Exit,
	Unknown
};

struct UserCommand {
	UserCommandType type{UserCommandType::Unknown};
	int slot{0};
	int targetX{0};
	int targetY{0};
	std::string raw;
	bool HasTarget() const noexcept {
		return type == UserCommandType::Cast;
	}
};

#endif
