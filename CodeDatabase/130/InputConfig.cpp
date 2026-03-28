#include "InputConfig.h"
#include "InputUtils.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace {
const char* DEFAULT_KEYS[] = {"w", "s", "a", "d", "f", "c", "v", "t", "q"};
const CommandType DEFAULT_COMMANDS[] = {
    CommandType::MOVE_UP,
    CommandType::MOVE_DOWN,
    CommandType::MOVE_LEFT,
    CommandType::MOVE_RIGHT,
    CommandType::ATTACK,
    CommandType::CAST_SPELL,
    CommandType::SAVE,
    CommandType::SWITCH_MODE,
    CommandType::QUIT
};

std::string trim(const std::string& s) {
    auto it1 = std::find_if_not(s.begin(), s.end(), [](unsigned char c){ return std::isspace(c); });
    auto it2 = std::find_if_not(s.rbegin(), s.rend(), [](unsigned char c){ return std::isspace(c); }).base();
    if (it1 >= it2) return "";
    return std::string(it1, it2);
}

CommandType parseCommandName(const std::string& name) {
    if (name == "MOVE_UP") return CommandType::MOVE_UP;
    if (name == "MOVE_DOWN") return CommandType::MOVE_DOWN;
    if (name == "MOVE_LEFT") return CommandType::MOVE_LEFT;
    if (name == "MOVE_RIGHT") return CommandType::MOVE_RIGHT;
    if (name == "ATTACK") return CommandType::ATTACK;
    if (name == "CAST_SPELL") return CommandType::CAST_SPELL;
    if (name == "SAVE") return CommandType::SAVE;
    if (name == "SWITCH_MODE") return CommandType::SWITCH_MODE;
    if (name == "QUIT") return CommandType::QUIT;
    return CommandType::INVALID;
}
}

InputConfig::InputConfig() : valid_(false) {
    setDefault();
}

void InputConfig::setDefault() {
    keyToCommand_.clear();
    for (size_t i = 0; i < sizeof(DEFAULT_COMMANDS) / sizeof(DEFAULT_COMMANDS[0]); ++i) {
        keyToCommand_[DEFAULT_KEYS[i][0]] = DEFAULT_COMMANDS[i];
    }
    valid_ = true;
}

bool InputConfig::validate() const {
    bool hasUp = false, hasDown = false, hasLeft = false, hasRight = false;
    bool hasAttack = false, hasCast = false, hasSave = false, hasSwitch = false, hasQuit = false;

    std::unordered_map<CommandType, int> counts;
    for (const auto& kv : keyToCommand_) {
        counts[kv.second] += 1;
    }

    hasUp = counts[CommandType::MOVE_UP] == 1;
    hasDown = counts[CommandType::MOVE_DOWN] == 1;
    hasLeft = counts[CommandType::MOVE_LEFT] == 1;
    hasRight = counts[CommandType::MOVE_RIGHT] == 1;
    hasAttack = counts[CommandType::ATTACK] == 1;
    hasCast = counts[CommandType::CAST_SPELL] == 1;
    hasSave = counts[CommandType::SAVE] == 1;
    hasSwitch = counts[CommandType::SWITCH_MODE] == 1;
    hasQuit = counts[CommandType::QUIT] == 1;

    bool duplicates = false;
    for (const auto& kv : counts) {
        if (kv.second > 1) duplicates = true;
    }

    return !duplicates && hasUp && hasDown && hasLeft && hasRight && hasAttack && hasCast && hasSave && hasSwitch && hasQuit;
}

bool InputConfig::loadFromFile(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        setDefault();
        return false;
    }

    std::unordered_map<char, CommandType> mapping;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (!line.empty() && line[0] == '#') continue;
        std::istringstream iss(line);
        std::string token;
        if (!std::getline(iss, token, '=')) {
            continue;
        }
        std::string keyStr = trim(token);
        std::string cmdStr;
        if (!std::getline(iss, cmdStr)) {
            continue;
        }
        keyStr = trim(keyStr);
        cmdStr = trim(cmdStr);
        if (keyStr.empty() || cmdStr.empty()) continue;
        char key = normalizeKey(keyStr[0]);
        CommandType ct = parseCommandName(cmdStr);
        if (ct == CommandType::INVALID) {
            mapping.clear();
            break;
        }
        if (mapping.find(key) != mapping.end()) {
            mapping.clear();
            break;
        }
        mapping[key] = ct;
    }

    keyToCommand_ = mapping.empty() ? keyToCommand_ : mapping;
    valid_ = validate();
    if (!valid_) {
        setDefault();
    }
    return valid_;
}

CommandType InputConfig::commandForKey(char key) const {
    char normalized = normalizeKey(key);
    auto it = keyToCommand_.find(normalized);
    if (it == keyToCommand_.end()) {
        return CommandType::INVALID;
    }
    return it->second;
}

bool InputConfig::isValid() const {
    return valid_;
}

std::unordered_map<CommandType, char> InputConfig::getBindings() const {
    std::unordered_map<CommandType, char> rev;
    for (const auto& kv : keyToCommand_) {
        if (rev.find(kv.second) == rev.end()) {
            rev[kv.second] = kv.first;
        }
    }
    return rev;
}

