#pragma once
#include <vector>
#include <memory>
#include <algorithm>
#include "field.hpp"
#include "ally.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "enemy_building.hpp"
#include "enemytower.hpp"
#include "trap.hpp"
#include "gamemove.hpp"
#include "spellfactory.hpp"
#include "enums.hpp"

struct EnemyUnit {
    int id;
    Enemy enemy;
    int x;
    int y;   
};

class Game {
public:
    Game(int width, int height);

    void CreateEmptyWorld(int width, int height);
    void InitializeWorld();
    void PopulateLevelRandom(int level);
    void GenerateNextLevel(int level);
    void NextTurn();

    void Cleanup();
    bool InBounds(int x, int y) const;
    std::pair<int, int> GetRandomEmptyCell();

    int getLevel() const noexcept { return level_; }
    int& getNextEnemyId() noexcept { return next_enemy_id_; }
    void SetLevel(int lvl) { level_ = lvl; }
    void IncreaseLevel() { ++level_; }

    void RewardPlayer(RewardType type);
    bool AllEnemiesDefeated() const;
    void AddTrap(int x, int y, int damage);
    void AddAlly(int x, int y);

    GameMove& getMove() { return move_; }  
    Field& getField() noexcept { return field_; }
    const Field& getField() const noexcept { return field_; }
    Player& getPlayer() noexcept { return player_; }
    const Player& getPlayer() const noexcept { return player_; }
    std::vector<EnemyUnit>& getEnemies() noexcept { return enemies_; }
    std::vector<EnemyBuilding>& getBuildings() noexcept { return buildings_; }
    std::vector<EnemyTower>& getTowers() noexcept { return towers_; }
    std::vector<std::unique_ptr<Ally>>& getAllies() noexcept { return allies_; }
    std::vector<std::unique_ptr<Trap>>& getTraps() noexcept { return traps_; }
private:
    void RemoveDeadAllies(std::vector<std::unique_ptr<Ally>>& allies);
    void RemoveDeadEnemies(std::vector<EnemyUnit>& enemies);

    Field field_;
    Player player_;
    GameMove move_;
        
    int initialWidth_;
    int initialHeight_;

    std::vector<EnemyUnit> enemies_; 
    std::vector<EnemyBuilding> buildings_;
    std::vector<EnemyTower> towers_;
    std::vector<std::unique_ptr<Ally>> allies_;
    std::vector<std::unique_ptr<Trap>> traps_; 

    int next_enemy_id_ = 1;
    int level_ = 1;
    int no_combat_turns_ = 0;
    int turnNumber_ = 0;
};
