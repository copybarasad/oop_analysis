#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <algorithm>
#include <fstream>
#include <sstream>

enum class ObjectType {
    Empty = '.',
    Hole = 'O',
    Enemy = 'E',
    Player = 'P',
    Tower = 'T',
    Spike = 'S'
};

#define MAX(a, b) std::max((a), (b))
#define MIN(a, b) std::min((a), (b))
#define WIDTH 10
#define HEIGHT 10
#define ENEMY_HP 50
#define ENEMY_MELEE_DAMAGE 20
#define TOWER_HP 70
#define TOWER_LESS_DAMAGE 10
#define TOWER_GREATER_DAMAGE 30
#define PLAYER_HP 100
#define PLAYER_MELEE_DAMAGE 30
#define PLAYER_RANGE_DAMAGE 20
#define NUMBER_OF_ENEMIES 1
#define NUMBER_OF_TOWERS 0
#define NUMBER_OF_HOLES 7
#define HAND_SIZE 5
#define PLAYER_MELEE_RANGE 1
#define PLAYER_FAR_RANGE 2
#define FIREBALL_DAMAGE 50
#define CHAOSBOLT_LESS_DAMAGE 20
#define CHAOSBOLT_GREATER_DAMAGE 80
#define SPIKE_DAMAGE 50

#endif