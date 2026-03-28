#include "ControlConfig.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <iostream>
#include <filesystem>

ControlConfig::ControlConfig() {
	setDefaultBindings();
}

bool ControlConfig::loadFromFile(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Не удалось открыть файл настроек: " << filename << "\n";
		return false; // Файл не найден, настройки по умолчанию
	}
	
	std::map<char, std::string> tempKeyToCommand;
	std::map<std::string, char> tempCommandToKey;
	bool hasError = false;
	
	std::string line;
	while (std::getline(file, line)) {
		// Удаляем пробелы в начале и конце
		size_t first = line.find_first_not_of(" \t\r\n");
		if (first == std::string::npos) {
			continue; // Пустая строка
		}
		size_t last = line.find_last_not_of(" \t\r\n");
		line = line.substr(first, last - first + 1);
		
		if (line.empty() || line[0] == '#') {
			continue;
		}
		
		std::istringstream iss(line);
		std::string command, keyStr;
		if (iss >> command >> keyStr) {
			if (keyStr.length() != 1) {
				std::cerr << "Ошибка: клавиша должна быть одним символом в строке: " << line << "\n";
				hasError = true;
				continue;
			}
			
			char key = keyStr[0];
			
			// Проверка на дублирование клавиш
			if (tempKeyToCommand.find(key) != tempKeyToCommand.end()) {
				if (tempKeyToCommand[key] != command) {
					std::cerr << "Ошибка: клавиша '" << key << "' уже назначена команде '" 
						<< tempKeyToCommand[key] << "'\n";
					hasError = true;
					continue;
				} 
				continue;
			}
			
			// Проверка на дублирование команд
			if (tempCommandToKey.find(command) != tempCommandToKey.end()) {
				if (tempCommandToKey[command] != key) {
					std::cerr << "Ошибка: команда '" << command << "' уже назначена клавише '" 
						<< tempCommandToKey[command] << "'\n";
					hasError = true;
					continue;
				}
				continue;
			}
			
			tempKeyToCommand[key] = command;
			tempCommandToKey[command] = key;
		}
	}
	
	file.close();
	
	std::vector<std::string> requiredCommands = {
		"move_up", "move_down", "move_left", "move_right",
		"switch_melee", "switch_ranged"
	};
	
	std::vector<std::string> missingCommands;
	for (const auto& cmd : requiredCommands) {
		if (tempCommandToKey.find(cmd) == tempCommandToKey.end()) {
			missingCommands.push_back(cmd);
			hasError = true;
		}
	}
	
	if (!missingCommands.empty()) {
		std::cerr << "Ошибка: отсутствуют обязательные команды: ";
		for (size_t i = 0; i < missingCommands.size(); ++i) {
			std::cerr << missingCommands[i];
			if (i < missingCommands.size() - 1) std::cerr << ", ";
		}
		std::cerr << "\n";
	}
	
	if (hasError) {
		std::cerr << "Ошибки в файле настроек. Используются настройки по умолчанию.\n";
		return false; 
	}
	
	keyToCommand_ = std::move(tempKeyToCommand);
	commandToKey_ = std::move(tempCommandToKey);
	std::cout << "Настройки управления успешно загружены из файла: " << filename << "\n";
	return true;
}

std::string ControlConfig::getCommandForKey(char key) const {
	auto it = keyToCommand_.find(key);
	if (it != keyToCommand_.end()) {
		return it->second;
	}
	return "";
}

char ControlConfig::getKeyForCommand(const std::string& command) const {
	auto it = commandToKey_.find(command);
	if (it != commandToKey_.end()) {
		return it->second;
	}
	return '\0';
}

std::map<std::string, char> ControlConfig::getAllBindings() const {
	return commandToKey_;
}

void ControlConfig::setDefaultBindings() {
	keyToCommand_.clear();
	commandToKey_.clear();
	
	// Движение
	keyToCommand_['w'] = "move_up";
	keyToCommand_['s'] = "move_down";
	keyToCommand_['a'] = "move_left";
	keyToCommand_['d'] = "move_right";
	
	// Переключение режима
	keyToCommand_['m'] = "switch_melee";
	keyToCommand_['r'] = "switch_ranged";
	
	// Другие команды
	keyToCommand_['u'] = "use_spell";
	keyToCommand_['b'] = "buy_spell";
	keyToCommand_['q'] = "menu";
	keyToCommand_['f'] = "attack";
	
	// Обратный маппинг
	for (const auto& pair : keyToCommand_) {
		commandToKey_[pair.second] = pair.first;
	}
}

bool ControlConfig::validateConfig() const {
	// Проверка на наличие всех необходимых команд
	std::vector<std::string> requiredCommands = {
		"move_up", "move_down", "move_left", "move_right",
		"switch_melee", "switch_ranged"
	};
	
	for (const auto& cmd : requiredCommands) {
		if (commandToKey_.find(cmd) == commandToKey_.end()) {
			return false;
		}
	}
	return true;
}

