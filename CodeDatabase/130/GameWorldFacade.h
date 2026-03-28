#ifndef GAMEWORLDFACADE_H
#define GAMEWORLDFACADE_H

#include "IGameWorldFacade.h"
#include "Position.h"

// Forward declarations
class Player;
class EnemyManager;
class BuildingManager;
class TowerManager;
class TrapManager;
class AllyManager;
class GameField;

class GameWorldFacade : public IGameWorldFacade {
private:
    Player* player_;
    EnemyManager* enemyManager_;
    BuildingManager* buildingManager_;
    TowerManager* towerManager_;
    TrapManager* trapManager_;
    AllyManager* allyManager_;
    GameField* field_;

public:
    GameWorldFacade(Player* player, EnemyManager* enemyManager, 
                   BuildingManager* buildingManager, TowerManager* towerManager,
                   TrapManager* trapManager, AllyManager* allyManager,
                   GameField* field);
    
    bool dealDamageToEnemy(Pos pos, int damage) override;
    bool dealDamageToBuilding(Pos pos, int damage) override;
    bool dealDamageToTower(Pos pos, int damage) override;
    
    bool placeTrap(Pos pos, int damage) override;
    bool summonAlly(Pos near, int count) override;
    
    bool isInRange(Pos from, Pos to, int range) const override;
    Pos getPlayerPosition() const override;
    bool isValidPosition(Pos pos) const override;
};

#endif

