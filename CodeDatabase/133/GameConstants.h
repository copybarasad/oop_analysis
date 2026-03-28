#ifndef GAME_CONSTANTS_H
#define GAME_CONSTANTS_H

namespace GameConstants {
    constexpr int MIN_FIELD_SIZE = 10;
    constexpr int MAX_FIELD_SIZE = 40;

    constexpr int DEFAULT_PLAYER_HEALTH = 100;
    constexpr int DEFAULT_PLAYER_MELEE_DAMAGE = 10;
    constexpr int DEFAULT_PLAYER_RANGED_DAMAGE = 5;

    constexpr int DEFAULT_ENEMY_HEALTH = 10;
    constexpr int DEFAULT_ENEMY_DAMAGE = 10;

    constexpr int DEFAULT_ALLY_HEALTH = 30;
    constexpr int DEFAULT_ALLY_DAMAGE = 5;

    constexpr int IMPASSABLE_PERCENTAGE = 10;
    constexpr int SLOWING_PERCENTAGE = 6;

    constexpr int DEFAULT_SPAWN_TURNS = 3;
    constexpr int DEFAULT_SPAWN_HEALTH = 10;
    constexpr int DEFAULT_SPAWN_DAMAGE = 6;

    constexpr int DEFAULT_TRAP_DAMAGE = 20;

    constexpr int ENEMY_DEFEAT_FOR_SPELL = 5;
}

#endif