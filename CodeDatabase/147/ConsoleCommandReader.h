#pragma once

#include <string>
#include <cstddef>
#include <unordered_map>
#include "Command.h"
#include "player.h"
#include "hand.h"

// Класс, отвечающий за считывание пользовательского ввода
// и преобразование его в объект Command.
class ConsoleCommandReader {
public:
    ConsoleCommandReader();
    explicit ConsoleCommandReader(const std::unordered_map<char, std::string>& bindings);

    Command readCommand(const Player& player, const Hand& hand, int currentLevel, int requiredPoints) const;
    int askEnemyIndex(size_t enemyCount) const;
    int askSpellIndex(size_t spellCount) const;
    std::string askFilename(const std::string& prompt) const;

    void setBindings(const std::unordered_map<char, std::string>& bindings);
    static std::unordered_map<char, std::string> defaultBindings();
    const std::unordered_map<char, std::string>& getBindings() const { return _bindings; }

private:
    std::unordered_map<char, std::string> _bindings;
};
