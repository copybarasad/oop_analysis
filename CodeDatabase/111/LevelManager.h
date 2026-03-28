#pragma once
#include "GameBoard.h"
#include "Hero.h"
#include "Foe.h"
#include "EnemyTower.h"
#include "SpellHand.h"
#include <vector>
#include <random>

struct LevelConfig {
    size_t width;
    size_t height;
    int enemyCount;
    int towerCount;
    int obstacleCount;
    int enemyHitpoints;
    int enemyAttack;
    int towerHitpoints;
    int towerAttack;
    int towerRange;
};

class LevelManager {
private:
    int currentLevel;
    std::vector<LevelConfig> levelConfigs;
    std::mt19937 rng;

public:
    LevelManager();
    
    void initializeLevel(int level, GameBoard& board, Hero& hero, 
                       std::vector<Foe>& enemies, std::vector<EnemyTower>& towers);
    
    int getCurrentLevel() const { return currentLevel; }
    void setCurrentLevel(int level) { currentLevel = level; }
    int getTotalLevels() const { return levelConfigs.size(); }
    
    bool canAdvanceToNextLevel() const { return currentLevel < getTotalLevels(); }
    void advanceToNextLevel() { if (canAdvanceToNextLevel()) currentLevel++; }
    
    void applyLevelTransitionEffects(Hero& hero);
    LevelConfig getCurrentLevelConfig() const;

private:
    void initializeLevelConfigs();
    void generateLevelLayout(const LevelConfig& config, GameBoard& board,
                           std::vector<Foe>& enemies, std::vector<EnemyTower>& towers);
    Location findValidSpawnLocation(GameBoard& board, const std::vector<Location>& exclude = {});
    void removeRandomSpells(SpellHand& spellHand, float percentage);
};