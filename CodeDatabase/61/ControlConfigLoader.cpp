#include "ControlConfigLoader.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

bool ControlConfigLoader::parseLine(const std::string& line, std::string& commandStr, char& key) {
	std::string cleaned = cleanLine(line);
	if (cleaned.empty()) {
		return false;
	}
	
	// Ищем знак равенства
	size_t eqPos = cleaned.find('=');
	if (eqPos == std::string::npos) {
		return false;
	}
	
	std::string left = cleaned.substr(0, eqPos);
	std::string right = cleaned.substr(eqPos + 1);
	
	// Убираем пробелы
	left.erase(std::remove_if(left.begin(), left.end(), ::isspace), left.end());
	right.erase(std::remove_if(right.begin(), right.end(), ::isspace), right.end());
	
	// Определяем, что слева, а что справа
	// Если слева одна буква - это клавиша, справа - команда
	// Если справа одна буква - это клавиша, слева - команда
	if (left.length() == 1 && right.length() > 1) {
		// Формат: key=COMMAND
		key = std::tolower(left[0]);
		commandStr = right;
		return true;
	} else if (right.length() == 1 && left.length() > 1) {
		// Формат: COMMAND=key
		commandStr = left;
		key = std::tolower(right[0]);
		return true;
	} else if (left.length() == 1 && right.length() == 1) {
		// Обе части - одна буква, считаем что слева команда (однобуквенная), справа клавиша
		commandStr = left;
		key = std::tolower(right[0]);
		return true;
	}
	
	return false;
}

std::string ControlConfigLoader::cleanLine(const std::string& line) {
	std::string result = line;
	
	// Удаляем комментарии (все после #)
	size_t commentPos = result.find('#');
	if (commentPos != std::string::npos) {
		result = result.substr(0, commentPos);
	}
	
	// Убираем пробелы в начале и конце
	size_t start = result.find_first_not_of(" \t\r\n");
	if (start == std::string::npos) {
		return "";
	}
	size_t end = result.find_last_not_of(" \t\r\n");
	return result.substr(start, end - start + 1);
}

ControlConfig ControlConfigLoader::loadFromFile(const std::string& filename) {
	ControlConfig config;
	// Конструктор ControlConfig уже инициализирует значения по умолчанию
	// Мы будем перезаписывать их значениями из файла
	
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Warning: Could not open control config file: " << filename << std::endl;
		std::cerr << "Using default controls." << std::endl;
		return config;
	}
	
	std::cout << "Loading control configuration from: " << filename << std::endl;
	
	std::string line;
	int lineNumber = 0;
	
	while (std::getline(file, line)) {
		lineNumber++;
		
		std::string commandStr;
		char key;
		
		if (parseLine(line, commandStr, key)) {
			ControlConfig::CommandType commandType = ControlConfig::stringToCommandType(commandStr);
			
			// Проверяем, что это не неизвестная команда
			// Проверяем правильно: если команда не распознана, stringToCommandType вернет EXIT только для EXIT/QUIT
			// Но нам нужно проверить, что команда действительно распознана
			std::string upperStr = commandStr;
			std::transform(upperStr.begin(), upperStr.end(), upperStr.begin(), ::toupper);
			bool isValidCommand = (upperStr == "MOVE_UP" || upperStr == "UP" ||
			                       upperStr == "MOVE_DOWN" || upperStr == "DOWN" ||
			                       upperStr == "MOVE_LEFT" || upperStr == "LEFT" ||
			                       upperStr == "MOVE_RIGHT" || upperStr == "RIGHT" ||
			                       upperStr == "ATTACK" ||
			                       upperStr == "CAST_SPELL" || upperStr == "SPELL" ||
			                       upperStr == "SHOW_HAND" || upperStr == "HAND" ||
			                       upperStr == "SWITCH_MODE" || upperStr == "MODE" ||
			                       upperStr == "SAVE" ||
			                       upperStr == "LOAD" ||
			                       upperStr == "EXIT" || upperStr == "QUIT");
			
			if (!isValidCommand) {
				std::cerr << "Warning: Unknown command '" << commandStr << "' at line " << lineNumber << std::endl;
				continue;
			}
			
			config.setKeyForCommand(key, commandType);
			std::cout << "DEBUG: Loaded mapping - " << commandStr << " = '" << key << "'" << std::endl;
		}
	}
	
	file.close();
	
	// Валидируем конфигурацию
	std::cout << "DEBUG: Validating configuration..." << std::endl;
	if (!config.validate()) {
		std::cerr << "ERROR: Invalid control configuration: " << config.getErrorMessage() << std::endl;
		std::cerr << "Using default controls." << std::endl;
		// Возвращаем конфигурацию по умолчанию (конструктор уже инициализирует её)
		ControlConfig defaultConfig;
		return defaultConfig;
	}
	
	std::cout << "SUCCESS: Control configuration loaded successfully from: " << filename << std::endl;
	std::cout << "Active keys: UP='" << config.getKeyForCommand(ControlConfig::CommandType::MOVE_UP)
	          << "' DOWN='" << config.getKeyForCommand(ControlConfig::CommandType::MOVE_DOWN)
	          << "' LEFT='" << config.getKeyForCommand(ControlConfig::CommandType::MOVE_LEFT)
	          << "' RIGHT='" << config.getKeyForCommand(ControlConfig::CommandType::MOVE_RIGHT) << "'" << std::endl;
	return config;
}

bool ControlConfigLoader::saveToFile(const ControlConfig& config, const std::string& filename) {
	std::ofstream file(filename);
	if (!file.is_open()) {
		return false;
	}
	
	file << "# Game Control Configuration File" << std::endl;
	file << "# Format: COMMAND=key or key=COMMAND" << std::endl;
	file << "# Available commands: MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT," << std::endl;
	file << "#                      ATTACK, CAST_SPELL, SHOW_HAND, SWITCH_MODE," << std::endl;
	file << "#                      SAVE, LOAD, EXIT" << std::endl;
	file << std::endl;
	
	auto requiredCommands = ControlConfig::getRequiredCommands();
	for (const auto& cmd : requiredCommands) {
		char key = config.getKeyForCommand(cmd);
		if (key != '\0') {
			file << ControlConfig::commandTypeToString(cmd) << "=" << key << std::endl;
		}
	}
	
	file.close();
	return true;
}

