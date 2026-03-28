#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include "GameCommand.hpp"

class ControlScheme {
public:
	ControlScheme();

	void resetToDefaults();
	bool loadFromFile(const std::string& path);

	GameCommand::Type commandFor(int ch) const noexcept;
	int keyFor(GameCommand::Type type) const noexcept;

private:
	using Mapping = std::unordered_map<GameCommand::Type, int>;

	Mapping mapping_;

	static std::vector<GameCommand::Type> requiredCommands();
	static std::optional<GameCommand::Type> commandFromName(const std::string& name);
	static bool hasConflicts(const Mapping& mapping);
};
