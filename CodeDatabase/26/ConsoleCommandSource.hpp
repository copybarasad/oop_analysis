#pragma once

#include <istream>
#include <ostream>

#include "ControlScheme.hpp"
#include "GameCommand.hpp"

class ConsoleCommandSource {
public:
	ConsoleCommandSource(const ControlScheme& scheme,
	                     std::istream& input,
	                     std::ostream& output);

	GameCommand readCommand();

private:
	const ControlScheme& scheme_;
	std::istream& input_;
	std::ostream& output_;

	static bool requiresTarget(GameCommand::Type type) noexcept;
	GameCommand readTargetedCommand(GameCommand::Type type);
};
