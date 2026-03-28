#ifndef GAMESAVER_H
#define GAMESAVER_H

#include "GameState.h"
#include <string>

class GameField;
class Player;
class Enemy;
class EnemyManager;
class LevelManager;

class GameSaver {
public:
    static bool saveGame(int slot, const GameField& field, const Player& player, 
                        const Enemy& enemy, const EnemyManager& enemyManager, const LevelManager& levelManager);
    static void showSaves();
    
private:
    static GameState collectGameState(const GameField& field, const Player& player, 
                                     const Enemy& enemy, const EnemyManager& enemyManager, const LevelManager& levelManager);
    static void validateStateBeforeSave(const GameState& state);
    static void writeToFile(const std::string& filename, const GameState& state);
};

#endif