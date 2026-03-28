#include "ConsoleCommandSource.hpp"

#include <limits>
#include <sstream>
#include <string>

ConsoleCommandSource::ConsoleCommandSource(const ControlScheme& scheme,
                                           std::istream& input,
                                           std::ostream& output)
	: scheme_(scheme),
	  input_(input),
	  output_(output) {}

GameCommand ConsoleCommandSource::readCommand() {
	std::string line;
	while (true) {
		if (!std::getline(input_, line)) {
			return GameCommand::make(GameCommand::Type::ExitApplication);
		}
		if (line.empty()) {
			continue;
		}
		const int key = static_cast<unsigned char>(line.front());
		const GameCommand::Type type = scheme_.commandFor(key);
		if (type == GameCommand::Type::None) {
			output_ << "Unknown command.\n";
			continue;
		}
		if (requiresTarget(type)) {
			return readTargetedCommand(type);
		}
		return GameCommand::make(type);
	}
}

bool ConsoleCommandSource::requiresTarget(GameCommand::Type type) noexcept {
	return type == GameCommand::Type::CastSpell1 ||
	       type == GameCommand::Type::CastSpell2 ||
	       type == GameCommand::Type::CastSpell3;
}

GameCommand ConsoleCommandSource::readTargetedCommand(GameCommand::Type type) {
	while (true) {
		output_ << "Target cell (x y): ";
		std::string line;
		if (!std::getline(input_, line)) {
			return GameCommand::make(GameCommand::Type::ExitApplication);
		}
		std::istringstream iss(line);
		Position target{};
		if (iss >> target.x >> target.y) {
			return GameCommand::makeTargeted(type, target);
		}
		output_ << "Invalid coordinates.\n";
	}
}
