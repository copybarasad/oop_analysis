#pragma once
#include "LevelManager/LMtypes.hpp"
#include "types.hpp"
#include "Serialization/JsonManager.hpp"
#include "GameObjects/Spells/Spell.hpp"

class LevelManager {
private:
    JsonManager JM;
public:
    MapDTO loadLevelMap(int level);
    std::vector<TowerDTO> loadLevelTowers(int level);
    std::vector<EnemyDTO> loadLevelEnemies(int level);
    StatisticDTO loadLevelStatistic(int level);

    HandDTO loadHand(int level);
    PlayerDTO loadPlayer(int level);
    std::vector<SpellContract> loadTraps();


    void saveLevelMap(MapDTO& map);
    void saveLevelTowers(std::vector<TowerDTO>& towers);
    void saveLevelEnemies(std::vector<EnemyDTO>& enemies);
    void saveLevelStatistic(StatisticDTO& stat);

    void savePlayer(PlayerDTO& player);
    void saveHand(HandDTO& hand);
    void saveTraps(std::vector<SpellContract>& traps);
    
};
