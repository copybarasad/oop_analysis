#include "game_config.h"
#include <cctype>
#include <set>

GameConfig::GameConfig()
    : moveUpKey('W'), moveDownKey('S'), moveLeftKey('A'), moveRightKey('D'),
      switchModeKey('M'), rangedAttackKey('X'), saveGameKey('Z'), 
      loadGameKey('L'), castSpellKey('0'), quitKey('Q') {}

bool GameConfig::validate() const {
    std::set<char> usedKeys;

    char keys[] = {
        moveUpKey, moveDownKey, moveLeftKey, moveRightKey,
        switchModeKey, rangedAttackKey, saveGameKey, loadGameKey,
        quitKey
    };

    for (char key : keys) {
        if (key == '\0') {
            return false;
        }
    }

    for (char key : keys) {
        char upperKey = std::toupper(key);
        if (usedKeys.count(upperKey) > 0) {
            return false;
        }
        usedKeys.insert(upperKey);
    }

    if (!std::isdigit(castSpellKey) && castSpellKey != '\0') {
        return false;
    }

    if (usedKeys.count(castSpellKey) > 0) {
        return false;
    }
    
    return true;
}

std::string GameConfig::getValidationError() const {
    std::set<char> usedKeys;

    char keys[] = {
        moveUpKey, moveDownKey, moveLeftKey, moveRightKey,
        switchModeKey, rangedAttackKey, saveGameKey, loadGameKey,
        quitKey
    };

    for (char key : keys) {
        if (key == '\0') {
            return "Missing configuration for one or more commands";
        }
    }

    for (char key : keys) {
        char upperKey = std::toupper(key);
        if (usedKeys.count(upperKey) > 0) {
            return std::string("Duplicate key binding for '") + upperKey + "'";
        }
        usedKeys.insert(upperKey);
    }

    if (!std::isdigit(castSpellKey) && castSpellKey != '\0') {
        return "Invalid spell cast key: must be a digit or empty";
    }
    
    if (usedKeys.count(castSpellKey) > 0) {
        return std::string("Spell cast key '") + castSpellKey + "' conflicts with another command";
    }
    
    return "Unknown validation error";
}