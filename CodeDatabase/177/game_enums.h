#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

enum class CombatMode {
    MELEE,
    RANGED
};

enum class CellType {
    EMPTY,
    OBSTACLE,
    SLOW
};

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum class GameState {
    PLAYING,
    LEVEL_COMPLETE,
    GAME_OVER,
    VICTORY
};

struct LevelConfig {
    int level;
    int fieldWidth;
    int fieldHeight;
    int enemyHealth;
    int enemyDamage;
    int playerHealth;
    int enemyCount;
    int buildingCount;
};

#endif