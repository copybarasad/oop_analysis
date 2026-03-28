#include "ControlScheme.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace {
int normalizeKey(int key) {
	if (key >= 'A' && key <= 'Z') {
		return key - 'A' + 'a';
	}
	return key;
}
}

ControlScheme::ControlScheme() {
	resetToDefaults();
}

void ControlScheme::resetToDefaults() {
	mapping_.clear();
	mapping_.emplace(GameCommand::Type::MoveUp, 'w');
	mapping_.emplace(GameCommand::Type::MoveDown, 's');
	mapping_.emplace(GameCommand::Type::MoveLeft, 'a');
	mapping_.emplace(GameCommand::Type::MoveRight, 'd');
	mapping_.emplace(GameCommand::Type::AttackUp, 'i');
	mapping_.emplace(GameCommand::Type::AttackDown, 'k');
	mapping_.emplace(GameCommand::Type::AttackLeft, 'j');
	mapping_.emplace(GameCommand::Type::AttackRight, 'l');
	mapping_.emplace(GameCommand::Type::SwitchMode, 'm');
	mapping_.emplace(GameCommand::Type::CastSpell1, '1');
	mapping_.emplace(GameCommand::Type::CastSpell2, '2');
	mapping_.emplace(GameCommand::Type::CastSpell3, '3');
	mapping_.emplace(GameCommand::Type::CastSpell4, '4');
	mapping_.emplace(GameCommand::Type::CastSpell5, '5');
	mapping_.emplace(GameCommand::Type::SaveGame, 'g');
	mapping_.emplace(GameCommand::Type::LoadGame, 'h');
	mapping_.emplace(GameCommand::Type::QuitToMenu, 'q');
	mapping_.emplace(GameCommand::Type::StartCampaign, 'n');
	mapping_.emplace(GameCommand::Type::LoadCampaignFromMenu, 'o');
	mapping_.emplace(GameCommand::Type::ExitApplication, 'x');
}

bool ControlScheme::loadFromFile(const std::string& path) {
	std::ifstream in(path);
	if (!in) {
		return false;
	}

	Mapping candidate;
	std::string line;
	while (std::getline(in, line)) {
		if (line.empty() || line[0] == '#') {
			continue;
		}
		auto pos = line.find('=');
		if (pos == std::string::npos) {
			continue;
		}
		std::string commandName = line.substr(0, pos);
		std::string keyPart = line.substr(pos + 1);
		commandName.erase(std::remove_if(commandName.begin(), commandName.end(), ::isspace), commandName.end());
		keyPart.erase(std::remove_if(keyPart.begin(), keyPart.end(), ::isspace), keyPart.end());
		if (keyPart.empty()) {
			continue;
		}
		auto cmd = commandFromName(commandName);
		if (!cmd.has_value()) {
			continue;
		}
		int key = normalizeKey(static_cast<unsigned char>(keyPart.front()));
		candidate[*cmd] = key;
	}

	for (auto required : requiredCommands()) {
		if (candidate.find(required) == candidate.end()) {
			resetToDefaults();
			return false;
		}
	}

	if (hasConflicts(candidate)) {
		resetToDefaults();
		return false;
	}

	mapping_ = std::move(candidate);
	return true;
}

GameCommand::Type ControlScheme::commandFor(int ch) const noexcept {
	const int key = normalizeKey(ch);
	for (const auto& pair : mapping_) {
		if (pair.second == key) {
			return pair.first;
		}
	}
	return GameCommand::Type::None;
}

int ControlScheme::keyFor(GameCommand::Type type) const noexcept {
	auto it = mapping_.find(type);
	if (it == mapping_.end()) {
		return 0;
	}
	return it->second;
}

std::vector<GameCommand::Type> ControlScheme::requiredCommands() {
	return {
	    GameCommand::Type::MoveUp,
	    GameCommand::Type::MoveDown,
	    GameCommand::Type::MoveLeft,
	    GameCommand::Type::MoveRight,
	    GameCommand::Type::AttackUp,
	    GameCommand::Type::AttackDown,
	    GameCommand::Type::AttackLeft,
	    GameCommand::Type::AttackRight,
	    GameCommand::Type::SwitchMode,
	    GameCommand::Type::CastSpell1,
	    GameCommand::Type::CastSpell2,
	    GameCommand::Type::CastSpell3,
	    GameCommand::Type::CastSpell4,
	    GameCommand::Type::CastSpell5,
	    GameCommand::Type::SaveGame,
	    GameCommand::Type::LoadGame,
	    GameCommand::Type::QuitToMenu,
	    GameCommand::Type::StartCampaign,
	    GameCommand::Type::LoadCampaignFromMenu,
	    GameCommand::Type::ExitApplication
	};
}

std::optional<GameCommand::Type> ControlScheme::commandFromName(const std::string& name) {
	const std::string lower = [&]() {
		std::string s = name;
		std::transform(s.begin(), s.end(), s.begin(), [](unsigned char ch) {
			return static_cast<char>(std::tolower(ch));
		});
		return s;
	}();

	if (lower == "move_up") return GameCommand::Type::MoveUp;
	if (lower == "move_down") return GameCommand::Type::MoveDown;
	if (lower == "move_left") return GameCommand::Type::MoveLeft;
	if (lower == "move_right") return GameCommand::Type::MoveRight;
	if (lower == "attack_up") return GameCommand::Type::AttackUp;
	if (lower == "attack_down") return GameCommand::Type::AttackDown;
	if (lower == "attack_left") return GameCommand::Type::AttackLeft;
	if (lower == "attack_right") return GameCommand::Type::AttackRight;
	if (lower == "switch_mode") return GameCommand::Type::SwitchMode;
	if (lower == "spell_1") return GameCommand::Type::CastSpell1;
	if (lower == "spell_2") return GameCommand::Type::CastSpell2;
	if (lower == "spell_3") return GameCommand::Type::CastSpell3;
	if (lower == "spell_4") return GameCommand::Type::CastSpell4;
	if (lower == "spell_5") return GameCommand::Type::CastSpell5;
	if (lower == "save") return GameCommand::Type::SaveGame;
	if (lower == "load") return GameCommand::Type::LoadGame;
	if (lower == "quit_to_menu") return GameCommand::Type::QuitToMenu;
	if (lower == "menu_start") return GameCommand::Type::StartCampaign;
	if (lower == "menu_load") return GameCommand::Type::LoadCampaignFromMenu;
	if (lower == "exit") return GameCommand::Type::ExitApplication;
	return std::nullopt;
}

bool ControlScheme::hasConflicts(const Mapping& mapping) {
	std::unordered_map<int, GameCommand::Type> seen;
	for (const auto& pair : mapping) {
		int normalized = normalizeKey(pair.second);
		if (seen.find(normalized) != seen.end()) {
			return true;
		}
		seen[normalized] = pair.first;
	}
	return false;
}
