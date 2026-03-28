#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <string>
#include <vector>
#include "player.h"
#include "field.h"

class GameState {
public:
    GameState();
    GameState(const GameState& other);
    GameState& operator=(const GameState& other);
    
    int currentLevel = 1;
    int playerHealthMax = 50;
    int playerHealth = 50;
    int playerMeleeDamage = 5;
    int playerRangedDamage = 3;
    int playerScore = 0;
    int fieldRows = 12;
    int fieldCols = 12;
    std::vector<std::string> playerSpells;
    std::vector<int> entityIds;
    std::vector<int> entityHealths;
    std::vector<int> entityTypes;
    std::vector<int> entityPositionsRow;
    std::vector<int> entityPositionsCols;
    
    std::string version = "1.0";
    static constexpr const char* VERSION = "1.0";
};

#endif
