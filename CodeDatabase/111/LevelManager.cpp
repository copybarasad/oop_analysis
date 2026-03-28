#include "LevelManager.h"
#include "ExceptionHandler.h"
#include <random>
#include <algorithm>

LevelManager::LevelManager() : currentLevel(1), rng(std::random_device{}()) {
    initializeLevelConfigs();
}

void LevelManager::initializeLevelConfigs() {
    levelConfigs = {
        // Level 1 - Easy
        {8, 8, 2, 1, 3, 30, 5, 50, 10, 3},
        // Level 2 - Medium  
        {10, 10, 4, 2, 6, 40, 8, 60, 15, 4},
        // Level 3 - Hard
        {12, 12, 6, 3, 9, 50, 12, 70, 20, 5},
        // Level 4 - Very Hard
        {14, 14, 8, 4, 12, 60, 15, 80, 25, 6},
        // Level 5 - Boss
        {16, 16, 10, 5, 15, 70, 18, 100, 30, 7}
    };
}

void LevelManager::initializeLevel(int level, GameBoard& board, Hero& hero, 
                                 std::vector<Foe>& enemies, std::vector<EnemyTower>& towers) {
    if (level < 1 || level > static_cast<int>(levelConfigs.size())) {
        throw GameException("Invalid level number: " + std::to_string(level));
    }
    
    currentLevel = level;
    const LevelConfig& config = levelConfigs[level - 1];
    
    board = GameBoard(config.width, config.height);
    board.generateRandomObstacles(config.obstacleCount, config.obstacleCount);
    
    enemies.clear();
    towers.clear();
    
    generateLevelLayout(config, board, enemies, towers);
    
    hero.setPosition(Location(0, 0));
}

void LevelManager::generateLevelLayout(const LevelConfig& config, GameBoard& board,
                                     std::vector<Foe>& enemies, std::vector<EnemyTower>& towers) {
    std::vector<Location> usedLocations;
    
    usedLocations.push_back(Location(0, 0));
    
    for (int i = 0; i < config.enemyCount; ++i) {
        Location enemyPos = findValidSpawnLocation(board, usedLocations);
        if (enemyPos.isValid()) {
            enemies.emplace_back(config.enemyHitpoints, config.enemyAttack, enemyPos);
            board.addActor(&enemies.back());
            usedLocations.push_back(enemyPos);
        }
    }
    
    for (int i = 0; i < config.towerCount; ++i) {
        Location towerPos = findValidSpawnLocation(board, usedLocations);
        if (towerPos.isValid()) {
            towers.emplace_back(config.towerHitpoints, config.towerAttack, towerPos, config.towerRange);
            board.addActor(&towers.back());
            usedLocations.push_back(towerPos);
        }
    }
}

Location LevelManager::findValidSpawnLocation(GameBoard& board, const std::vector<Location>& exclude) {
    std::uniform_int_distribution<size_t> rowDist(1, board.getWidth() - 1);
    std::uniform_int_distribution<size_t> colDist(1, board.getHeight() - 1);
    
    const int maxAttempts = 100;
    for (int attempt = 0; attempt < maxAttempts; ++attempt) {
        Location loc(rowDist(rng), colDist(rng));
        
        if (board.isLocationValid(loc) && 
            board.isLocationFree(loc) && 
            !board.isObstacle(loc) &&
            std::find(exclude.begin(), exclude.end(), loc) == exclude.end()) {
            return loc;
        }
    }
    
    return Location::invalid();
}

void LevelManager::applyLevelTransitionEffects(Hero& hero) {
    hero.setHitpoints(hero.getMaxHitpoints());
    
    removeRandomSpells(hero.getSpellHand(), 0.5f);
    
    std::cout << "Level transition complete! Health restored, some spells removed.\n";
}

void LevelManager::removeRandomSpells(SpellHand& spellHand, float percentage) {
    if (spellHand.getSpellCount() <= 1) return; 
    
    size_t spellsToRemove = static_cast<size_t>(spellHand.getSpellCount() * percentage);
    if (spellsToRemove == 0) spellsToRemove = 1;
    
    std::vector<size_t> indices;
    for (size_t i = 0; i < spellHand.getSpellCount(); ++i) {
        indices.push_back(i);
    }
    
    std::shuffle(indices.begin(), indices.end(), rng);
    
    std::sort(indices.rbegin(), indices.rend());
    for (size_t i = 0; i < spellsToRemove && i < indices.size(); ++i) {
        spellHand.removeSpell(indices[i]);
    }
}

LevelConfig LevelManager::getCurrentLevelConfig() const {
    if (currentLevel < 1 || currentLevel > static_cast<int>(levelConfigs.size())) {
        throw GameException("Current level out of range");
    }
    return levelConfigs[currentLevel - 1];
}