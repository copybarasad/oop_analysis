 #include "control_config.h"
 #include <algorithm>
 #include <cctype>
 #include <fstream>
#include <iostream>
#include <sstream>
 #include <unordered_map>

 namespace {
 
 // Имя команды -> перечисление
 const std::unordered_map<std::string, CommandName> kNameMap = {
 	{"move_up", CommandName::MoveUp},
 	{"move_down", CommandName::MoveDown},
 	{"move_left", CommandName::MoveLeft},
 	{"move_right", CommandName::MoveRight},
 	{"switch_mode", CommandName::SwitchMode},
 	{"ranged_attack", CommandName::RangedAttack},
 	{"cast_spell", CommandName::CastSpell},
 	{"buy_spell", CommandName::BuySpell},
 	{"show_info", CommandName::ShowInfo},
 	{"save", CommandName::Save},
 	{"load", CommandName::Load},
 	{"quit", CommandName::Quit},
 	{"help", CommandName::Help}
 };
 
 // Базовый набор команд с привязками по умолчанию
 const std::map<char, CommandBinding> kDefaultBindings = {
 	{'w', {CommandName::MoveUp, CommandType::Move, 0, -1, false, false}},
 	{'s', {CommandName::MoveDown, CommandType::Move, 0, 1, false, false}},
 	{'a', {CommandName::MoveLeft, CommandType::Move, -1, 0, false, false}},
 	{'d', {CommandName::MoveRight, CommandType::Move, 1, 0, false, false}},
 	{'m', {CommandName::SwitchMode, CommandType::SwitchMode, 0, 0, false, false}},
 	{'f', {CommandName::RangedAttack, CommandType::RangedAttack, 0, 0, true, false}},
 	{'c', {CommandName::CastSpell, CommandType::CastSpell, 0, 0, false, true}},
 	{'b', {CommandName::BuySpell, CommandType::BuySpell, 0, 0, false, false}},
 	{'i', {CommandName::ShowInfo, CommandType::ShowInfo, 0, 0, false, false}},
 	{'p', {CommandName::Save, CommandType::Save, 0, 0, false, false}},
 	{'o', {CommandName::Load, CommandType::Load, 0, 0, false, false}},
 	{'q', {CommandName::Quit, CommandType::Quit, 0, 0, false, false}},
 	{'h', {CommandName::Help, CommandType::Help, 0, 0, false, false}}
 };
 
 std::string trim(const std::string& value) {
 	auto begin = value.find_first_not_of(" \t\r\n");
 	if (begin == std::string::npos) {
 		return "";
 	}
 	auto end = value.find_last_not_of(" \t\r\n");
 	return value.substr(begin, end - begin + 1);
 }
 
 } // namespace
 
 ControlConfig ControlConfig::createDefault() {
 	ControlConfig cfg;
 	cfg.bindings = kDefaultBindings;
 	cfg.fallbackUsed = true;
 	return cfg;
 }
 
 ControlConfig ControlConfig::loadFromFile(const std::string& path) {
 	std::ifstream input(path);
 	if (!input.is_open()) {
 		return createDefault();
 	}
 
 	std::map<char, CommandBinding> result;
 	std::unordered_map<CommandName, char> reverseIndex;
 	std::string line;
 	bool invalid = false;
 
	while (std::getline(input, line)) {
		if (line.empty()) {
			continue;
		}

		// Пропускаем комментарии (строки, начинающиеся с #)
		std::string trimmedLine = trim(line);
		if (trimmedLine.empty() || trimmedLine[0] == '#') {
			continue;
		}

		auto delimiter = line.find('=');
		if (delimiter == std::string::npos) {
			invalid = true;
			break;
		}
 
 		std::string namePart = trim(line.substr(0, delimiter));
 		std::string keyPart = trim(line.substr(delimiter + 1));
 		std::transform(namePart.begin(), namePart.end(), namePart.begin(), [](unsigned char c) { return std::tolower(c); });
 
 		auto nameIt = kNameMap.find(namePart);
 		if (nameIt == kNameMap.end() || keyPart.size() != 1) {
 			invalid = true;
 			break;
 		}
 
 		char key = static_cast<char>(std::tolower(keyPart[0]));
 		CommandName commandName = nameIt->second;
 
 		if (result.find(key) != result.end()) {
 			invalid = true;
 			break;
 		}
 		if (reverseIndex.find(commandName) != reverseIndex.end()) {
 			invalid = true;
 			break;
 		}
 
		CommandBinding binding{commandName, CommandType::Invalid, 0, 0, false, false};
		bool foundTemplate = false;
		// Берем параметры из шаблона по имени, а не по клавише.
 		for (const auto& [defaultKey, defaultBinding] : kDefaultBindings) {
 			if (defaultBinding.name == commandName) {
 				binding = defaultBinding;
				foundTemplate = true;
 				break;
 			}
 		}
 
		if (!foundTemplate) {
 			invalid = true;
 			break;
 		}
 
 		result.insert({key, binding});
 		reverseIndex.insert({commandName, key});
 	}
 
 	// Проверяем, что каждая команда настроена ровно один раз
 	if (!invalid && reverseIndex.size() == kNameMap.size()) {
 		ControlConfig cfg;
 		cfg.bindings = std::move(result);
 		cfg.fallbackUsed = false;
 		return cfg;
 	}
 
	std::cout << "Invalid control mapping file. Using default controls.\n";
 	return createDefault();
 }
 
 const CommandBinding* ControlConfig::findBinding(char key) const {
 	auto it = bindings.find(static_cast<char>(std::tolower(key)));
 	if (it == bindings.end()) {
 		return nullptr;
 	}
 	return &it->second;
 }
 
 std::optional<char> ControlConfig::keyFor(CommandName name) const {
 	for (const auto& [key, binding] : bindings) {
 		if (binding.name == name) {
 			return key;
 		}
 	}
 	return std::nullopt;
 }

