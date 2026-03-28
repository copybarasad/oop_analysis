#pragma once
#include "GameObjects/Entities/AttackTower.hpp"
#include "GameObjects/Entities/SpawnTower.hpp"
#include "GameClass/EnemyController.hpp"
#include "GameClass/MapController.hpp"
#include <vector>

class TowerController {
public:
    TowerController(int level, MapController& map);
    std::vector<TowerDTO> toDTO();
    void loadLevel(int level, MapController& map);


    void makeAllStep(MapController& map, EnemyController& ec, Player& player, StatisticDTO& stats);

    AttackTower* getAttackTower(Coordinates pos);
    SpawnTower* getSpawnTower(Coordinates pos);

    void makeActive();
    bool isActive();
private:
    std::unique_ptr<Spell> targetSpell;
    bool needMove;
    LevelManager LM;
    std::vector<AttackTower> attackTowers;
    std::vector<SpawnTower> spawnTowers;
    
    void removeDeadTowers(MapController& map, StatisticDTO& stats);
};
