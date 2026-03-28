#include "Settings.h"

#include <cctype>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

bool isAlphaOrDigit(char key) {
    return std::isprint(static_cast<unsigned char>(key)) && !std::isspace(static_cast<unsigned char>(key));
}

Settings::Settings()
    : moveUp('w'),
      moveDown('s'),
      moveLeft('a'),
      moveRight('d'),
      switchCombatMode('c'),
      attack('x'),
      castSpell('v'),
      buySpell('b'),
      saveGame('z'),
      loadGame('l'),
      help('h'),
      exitGame('q'),
      startNewGame('n') {}

Settings Settings::createDefault() {
    return Settings();
}



bool Settings::isValid() const {
    auto normalize = [](char key) {
        return static_cast<char>(std::tolower(static_cast<unsigned char>(key)));
    };

    std::unordered_map<char, std::string> keyToCommand;
    std::unordered_set<char> keys = {
        moveUp, moveDown, moveLeft, moveRight,
        switchCombatMode, attack, castSpell, buySpell,
        saveGame, loadGame, help, exitGame,
        startNewGame
    };

    // Нельзя назначать пробел/невидимые символы
    for (char key : keys) {
        if (!isAlphaOrDigit(key)) {
            return false;
        }
    }
    auto registerKey = [&keyToCommand, &normalize](char key, const std::string& command) {
        char normalized = normalize(key);
        auto [it, inserted] = keyToCommand.emplace(normalized, command);
        return inserted;
    };

    return registerKey(moveUp, "moveUp") &&
           registerKey(moveDown, "moveDown") &&
           registerKey(moveLeft, "moveLeft") &&
           registerKey(moveRight, "moveRight") &&
           registerKey(switchCombatMode, "switchCombatMode") &&
           registerKey(attack, "attack") &&
           registerKey(castSpell, "castSpell") &&
           registerKey(buySpell, "buySpell") &&
           registerKey(saveGame, "saveGame") &&
           registerKey(loadGame, "loadGame") &&
           registerKey(help, "help") &&
           registerKey(exitGame, "exitGame") &&
           registerKey(startNewGame, "startNewGame");
}

