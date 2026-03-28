#ifndef GAME_H
#define GAME_H

#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyBarrack.h"
#include "EnemyTower.h"
#include "CombatManager.h"
#include "AllyUnit.h"
#include "GameState.h"
#include "LevelUpSystem.h"
#include <vector>
#include <memory>

class Game {
private:
    GameField field;
    Player player;
    std::unique_ptr<LevelUpSystem> levelUpSystem;
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<EnemyBarrack>> barracks;
    std::vector<std::shared_ptr<EnemyTower>> towers; 
    std::vector<std::shared_ptr<EnemyBuilding>> buildings; 
    std::vector<std::shared_ptr<AllyUnit>> allies;
    CombatManager combatManager;
    bool gameRunning;
    bool playerWon;
    int killCount;
    int currentLevel;
    int totalLevels;

public:
    Game(int fieldSize = DEFAULT_FIELD_SIZE);

    struct GameEndData {
        bool playerWon;
        bool playerAlive;
        int score;
        int kills;
        int level;
        int totalLevels;
    };
    
    GameEndData getGameEndData() const {
        return GameEndData{
            playerWon,
            player.isAlive(),
            player.getScorePoints(),
            killCount,
            currentLevel,
            totalLevels
        };
    }
    
    // Основные игровые методы
    void addAlly(std::unique_ptr<AllyUnit> ally);
    
    // Методы для сохранения/загрузки
    void saveGame(const std::string& filename);
    void loadGame(const std::string& filename);
    
    // Методы для работы с GameState
    GameState captureGameState() const;
    void applyGameState(const GameState& state);

    // Геттеры для визуализатора и контроллера
    const GameField& getField() const { return field; }
    GameField& getField() { return field; }
    const Player& getPlayer() const { return player; }
    Player& getPlayer() { return player; }
    const std::vector<std::shared_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::shared_ptr<EnemyBuilding>>& getBuildings() const { return buildings; }
    const std::vector<std::shared_ptr<AllyUnit>>& getAllies() const { return allies; }
    const std::vector<std::shared_ptr<EnemyBarrack>>& getBarracks() const { return barracks; }
    const std::vector<std::shared_ptr<EnemyTower>>& getTowers() const { return towers; }
    
    int getKillCount() const { return killCount; }
    int getCurrentLevel() const { return currentLevel; }
    int getTotalLevels() const { return totalLevels; }
    bool isGameRunning() const { return gameRunning; }
    bool isPlayerWon() const { return playerWon; }
    
    void quitGame() { gameRunning = false; }
    void setPlayerWon(bool won) { playerWon = won; }

    void advanceToNextLevel();            
    bool checkGameEndCondition() const;  
    void updateGameState();

    void updateEnemies();
    void updateBarracks();
    void updateTowers();
    void updateAllies();
    void removeDeadUnits();
    void onEnemyKilled();
    
    void movePlayer(Direction direction);
    void performAttack();
    void changeFightingStyle();
    bool processPlayerSpell();

private:
    void createInitialUnits();
    void createEnemies(int count);
    void createBarracks(int count);
    void createTowers(int count);
    bool isValidSpawnPosition(int x, int y) const;

    void resetGame();
    
    // Методы для работы с GameState
    void createEnemyFromData(const GameState::UnitData& data);
    void createBuildingFromData(const GameState::UnitData& data);
    void createAllyFromData(const GameState::UnitData& data);
};

#endif