#pragma once
#include <string>
#include <vector>

struct SavedEnemy {
    int hp = 0;
    int damage = 0;
    int x = 0;
    int y = 0;
};

struct SavedTower {
    int hp = 0;
    int damage = 0;
    int x = 0;
    int y = 0;
    int range = 0;
    int cooldown = 0;
};

struct SavedTrap {
    int x = 0;
    int y = 0;
    int damage = 0;
};

struct SaveData {
    int fileVersion = 2;
    bool hasActiveGame = false;
    int currentLevelIndex = 0;
    int baseWidth = 0;
    int baseHeight = 0;
    double baseWallDensity = 0.0;
    int baseEnemyCount = 0;
    int baseEnemyHp = 0;
    int baseEnemyDamage = 0;
    int boardWidth = 0;
    int boardHeight = 0;
    double wallDensity = 0.0;
    std::vector<int> boardCells;
    std::string playerName;
    int playerCurrentHealth = 0;
    int playerMaxHealth = 0;
    int playerMeleeDamage = 0;
    int playerRangedDamage = 0;
    int playerRangedRange = 0;
    int playerScore = 0;
    int playerMode = 0;
    int playerX = 0;
    int playerY = 0;
    int killsSinceSpell = 0;
    std::vector<std::string> serializedSpells;
    std::vector<SavedEnemy> enemies;
    std::vector<SavedTower> towers;
    std::vector<SavedTrap> traps;
    int spellPowerBonus = 0;
    std::string spellRngState;
    std::string controllerRngState;
};

class SaveManager {
public:
    explicit SaveManager(const std::string& fileName);

    void save(const SaveData& data) const;
    SaveData load() const;

private:
    std::string fileName_;
};
