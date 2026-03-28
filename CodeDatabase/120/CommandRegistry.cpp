#include "CommandRegistry.h"

void CommandRegistry::RegisterCommand(char key, std::function<std::unique_ptr<ICommand>()> factory) {
	commandMap_[key] = std::move(factory);
}

std::unique_ptr<ICommand> CommandRegistry::FromChar(char key) const {
	auto it = commandMap_.find(key);
	if (it != commandMap_.end()) {
		return it->second();
	}
	return nullptr;
}
