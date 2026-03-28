#include "LevelManager/LevelManager.hpp"
#include <iostream>


MapDTO LevelManager::loadLevelMap(int level) {
    char path[29] = "static/levels/?/map.json";

    path[14] = 48 + level;
    MapDTO dto;
    JM.unmarshal(dto, path);

    return dto;
}

std::vector<TowerDTO> LevelManager::loadLevelTowers(int level) {
    char path[33] = "static/levels/?/tower.json";
    path[14] = 48 + level;
    std::vector<TowerDTO> dto;
    JM.unmarshal(dto, path);

    return dto;
}

std::vector<EnemyDTO> LevelManager::loadLevelEnemies(int level) {
    char path[36] = "static/levels/?/enemies.json";
    path[14] = 48 + level;
    std::vector<EnemyDTO> dto;
    JM.unmarshal(dto, path);

    return dto;
}

PlayerDTO LevelManager::loadPlayer(int level) {
    std::string path = "static/levels/player.json";
    if (level == 0) {
        path = "static/levels/0/player.json";
    }
    PlayerDTO dto;
    JM.unmarshal(dto, path.c_str());

    return dto;
}

HandDTO LevelManager::loadHand(int level) {
    std::string path = "static/levels/hand.json";

    if (level == 0) {
        path = "static/levels/0/hand.json";
    }

    HandDTO dto;
    JM.unmarshal(dto, path.c_str());

    return dto;
}

StatisticDTO LevelManager::loadLevelStatistic(int level) {
    char path[33] = "static/levels/?/stat.json";
    path[14] = 48 + level;
    StatisticDTO dto;
    JM.unmarshal(dto, path);

    return dto;
}

std::vector<SpellContract> LevelManager::loadTraps() {
    char path[] = "static/levels/0/traps.json";
    std::vector<SpellContract> traps;
    JM.unmarshal(traps, path);
    return traps;
}



void LevelManager::saveLevelMap(MapDTO& map) {
    char path[] = "static/levels/0/map.json";
    JM.marshal(map, path);
}


void LevelManager::saveLevelTowers(std::vector<TowerDTO>& towers) {
    char path[] = "static/levels/0/tower.json";
    JM.marshal(towers, path);
}


void LevelManager::saveLevelEnemies(std::vector<EnemyDTO>& enemies) {
    char path[] = "static/levels/0/enemies.json";
    JM.marshal(enemies, path);
}


void LevelManager::savePlayer(PlayerDTO& player) {
    char path[] = "static/levels/0/player.json";
    JM.marshal(player, path);
}

void LevelManager::saveHand(HandDTO& hand) {
    char path[] = "static/levels/0/hand.json";
    JM.marshal(hand, path);
}

void LevelManager::saveLevelStatistic(StatisticDTO& stat) {
    char path[] = "static/levels/0/stat.json";
    JM.marshal(stat, path);
}

void LevelManager::saveTraps(std::vector<SpellContract>& traps) {
    char path[] = "static/levels/0/traps.json";
    JM.marshal(traps, path);
}