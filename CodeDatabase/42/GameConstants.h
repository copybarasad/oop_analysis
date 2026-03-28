#pragma once

namespace Constants {
    constexpr int MIN_FIELD_SIZE = 10;
    constexpr int MAX_FIELD_SIZE = 25;
    constexpr int INITIAL_ENEMY_COUNT = 5;

    constexpr int PLAYER_INITIAL_HEALTH = 100;
    constexpr int PLAYER_MELEE_DAMAGE = 20;
    constexpr int PLAYER_RANGED_DAMAGE = 15;
    constexpr int RANGED_ATTACK_RANGE = 3;

    constexpr int ENEMY_HEALTH = 30;
    constexpr int ENEMY_DAMAGE = 10;
    constexpr int ENEMY_DETECTION_RANGE = 5;

    constexpr char EMPTY_CELL = '.';
    constexpr char PLAYER_CELL = 'P';
    constexpr char ENEMY_CELL = 'E';
    constexpr char OBSTACLE_CELL = '#';
    constexpr char TOWER_CELL = 'T';
    constexpr char TRAP_CELL = 'S';

    constexpr double OBSTACLE_PERCENTAGE = 0.1;
    constexpr int MIN_OBSTACLES = 3;
    constexpr int MAX_OBSTACLES = 25;

    constexpr int PLAYER_HAND_SIZE = 5;
    constexpr int ENEMIES_KILLED_FOR_SPELL = 2;

    constexpr int TOWER_HEALTH = 50;
    constexpr int TOWER_DAMAGE = 15;
    constexpr int TOWER_RANGE = 2;
}