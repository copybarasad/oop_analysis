#pragma once

#include "Cell.h"
#include "BattleType.h"
#include "Spell.h"
#include "Enhancement.h"
#include <vector>

struct SpellState {
    SpellType type;
    std::vector<int> parameters;
};

struct PlayerState {
    int x;
    int y;
    int health;
    int meleeDamage;
    int rangedDamage;
    int score;
    int handCapacity;
    BattleType battleType;
    bool skipTurn;
    std::vector<SpellState> hand;
};

struct EnemyState {
    int x;
    int y;
    int health;
    int damage;
};

struct BuildingState {
    int x;
    int y;
    int spawnCooldown;
    int currentCooldown;
};

struct TowerState {
    int x;
    int y;
    int radius;
    int damage;
    int cooldown;
    int currentCooldown;
};

struct TrapState {
    int x;
    int y;
    int damage;
    bool active;
};

struct AllyState {
    int x;
    int y;
    int health;
    int damage;
};

struct GameSnapshot {
    int currentLevel;
    int fieldWidth;
    int fieldHeight;
    int enemiesKilled;
    PlayerState player;
    std::vector<std::vector<CellType> > cells;
    std::vector<EnemyState> enemies;
    std::vector<BuildingState> buildings;
    std::vector<TowerState> towers;
    std::vector<TrapState> traps;
    std::vector<AllyState> allies;
    std::vector<Enhancement> pendingEnhancements;
};
