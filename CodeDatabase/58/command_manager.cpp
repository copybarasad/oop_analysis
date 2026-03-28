#include "command_manager.hpp"

CommandManager::CommandManager(CommandType type) : type(type) {}

CommandType CommandManager::get_type() {return type;};