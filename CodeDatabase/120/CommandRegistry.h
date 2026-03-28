#pragma once
#include "ICommand.h"
#include <functional>

class CommandRegistry {
public:
	CommandRegistry() = default;

	void RegisterCommand(char key, std::function<std::unique_ptr<ICommand>()> factory);
	std::unique_ptr<ICommand> FromChar(char key) const;

private:
	std::unordered_map<char, std::function<std::unique_ptr<ICommand>()>> commandMap_;
};
