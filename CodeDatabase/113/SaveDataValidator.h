#ifndef SAVEDATAVALIDATOR_H
#define SAVEDATAVALIDATOR_H

#include "GameState.h"
#include <string>

class SaveDataValidator {
public:
    static bool validateGameState(const GameState& gameState);
    static bool validatePlayerData(const GameState& gameState);
    static bool validateEnemiesData(const GameState& gameState);
    static bool validateTowersData(const GameState& gameState);
    static bool validateFieldData(const GameState& gameState);

private:
    static const int MAX_PLAYER_HEALTH;
    static const int MAX_ENEMY_HEALTH;
    static const int MAX_TOWER_HEALTH;
    static const int MAX_LEVEL;
    static const int MAX_TURN_COUNT;
};

#endif // SAVEDATAVALIDATOR_H