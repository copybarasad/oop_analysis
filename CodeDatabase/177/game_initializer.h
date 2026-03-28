#ifndef GAME_INITIALIZER_H
#define GAME_INITIALIZER_H

#include "player.h"
#include "game_field.h"
#include "enemy_manager.h"
#include "building_manager.h"
#include "tower.h"
#include <vector>
#include <random>

class GameInitializer {
private:
    Player& player;
    GameField& field;
    EnemyManager& enemyManager;
    BuildingManager& buildingManager;
    std::vector<Tower>& towers;
    
    Position findValidSpawnPosition() const;
    void initializeTowersForLevel(int level);
    
public:
    GameInitializer(Player& player, GameField& field, 
                   EnemyManager& enemyManager, BuildingManager& buildingManager,
                   std::vector<Tower>& towers);
    
    void initializeGame();
    void initializeGameWithConfig(int enemyCount, int buildingCount, 
                                 int enemyHealth = 20, int enemyDamage = 10);
};

#endif