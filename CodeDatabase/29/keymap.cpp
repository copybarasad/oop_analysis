#include "keymap.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

namespace {
	UserCommandType FromString(const std::string& s) {
		std::string up = s;
		std::transform(up.begin(), up.end(), up.begin(), [](unsigned char c){ return static_cast<char>(std::toupper(c)); });
		if (up == "UP") return UserCommandType::MoveUp;
		if (up == "DOWN") return UserCommandType::MoveDown;
		if (up == "LEFT") return UserCommandType::MoveLeft;
		if (up == "RIGHT") return UserCommandType::MoveRight;
		if (up == "ATTACK_UP") return UserCommandType::AttackUp;
		if (up == "ATTACK_DOWN") return UserCommandType::AttackDown;
		if (up == "ATTACK_LEFT") return UserCommandType::AttackLeft;
		if (up == "ATTACK_RIGHT") return UserCommandType::AttackRight;
		if (up == "TOGGLE") return UserCommandType::ToggleMode;
		if (up == "CAST") return UserCommandType::Cast;
		if (up == "SAVE") return UserCommandType::Save;
		if (up == "LOAD") return UserCommandType::Load;
		if (up == "EXIT") return UserCommandType::Exit;
		return UserCommandType::Unknown;
	}
}

Keymap::Keymap() {
	SetDefault();
}

void Keymap::SetDefault() {
	mapping_.clear();
	mapping_['W'] = UserCommandType::MoveUp;
	mapping_['S'] = UserCommandType::MoveDown;
	mapping_['A'] = UserCommandType::MoveLeft;
	mapping_['D'] = UserCommandType::MoveRight;
	mapping_['U'] = UserCommandType::AttackUp;
	mapping_['V'] = UserCommandType::AttackDown;
	mapping_['L'] = UserCommandType::AttackLeft;
	mapping_['R'] = UserCommandType::AttackRight;
	mapping_['T'] = UserCommandType::ToggleMode;
	mapping_['C'] = UserCommandType::Cast;
	mapping_['Z'] = UserCommandType::Save;
	mapping_['X'] = UserCommandType::Load;
	mapping_['E'] = UserCommandType::Exit;
}

bool Keymap::Validate(const std::unordered_map<char, UserCommandType>& map) const {
	std::unordered_map<UserCommandType, char> reverse;
	for (const auto& kv : map) {
		if (kv.first == 0) return false;
		if (kv.second == UserCommandType::Unknown) return false;
		if (reverse.count(kv.second)) {
			return false;
		}
		reverse[kv.second] = kv.first;
	}
	UserCommandType required[] = {
		UserCommandType::MoveUp, UserCommandType::MoveDown, UserCommandType::MoveLeft, UserCommandType::MoveRight,
		UserCommandType::AttackUp, UserCommandType::AttackDown, UserCommandType::AttackLeft, UserCommandType::AttackRight,
		UserCommandType::ToggleMode, UserCommandType::Cast, UserCommandType::Save, UserCommandType::Load, UserCommandType::Exit
	};
	for (UserCommandType ct : required) {
		if (!reverse.count(ct)) return false;
	}
	return true;
}

bool Keymap::LoadFromFile(const std::string& path, std::string& error) {
	std::ifstream in(path);
	if (!in) {
		error = "cannot open keymap file";
		return false;
	}
	std::unordered_map<char, UserCommandType> tmp;
	std::string line;
	int line_no = 0;
	while (std::getline(in, line)) {
		++line_no;
		if (line.empty()) continue;
		std::istringstream iss(line);
		std::string keyStr, cmdStr;
		if (!(iss >> keyStr >> cmdStr)) {
			error = "parse error at line " + std::to_string(line_no);
			return false;
		}
		if (keyStr.size() != 1) {
			error = "invalid key at line " + std::to_string(line_no);
			return false;
		}
		char key = static_cast<char>(std::toupper(static_cast<unsigned char>(keyStr[0])));
		UserCommandType ct = FromString(cmdStr);
		if (ct == UserCommandType::Unknown) {
			error = "unknown command at line " + std::to_string(line_no);
			return false;
		}
		if (tmp.count(key)) {
			error = "duplicate key at line " + std::to_string(line_no);
			return false;
		}
		tmp[key] = ct;
	}
	if (!Validate(tmp)) {
		error = "keymap missing required bindings or has duplicates";
		return false;
	}
	mapping_.swap(tmp);
	return true;
}

UserCommandType Keymap::Resolve(char key) const {
	char k = static_cast<char>(std::toupper(static_cast<unsigned char>(key)));
	auto it = mapping_.find(k);
	if (it != mapping_.end()) return it->second;
	return UserCommandType::Unknown;
}
