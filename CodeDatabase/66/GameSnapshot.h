#ifndef GAMESNAPSHOT_H
#define GAMESNAPSHOT_H

#include "SpellType.h"
#include "Position.h"
#include "CellType.h"
#include "CombatMode.h"
#include "Direction.h"
#include "GameMode.h"
#include <vector>
#include <string>

struct PlayerSnapshot {
    int health;
    int maxHealth;
    int meleeDamage;
    int rangedDamage;
    int score;
    CombatMode combatMode;
    bool isSlowed;
    Position position;
};

struct EnhancementSnapshot {
    int fireballEnhancements;
    int meteorEnhancements;
    int trapEnhancements;
    int summonEnhancements;
};

struct SpellSnapshot {
    SpellType type;
    int baseDamage;
    int baseRange;
    int baseAreaSize;
    int baseSummonCount;
};

struct EnemySnapshot {
    Position position;
    int health;
    int damage;
    std::vector<Direction> patrolPattern;
};

struct BuildingSnapshot {
    Position position;
    int health;
    int turnsUntilSpawn;
};

struct TowerSnapshot {
    Position position;
    int health;
    int cooldownRemaining;
};

struct AllySnapshot {
    Position position;
    int health;
    int damage;
};

struct TrapSnapshot {
    Position position;
    int damage;
};

struct WorldSnapshot {
    int width;
    int height;
    std::vector<std::vector<CellType>> grid;
    std::vector<EnemySnapshot> enemies;
    std::vector<BuildingSnapshot> buildings;
    std::vector<TowerSnapshot> towers;
    std::vector<AllySnapshot> allies;
    std::vector<TrapSnapshot> traps;
};

struct PlayerProgress {
    int experiencePoints;
    int level;
    int totalEnemiesKilled;
    int totalDamageDealt;
    
    PlayerProgress() : experiencePoints(0), level(1), 
                       totalEnemiesKilled(0), totalDamageDealt(0) {}
};

struct GameSnapshot {
    int version;
    int turnNumber;
    int currentLevel;
    GameMode gameMode;
    
    PlayerSnapshot player;
    PlayerProgress progress;
    EnhancementSnapshot enhancements;
    std::vector<SpellSnapshot> spells;
    WorldSnapshot world;
    
    GameSnapshot() : version(1), turnNumber(0), currentLevel(1), gameMode(GameMode::CUSTOM) {}
};

#endif
