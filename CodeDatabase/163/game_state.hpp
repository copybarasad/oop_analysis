#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <vector>
#include <memory>
#include <string>
#include <tuple>
#include "game_object.hpp"
#include "player.hpp"
#include "map.hpp"

class GameState {
private:
    std::unique_ptr<Map> map;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<GameObject>> enemies;
    std::vector<std::unique_ptr<GameObject>> buildings;
    
    int currentTurn;
    bool isGameRunning;
    int currentLevel;
    int score;
    
public:
    GameState();

    void activateGame() { isGameRunning = true; }
    void initializeNewGame();
    void initializeLevel(int level);
    void cleanupGame();
    
    Map* getMap() const;
    Player* getPlayer() const;
    const std::vector<std::unique_ptr<GameObject>>& getEnemies() const;
    const std::vector<std::unique_ptr<GameObject>>& getBuildings() const;
    
    int getCurrentTurn() const;
    int getCurrentLevel() const;
    int getScore() const;
    bool isGameActive() const;
    
    void endGame();
    void addScore(int points);
    
    void nextTurn();
    void processEnemyTurns();
    void cleanupDeadObjects();
    
    void enemyDefeated();
    
    bool isVictory() const;
    bool isDefeat() const;
    
    void saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    void setCurrentTurn(int turn);
    void setCurrentLevel(int level);
    void setScore(int newScore);
    
    void createPlayer(int x, int y, Cell& cell, int hp, int maxHp);
    void addEnemy(std::unique_ptr<GameObject> enemy);
    void addBuilding(std::unique_ptr<GameObject> building);
    
    void createMapForLevel(int level);
    
private:
    void spawnEntitiesForLevel(int level);
    bool isValidSpawnPosition(int x, int y) const;
    
    struct SerializedData {
        int playerX, playerY;
        int playerHP, playerMaxHP;
        std::vector<std::tuple<int, int, int>> enemies;
        std::vector<std::tuple<int, int, int>> buildings;
        int currentTurn;
        int currentLevel;
        int score;
    };
    
    SerializedData serialize() const;
    void deserialize(const SerializedData& data);
};

#endif