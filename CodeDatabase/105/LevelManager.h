#pragma once

#include "Field.h"
#include "Player.h"
#include "GameVisualize.h"
#include <random>

class LevelManager {
private:
    int currentLevel;
    int scoreForNextLevel;
    std::mt19937 rng;
    ConsoleGameVisualizer visualizer;

public:
    LevelManager();
    
    int getCurrentLevel() const { return currentLevel; }
    int getScoreForNextLevel() const { return scoreForNextLevel; }
    
    void resetToLevel(int level = 1);
    bool shouldLevelUp(const Player& player, const GameField& field) const;
    void advanceLevel(Player& player, GameField& field);
    void setLevelState(int level, int requiredScore);

    void showLevelUpMenu(Player& player);
    
private:
    int removeRandomSpells(Player& player);
    int calculateLevelScore(int level) const;
    std::pair<int, int> calculateLevelSize(int level) const;
    void setupEnemiesForLevel(GameField& field, int level);
    void upgradeSpell(Player& player);
    void displayPlayerStats(const Player& player);
};
