#ifndef GAME_CONFIG_H
#define GAME_CONFIG_H

#include <string>
#include <unordered_map>

struct GameConfig {
    char moveUpKey;
    char moveDownKey;
    char moveLeftKey;
    char moveRightKey;
    char switchModeKey;
    char rangedAttackKey;
    char saveGameKey;
    char loadGameKey;
    char castSpellKey;
    char quitKey;
    
    GameConfig();
    bool validate() const;
    std::string getValidationError() const;
};

#endif