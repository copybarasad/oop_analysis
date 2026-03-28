#include "ConsoleCommandReader.h"
#include <iostream>
#include <cctype>

ConsoleCommandReader::ConsoleCommandReader()
    : _bindings(defaultBindings()) {}

ConsoleCommandReader::ConsoleCommandReader(const std::unordered_map<char, std::string>& bindings)
    : _bindings(bindings.empty() ? defaultBindings() : bindings) {}

void ConsoleCommandReader::setBindings(const std::unordered_map<char, std::string>& bindings) {
    _bindings = bindings.empty() ? defaultBindings() : bindings;
}

std::unordered_map<char, std::string> ConsoleCommandReader::defaultBindings() {
    return {
        {'W', "UP"},
        {'S', "DOWN"},
        {'A', "LEFT"},
        {'D', "RIGHT"},
        {'M', "TOGGLE_MODE"},
        {'C', "CAST_SPELL"},
        {'F', "RANGED_ATTACK"},
        {'B', "BUY_SPELL"},
        {'V', "SAVE"},
        {'L', "LOAD"},
        {'Q', "EXIT"}
    };
}

Command ConsoleCommandReader::readCommand(const Player& player, const Hand& hand, int currentLevel, int requiredPoints) const {

    Command cmd;

    std::cout << "\n Характеристики игрока:\n";
    std::cout << "Здоровье: " << player.get_health() << "\n";
    std::cout << "Урон: " << player.get_damage() << "\n";
    std::cout << "Очки: " << player.get_score() << "\n";

    std::cout << "\nУправление (текущие клавиши):\n";
    for (const auto& kv : _bindings) {
        std::cout << kv.first << " -> " << kv.second << "\n";
    }
    std::cout << "Нажмите клавишу: ";
    char key;
    std::cin >> key;
    key = static_cast<char>(std::toupper(key));

    auto it = _bindings.find(key);
    if (it == _bindings.end()) {
        cmd.type = CommandType::None;
        return cmd;
    }

    const std::string& action = it->second;
    if (action == "UP") {
        cmd.type = CommandType::Move;
        cmd.direction = Direction::UP;
    } else if (action == "DOWN") {
        cmd.type = CommandType::Move;
        cmd.direction = Direction::DOWN;
    } else if (action == "LEFT") {
        cmd.type = CommandType::Move;
        cmd.direction = Direction::LEFT;
    } else if (action == "RIGHT") {
        cmd.type = CommandType::Move;
        cmd.direction = Direction::RIGHT;
    } else if (action == "TOGGLE_MODE") {
        cmd.type = CommandType::ToggleAttackMode;
    } else if (action == "CAST_SPELL") {
        cmd.type = CommandType::CastSpell;
        cmd.spellIndex = askSpellIndex(hand.size());
    } else if (action == "RANGED_ATTACK") {
        cmd.type = CommandType::RangedAttack;
    } else if (action == "BUY_SPELL") {
        cmd.type = CommandType::BuySpell;
        std::cout << "1) Direct Damage (100 очков)\n2) Area Damage (200 очков)\n";
        int buy;
        std::cin >> buy;
        if (buy == 1) cmd.spellName = "Direct Damage";
        else if (buy == 2) cmd.spellName = "Area Damage";
        else cmd.type = CommandType::None;
    } else if (action == "SAVE") {
        cmd.type = CommandType::SaveGame;
        cmd.filename = askFilename("Имя файла для сохранения: ");
    } else if (action == "LOAD") {
        cmd.type = CommandType::LoadGame;
        cmd.filename = askFilename("Имя файла для загрузки: ");
    } else if (action == "EXIT") {
        cmd.type = CommandType::ExitGame;
    } else {
        cmd.type = CommandType::None;
    }

    return cmd;
}

int ConsoleCommandReader::askEnemyIndex(size_t enemyCount) const {
    if (enemyCount == 0) return -1;
    std::cout << "Выберите индекс врага: ";
    int idx;
    std::cin >> idx;
    return idx;
}

int ConsoleCommandReader::askSpellIndex(size_t spellCount) const {
    if (spellCount == 0) return -1;
    std::cout << "Выберите индекс заклинания: ";
    int idx;
    std::cin >> idx;
    return idx;
}

std::string ConsoleCommandReader::askFilename(const std::string& prompt) const {
    std::cout << prompt;
    std::string name;
    std::cin >> name;
    return name;
}
