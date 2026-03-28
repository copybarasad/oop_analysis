#include "ControlConfig.h"
#include <algorithm>
#include <sstream>
#include <cctype>
#include <vector>

ControlConfig::ControlConfig() : isValid(false) {
	initializeDefaults();
}

void ControlConfig::initializeDefaults() {
	// Очищаем текущие маппинги
	keyToCommand.clear();
	commandToKey.clear();
	
	// Устанавливаем значения по умолчанию
	setKeyForCommand('w', CommandType::MOVE_UP);
	setKeyForCommand('s', CommandType::MOVE_DOWN);
	setKeyForCommand('a', CommandType::MOVE_LEFT);
	setKeyForCommand('d', CommandType::MOVE_RIGHT);
	setKeyForCommand('f', CommandType::ATTACK);
	setKeyForCommand('c', CommandType::CAST_SPELL);
	setKeyForCommand('h', CommandType::SHOW_HAND);
	setKeyForCommand('q', CommandType::SWITCH_MODE);
	setKeyForCommand('v', CommandType::SAVE);
	setKeyForCommand('z', CommandType::LOAD);
	setKeyForCommand('x', CommandType::EXIT);
	
	isValid = true;
	errorMessage = "";
}

void ControlConfig::setKeyForCommand(char key, CommandType command) {
	// Приводим клавишу к нижнему регистру
	key = std::tolower(key);
	
	// Удаляем старый маппинг для этой клавиши, если он есть
	auto it = keyToCommand.find(key);
	if (it != keyToCommand.end()) {
		commandToKey.erase(it->second);
	}
	
	// Удаляем старый маппинг для этой команды, если он есть
	auto it2 = commandToKey.find(command);
	if (it2 != commandToKey.end()) {
		keyToCommand.erase(it2->second);
	}
	
	// Устанавливаем новый маппинг
	keyToCommand[key] = command;
	commandToKey[command] = key;
}

ControlConfig::CommandType ControlConfig::getCommandForKey(char key) const {
	key = std::tolower(key);
	auto it = keyToCommand.find(key);
	if (it != keyToCommand.end()) {
		return it->second;
	}
	// Возвращаем EXIT как значение по умолчанию для неизвестных клавиш
	return CommandType::EXIT;
}

char ControlConfig::getKeyForCommand(CommandType command) const {
	auto it = commandToKey.find(command);
	if (it != commandToKey.end()) {
		return it->second;
	}
	return '\0'; // Возвращаем нулевой символ, если команда не найдена
}

bool ControlConfig::isKeyMapped(char key) const {
	key = std::tolower(key);
	return keyToCommand.find(key) != keyToCommand.end();
}

bool ControlConfig::isCommandMapped(CommandType command) const {
	return commandToKey.find(command) != commandToKey.end();
}

bool ControlConfig::validate() {
	errorMessage = "";
	
	// Получаем все обязательные команды
	auto requiredCommands = getRequiredCommands();
	
	// Проверка 1: Все обязательные команды должны быть назначены
	std::vector<CommandType> missingCommands;
	for (const auto& cmd : requiredCommands) {
		if (!isCommandMapped(cmd)) {
			missingCommands.push_back(cmd);
		}
	}
	
	if (!missingCommands.empty()) {
		std::ostringstream oss;
		oss << "Missing key bindings for commands: ";
		for (size_t i = 0; i < missingCommands.size(); ++i) {
			if (i > 0) oss << ", ";
			oss << commandTypeToString(missingCommands[i]);
		}
		errorMessage = oss.str();
		isValid = false;
		return false;
	}
	
	// Проверка 2: Одна клавиша не должна быть назначена нескольким командам
	// (это уже проверяется в setKeyForCommand, но проверим еще раз)
	std::map<char, int> keyCount;
	for (const auto& pair : keyToCommand) {
		keyCount[pair.first]++;
		if (keyCount[pair.first] > 1) {
			std::ostringstream oss;
			oss << "Key '" << pair.first << "' is mapped to multiple commands";
			errorMessage = oss.str();
			isValid = false;
			return false;
		}
	}
	
	// Проверка 3: Одна команда не должна быть назначена нескольким клавишам
	// (это уже проверяется в setKeyForCommand, но проверим еще раз)
	std::map<CommandType, int> commandCount;
	for (const auto& pair : commandToKey) {
		commandCount[pair.first]++;
		if (commandCount[pair.first] > 1) {
			std::ostringstream oss;
			oss << "Command " << commandTypeToString(pair.first) << " is mapped to multiple keys";
			errorMessage = oss.str();
			isValid = false;
			return false;
		}
	}
	
	// Все проверки пройдены
	isValid = true;
	errorMessage = "";
	return true;
}

bool ControlConfig::getIsValid() const {
	return isValid;
}

const std::string& ControlConfig::getErrorMessage() const {
	return errorMessage;
}

std::unordered_set<ControlConfig::CommandType> ControlConfig::getRequiredCommands() {
	return {
		CommandType::MOVE_UP,
		CommandType::MOVE_DOWN,
		CommandType::MOVE_LEFT,
		CommandType::MOVE_RIGHT,
		CommandType::ATTACK,
		CommandType::CAST_SPELL,
		CommandType::SHOW_HAND,
		CommandType::SWITCH_MODE,
		CommandType::SAVE,
		CommandType::LOAD,
		CommandType::EXIT
	};
}

std::string ControlConfig::commandTypeToString(CommandType type) {
	switch (type) {
		case CommandType::MOVE_UP: return "MOVE_UP";
		case CommandType::MOVE_DOWN: return "MOVE_DOWN";
		case CommandType::MOVE_LEFT: return "MOVE_LEFT";
		case CommandType::MOVE_RIGHT: return "MOVE_RIGHT";
		case CommandType::ATTACK: return "ATTACK";
		case CommandType::CAST_SPELL: return "CAST_SPELL";
		case CommandType::SHOW_HAND: return "SHOW_HAND";
		case CommandType::SWITCH_MODE: return "SWITCH_MODE";
		case CommandType::SAVE: return "SAVE";
		case CommandType::LOAD: return "LOAD";
		case CommandType::EXIT: return "EXIT";
		default: return "UNKNOWN";
	}
}

ControlConfig::CommandType ControlConfig::stringToCommandType(const std::string& str) {
	std::string upperStr = str;
	std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
	
	if (upperStr == "MOVE_UP" || upperStr == "UP") return CommandType::MOVE_UP;
	if (upperStr == "MOVE_DOWN" || upperStr == "DOWN") return CommandType::MOVE_DOWN;
	if (upperStr == "MOVE_LEFT" || upperStr == "LEFT") return CommandType::MOVE_LEFT;
	if (upperStr == "MOVE_RIGHT" || upperStr == "RIGHT") return CommandType::MOVE_RIGHT;
	if (upperStr == "ATTACK") return CommandType::ATTACK;
	if (upperStr == "CAST_SPELL" || upperStr == "SPELL") return CommandType::CAST_SPELL;
	if (upperStr == "SHOW_HAND" || upperStr == "HAND") return CommandType::SHOW_HAND;
	if (upperStr == "SWITCH_MODE" || upperStr == "MODE") return CommandType::SWITCH_MODE;
	if (upperStr == "SAVE") return CommandType::SAVE;
	if (upperStr == "LOAD") return CommandType::LOAD;
	if (upperStr == "EXIT" || upperStr == "QUIT") return CommandType::EXIT;
	
	// Значение по умолчанию
	return CommandType::EXIT;
}

