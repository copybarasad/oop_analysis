#ifndef ENEMYSPAWNER_HPP
#define ENEMYSPAWNER_HPP

#include "../map/gamemap.hpp"
#include "../entities/tower.hpp"
#include "../entities/magetower.hpp"
#include "../entities/enemy.hpp"
#include "../entities/player.hpp"
#include "magic.hpp"
#include <vector>
#include <memory>

class EnemyManager {
private:
    GameMap& gameMap;
    std::vector<std::unique_ptr<Character>>& allCharacters;
    MagicHand magichand;
public:
    EnemyManager(GameMap& map, std::vector<std::unique_ptr<Character>>& characters);
    void LoadEnemies(json& j);
    Tower* addTower(int x, int y, int cooldown, char type);
    void spawnTowers(int mageCount, int spawnerCount);
    void updateMageTowers(Player* player);
    void spawnInitialEnemies(int count);
    void spawnEnemyFromTower(Tower* tower);
    void updateTowers();
    
private:
    bool findEmptyPosition(int& x, int& y) const;
};

#endif