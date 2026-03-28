#ifndef GAMESTATE_H
#define GAMESTATE_H
#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class GameState {
private:
    GameField gameField;
    Player player;
    std::vector<Enemy> enemies;
    bool isGameRunning;

public:
    GameState(int fieldWidth, int fieldHeight);
    
    GameState(GameState&& other) = default;
    GameState& operator=(GameState&& other) = default;
    
    GameState(const GameState& other) = delete;
    GameState& operator=(const GameState& other) = delete;
    
    const GameField& getGameField() const {
        return gameField;
    }
    const Player& getPlayer() const {
        return player;
    }
    Player& getPlayer() {
        return player;
    }
    const std::vector<Enemy>& getEnemies() const {
        return enemies;
    }
    std::vector<Enemy>& getEnemies() {
        return enemies;
    }
    bool getIsGameRunning() const {
        return isGameRunning;
    }
    void setPlayerPosition(const Position& newPosition);
    void setGameRunning(bool running) { isGameRunning = running; } 
    void damagePlayer(int damageAmount);
    void damageEnemy(int enemyIndex, int damageAmount);
    void addEnemy(const Enemy& enemy);
    void removeDeadEnemies();
    
    static std::unique_ptr<GameState> createNewGame(int fieldWidth, int fieldHeight);
    static std::unique_ptr<GameState> createEmptyGame(int fieldWidth, int fieldHeight);
};

#endif