#include "LevelsManager.hpp"

LevelsManager::~LevelsManager () {
    for (auto& [index, level] : levels) {
        delete level;
    }
    for (auto [index, boost] : boosts) {
        delete boost[0];
        delete boost[1];
        delete boost[2];
    }
}

void LevelsManager::addLevel (size_t index, GameLevel* level) { levels[index] = level; }

void LevelsManager::addBoost (size_t level, size_t index, Item* boost) { boosts[level][index] = boost; }
    
GameLevel& LevelsManager::getLevel (size_t index) { return *levels[index]; }

std::map<size_t, GameLevel*> LevelsManager::getLevels () { return levels; }

std::map<size_t, std::vector<Item*>> LevelsManager::getBoosts () { return boosts; }