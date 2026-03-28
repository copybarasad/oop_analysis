#ifndef SAVE_GAME_H
#define SAVE_GAME_H

#include "game_map.h"
#include "entity_manager.h"
#include "player.h"
#include "enemy.h"
#include "enemy_tower.h"
#include "follower.h"
#include "enemy_building.h"
#include <string>
#include <fstream>
#include <ctime>
#include <sys/stat.h>

class SaveGame {
private:
    std::string filename;

public:
    SaveGame(const std::string& file = "save.bin") : filename(file) {}
    bool save(const GameMap& map, EntityManager* entities);
    bool load(GameMap& map, EntityManager* entities);
    
private:
    bool savePlayer(std::ofstream& file, Player& player);
    bool loadPlayer(std::ifstream& file, EntityManager* entities);
    bool saveMap(std::ofstream& file, const GameMap& map);
    bool loadMap(std::ifstream& file, GameMap& map);
    bool saveEntities(std::ofstream& file, EntityManager* entities);
    bool loadEntities(std::ifstream& file, GameMap& map, EntityManager* entities);
    bool checkFileIntegrity();
};

#endif