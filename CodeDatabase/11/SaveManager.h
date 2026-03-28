#ifndef SAVEMANAGER_H
#define SAVEMANAGER_H

#include <string>
#include <vector>
#include "EventLogger.h"

class Player;
class Field;
class Enemy;
class EnemySpawner;
class EnemyTower;

class SaveManager {
public:
    static bool save(const std::string& filename, 
                     int level,
                     const Player& player, 
                     const Field& field,
                     const std::vector<Enemy>& enemies,
                     const std::vector<EnemySpawner>& spawners,
                     const std::vector<EnemyTower>& towers,
                     EventLogger& logger);
    static bool load(const std::string& filename, 
                     int& level,
                     Player& player, 
                     Field& field,
                     std::vector<Enemy>& enemies,
                     std::vector<EnemySpawner>& spawners,
                     std::vector<EnemyTower>& towers,
                     EventLogger& logger);
};
#endif