#ifndef CONSTANTS_H
#define CONSTANTS_H

// БАЗОВЫЕ КОНСТАНТЫ ИГРЫ
constexpr int DEFAULT_FIELD_SIZE = 15;

constexpr int DEFAULT_ENEMY_HEALTH = 15;
constexpr int DEFAULT_ENEMY_DAMAGE = 10;

constexpr int DEFAULT_PLAYER_HEALTH = 150;
constexpr char DEFAULT_FIGHTING_STYLE = 'm';
constexpr int DEFAULT_PLAYER_MELEE_DAMAGE = 15;
constexpr int DEFAULT_PLAYER_RANGED_DAMAGE = 10;
constexpr int DEFAULT_HAND_SIZE = 8;
constexpr int KILL_COUNT_FOR_SPELL = 3;

constexpr int DEFAULT_MELEE_ATTACK_RANGE = 1;
constexpr int DEFAULT_RANGED_ATTACK_RANGE = 4;

constexpr int DEFAULT_BARRACK_HEALTH = 45;
constexpr int DEFAULT_BARRACK_DAMAGE = 0;
constexpr int DEFAULT_SPAWN_INTERVAL = 4;

//КОНСТАНТЫ ДЛЯ ЗАКЛИНАНИЙ
constexpr int DEFAULT_SPELL_DIRECT_DAMAGE = 40;
constexpr int DEFAULT_SPELL_DIRECT_RANGE = 4;

constexpr int DEFAULT_SPELL_AOE_DAMAGE = 25;
constexpr int DEFAULT_SPELL_AOE_AREA_SIZE = 2;
constexpr int DEFAULT_SPELL_AOE_RANGE = 5;

constexpr int DEFAULT_TRAP_DAMAGE = 40;

//БАШНЯ
constexpr int  DEFAULT_TOWER_HEALTH = 30;
constexpr int  DEFAULT_TOWER_DAMAGE = DEFAULT_SPELL_DIRECT_DAMAGE/2; 
constexpr int  DEFAULT_TOWER_ATTACK_RANGE = 2;
constexpr int  DEFAULT_TOWER_ATTACK_COOLDOWN = 4;

// Очки опыта за врагов/башни
constexpr int DEFAULT_ENEMY_POINTS = 20;
constexpr int DEFAULT_BUILDING_POINTS = 50;

// Константы для генерации поля
constexpr double OBSTACLE_PERCENTAGE = 0.1;
constexpr double SLOW_PERCENTAGE = 0.1;

// Константы для инициализации игры
constexpr int INITIAL_ENEMIES = 3;
constexpr int INITIAL_BARRACKS = 2;
constexpr int INITIAL_TOWERS = 2;
constexpr int MIN_SPAWN_DISTANCE = 5;
constexpr int TOTAL_LEVELS = 3;

#endif