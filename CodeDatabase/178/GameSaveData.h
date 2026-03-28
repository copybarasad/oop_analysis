#ifndef GAME_SAVE_DATA_H
#define GAME_SAVE_DATA_H

#include "Position.h"
#include "CombatType.h"

struct GameSaveData {
    int currentLevel = 1;
    int playerHealth = 100;
    int playerMaxHealth = 100;
    int playerMeleeDamage = 20;
    int playerRangedDamage = 15;
    int playerScore = 0;
    int playerCoins = 0;
    Position playerPosition = Position(0, 0);
    CombatType playerCombatType = CombatType::MELEE;
    int fieldWidth = 20;
    int fieldHeight = 20;
    std::vector<CellType> fieldCells;
    GameSaveData() = default;
};
#endif