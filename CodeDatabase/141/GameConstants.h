#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

namespace GameConstants {
    constexpr int MIN_FIELD_SIZE = 10;
    constexpr int MAX_FIELD_SIZE = 25;

    constexpr int PLAYER_INITIAL_HEALTH = 100;
    constexpr int PLAYER_MELEE_DAMAGE = 20;
    constexpr int PLAYER_RANGED_DAMAGE = 15;
    constexpr int PLAYER_SCORE_INITIAL = 0;

    constexpr int ENEMY_INITIAL_COUNT = 4;
    constexpr int ENEMY_BASE_HEALTH = 20;
    constexpr int ENEMY_MIN_DAMAGE = 7;
    constexpr int ENEMY_MAX_DAMAGE = 14;
    constexpr int ENEMY_SPAWN_ATTEMPTS = 50;

    constexpr int BUILDING_INITIAL_COUNT = 2;
    constexpr int BUILDING_SPAWN_ATTEMPTS = 30;
    constexpr int BUILDING_MIN_COOLDOWN = 3;
    constexpr int BUILDING_MAX_COOLDOWN = 5;

    constexpr double OBSTACLE_PERCENTAGE = 0.15;
    constexpr double SLOW_CELL_PERCENTAGE = 0.1;
    constexpr int MIN_EMPTY_NEIGHBORS = 4;

    constexpr int RANGED_ATTACK_RANGE = 3;
    constexpr int SCORE_PER_KILL = 10;

    constexpr int SPAWN_SAFE_RADIUS = 5;
    constexpr int MAX_SPAWN_ATTEMPTS = 50;

    constexpr int DEFAULT_HAND_SIZE = 5;
    constexpr int SPELL_BASE_RANGE = 3;
    constexpr int SCORE_FOR_NEW_SPELL = 30;
    constexpr int ENEMY_KILLS_FOR_NEW_SPELL = 3;

    constexpr int TRAP_DAMAGE = 25;
    constexpr int TRAP_RANGE = 1;

    constexpr int TOWER_INITIAL_COUNT = 2;
    constexpr int TOWER_ATTACK_RADIUS = 4;
    constexpr int TOWER_DAMAGE = 12;
    constexpr int TOWER_COOLDOWN = 3;
    constexpr int TOWER_SPAWN_ATTEMPTS = 50;

    constexpr int ALLY_BASE_HEALTH = 30;
    constexpr int ALLY_BASE_DAMAGE = 10;

    constexpr int ENHANCE_RANGE_BOOST = 1;
    constexpr int ENHANCE_AREA_BOOST = 1;
    constexpr int ENHANCE_DAMAGE_BOOST = 5;
    constexpr int ENHANCE_SUMMON_BOOST = 1;
}

#endif
