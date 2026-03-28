#ifndef GAMELOADER_H
#define GAMELOADER_H

#include "GameState.h"
#include <string>

class GameField;
class Player;
class Enemy;
class EnemyManager;
class LevelManager;

class GameLoader {
public:
    static bool loadGame(int slot, GameField& field, Player& player, 
                        Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager);
    
private:
    static GameState readFromFile(int slot);
    static void validateGameState(const GameState& state);
    static void restoreGameState(const GameState& state, GameField& field, Player& player, 
                               Enemy& enemy, EnemyManager& enemyManager, LevelManager& levelManager);
};

#endif