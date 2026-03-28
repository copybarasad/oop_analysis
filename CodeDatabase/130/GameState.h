#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Position.h"
#include "PlayerUpgrade.h"
#include "SpellTypes.h"
#include <string>
#include <vector>
#include <iostream>

struct SpellState {
    SpellType type;
    std::string name;
    
    void serialize(std::ostream& out) const;
    static SpellState deserialize(std::istream& in);
};

// State structures for game entities
struct PlayerState {
    int x, y;
    int health;
    int maxHealth;
    int damage;
    int baseMovementSpeed;
    bool canMove;
    bool isSlowed;
    
    void serialize(std::ostream& out) const;
    static PlayerState deserialize(std::istream& in);
};

struct EnemyState {
    int x, y;
    int health;
    int damage;
    
    void serialize(std::ostream& out) const;
    static EnemyState deserialize(std::istream& in);
};

struct BuildingState {
    int x, y;
    int health;
    int maxHealth;
    int turnCounter;
    int spawnCooldown;
    
    void serialize(std::ostream& out) const;
    static BuildingState deserialize(std::istream& in);
};

struct TowerState {
    int x, y;
    int health;
    int maxHealth;
    int attackRange;
    int attackDamage;
    int attackCooldown;
    int cooldownCounter;
    
    void serialize(std::ostream& out) const;
    static TowerState deserialize(std::istream& in);
};

struct AllyState {
    int x, y;
    int health;
    int damage;
    bool shouldSkip;
    
    void serialize(std::ostream& out) const;
    static AllyState deserialize(std::istream& in);
};

struct TrapState {
    int x, y;
    int damage;
    
    void serialize(std::ostream& out) const;
    static TrapState deserialize(std::istream& in);
};

struct FieldState {
    int width;
    int height;
    std::vector<std::vector<int>> cellTypes; // CellType as int
    
    void serialize(std::ostream& out) const;
    static FieldState deserialize(std::istream& in);
};

// Main game state structure
struct GameState {
    std::string version;
    int currentLevel;
    int currentTurn;
    int enemiesKilled;
    bool hasUsedRevive;
    
    FieldState field;
    PlayerState player;
    std::vector<EnemyState> enemies;
    std::vector<BuildingState> buildings;
    std::vector<TowerState> towers;
    std::vector<AllyState> allies;
    std::vector<TrapState> traps;
    std::vector<SpellState> handSpells;
    std::vector<UpgradeType> upgrades;
    
    void serialize(std::ostream& out) const;
    static GameState deserialize(std::istream& in);
};

#endif

