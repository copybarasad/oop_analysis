#pragma once
#include "Position.h"
#include "Player.h"
#include <vector>
#include <string>
#include <cstdint>
#include <cstring>

enum class SavedSpellType : uint8_t {
    NONE = 0,
    MAGIC_MISSILE = 1,
    FIREBALL = 2,
    TRAP = 3
};

struct SavedEnemy {
    Position position;
    int health;
    int damage;
    
    SavedEnemy() : position(0, 0), health(0), damage(0) {}
    SavedEnemy(Position pos, int h, int d) : position(pos), health(h), damage(d) {}
};

struct SavedTower {
    Position position;
    int health;
    int maxHealth;
    int damage;
    int range;
    int cooldown;
    
    SavedTower() : position(0, 0), health(0), maxHealth(0), damage(0), range(0), cooldown(0) {}
    SavedTower(Position pos, int h, int mh, int d, int r, int c) 
        : position(pos), health(h), maxHealth(mh), damage(d), range(r), cooldown(c) {}
};

struct SavedTrap {
    Position position;
    int damage;
    bool active;
    
    SavedTrap() : position(0, 0), damage(0), active(false) {}
    SavedTrap(Position pos, int d, bool a) : position(pos), damage(d), active(a) {}
};

struct SavedObstacle {
    Position position;
    
    SavedObstacle() : position(0, 0) {}
    SavedObstacle(Position pos) : position(pos) {}
};

struct SaveData {
    static const int SAVE_VERSION = 1;
    int version;
    int fieldWidth;
    int fieldHeight;
    
    Position playerPosition;
    int playerHealth;
    int playerMaxHealth;
    AttackType playerAttackType;
    int enemiesKilled;
    int enemiesKilledSinceLastSpell;
    
    std::vector<SavedSpellType> spells;
    
    std::vector<SavedEnemy> enemies;
    std::vector<SavedTower> towers;
    std::vector<SavedTrap> traps;
    std::vector<SavedObstacle> obstacles;
    
    int turnCount;
    
    SaveData() : version(SAVE_VERSION), fieldWidth(15), fieldHeight(15),
                 playerPosition(0, 0), playerHealth(100), playerMaxHealth(100),
                 playerAttackType(AttackType::MELEE), enemiesKilled(0),
                 enemiesKilledSinceLastSpell(0), turnCount(0) {}
    
    size_t getSerializedSize() const;
    void serialize(char* buffer) const;
    bool deserialize(const char* buffer, size_t size);
};