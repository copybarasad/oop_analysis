#include "ControlMapping.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iostream>

namespace {
    CommandType commandFromString(const std::string &name) {
        if (name == "MoveUp") return CommandType::MOVE_UP;
        if (name == "MoveDown") return CommandType::MOVE_DOWN;
        if (name == "MoveLeft") return CommandType::MOVE_LEFT;
        if (name == "MoveRight") return CommandType::MOVE_RIGHT;
        if (name == "Attack") return CommandType::ATTACK;
        if (name == "SwitchWeapon") return CommandType::SWITCH_WEAPON;
        if (name == "CastSpell") return CommandType::CAST_SPELL;
        if (name == "Save") return CommandType::SAVE_GAME;
        if (name == "Load") return CommandType::LOAD_GAME;
        if (name == "Exit") return CommandType::EXIT;
        if (name == "Help") return CommandType::HELP;
        return CommandType::NONE;
    }
}

ControlMapping::ControlMapping() {
    setDefault();
}

void ControlMapping::setDefault() {
    // Базовая раскладка управления
    keyToCommand = {
        {'W', CommandType::MOVE_UP},
        {'A', CommandType::MOVE_LEFT},
        {'S', CommandType::MOVE_DOWN},
        {'D', CommandType::MOVE_RIGHT},
        {'G', CommandType::ATTACK},
        {'F', CommandType::SWITCH_WEAPON},
        {'C', CommandType::CAST_SPELL},
        {'Q', CommandType::SAVE_GAME},
        {'R', CommandType::LOAD_GAME},
        {'X', CommandType::EXIT}
    };
}

bool ControlMapping::isValid(const std::unordered_map<char, CommandType> &candidate) const {
    auto has = [&candidate](CommandType ct) {
        for (const auto &p: candidate) {
            if (p.second == ct) return true;
        }
        return false;
    };
    if (!has(CommandType::MOVE_UP) || !has(CommandType::MOVE_DOWN) ||
        !has(CommandType::MOVE_LEFT) || !has(CommandType::MOVE_RIGHT) ||
        !has(CommandType::ATTACK) || !has(CommandType::CAST_SPELL)) {
        return false;
    }
    for (const auto &outer: candidate) {
        for (const auto &inner: candidate) {
            if (&outer != &inner && outer.second == inner.second) {
                return false;
            }
        }
    }
    return true;
}

bool ControlMapping::loadFromFile(const std::string &path,
                                  std::function<void(const std::string &)> reporter) {
    std::ifstream file(path);
    if (!file.is_open()) {
        if (reporter) reporter("controls.cfg не найден, используются настройки по умолчанию");
        return false;
    }
    std::unordered_map<char, CommandType> candidate;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::size_t pos = line.find('=');
        if (pos == std::string::npos || pos == 0 || pos == line.size() - 1) {
            if (reporter) reporter("Некорректная строка в controls.cfg, используются настройки по умолчанию");
            return false;
        }
        char key = static_cast<char>(std::toupper(static_cast<unsigned char>(line[0])));
        std::string cmdName = line.substr(pos + 1);
        cmdName.erase(std::remove_if(cmdName.begin(), cmdName.end(), ::isspace), cmdName.end());
        CommandType type = commandFromString(cmdName);
        if (type == CommandType::NONE) {
            if (reporter) reporter("Неизвестная команда в controls.cfg, используются настройки по умолчанию");
            return false;
        }
        if (candidate.find(key) != candidate.end()) {
            if (reporter) reporter("Дублирование клавиши в controls.cfg, используются настройки по умолчанию");
            return false;
        }
        candidate[key] = type;
    }
    if (!isValid(candidate)) {
        if (reporter) reporter("Некорректная карта управления в controls.cfg, используются настройки по умолчанию");
        return false;
    }
    keyToCommand = candidate;
    return true;
}

CommandType ControlMapping::commandForKey(char key) const {
    char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(key)));
    auto it = keyToCommand.find(upper);
    if (it != keyToCommand.end()) {
        return it->second;
    }
    return CommandType::NONE;
}

const std::unordered_map<char, CommandType> &ControlMapping::bindings() const {
    return keyToCommand;
}

char ControlMapping::keyFor(CommandType cmd) const {
    for (const auto &p: keyToCommand) {
        if (p.second == cmd) return p.first;
    }
    return '\0';
}
