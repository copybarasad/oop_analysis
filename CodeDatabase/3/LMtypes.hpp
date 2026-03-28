#pragma once
#include <vector>
#include "types.hpp"
#include "GameObjects/Spells/Spell.hpp"

// Для карты
struct FieldPair {
    Coordinates coord;
    FieldType type;
};

struct MapDTO {
    int width;
    int height;
    std::vector<FieldPair> FPs;
};

// Для врагов
struct EnemyDTO {
    Coordinates coord;
    int maxHP;
    int hp;
    int dmg;
    bool disabled;
};

// Для башен
struct TowerDTO {
    int coolDown;
    int steps;
    Coordinates coord;
    bool isAttack;
    int hp;
    int maxHp;
    EnemyDTO enemyStats;
};

// Для игрока
struct PlayerDTO {
    Coordinates coord;
    int maxHP;
    int hp;
    int dmg;
    bool disabled;
    bool rangeForm;
};

// Для каста
struct HandDTO {
    int targetLvl;
    int areaLvl;
    int trapLvl;
    int capacity;
    std::vector<SpellType> spells;
};

struct CastDTO {
    std::vector<SpellContract> traps;
    std::vector<Coordinates> castRange;
    Coordinates target;
    HandDTO hand;
};


// Вся игра

enum class GameState {
    OnWalk,
    OnCast,
    Win,
    Lose,
    Menu,
    Improve
};

struct StatisticDTO {
    int lvl;
    int points;
    int endLevelPoints;
    bool bonus;
};

struct GameDTO {
    GameState state;
    StatisticDTO stats;
    int points;
    MapDTO map;
    PlayerDTO player;
    std::vector<EnemyDTO> enemies;
    std::vector<TowerDTO> towers;
    std::vector<Coordinates> shooted;
    CastDTO cast;
};

