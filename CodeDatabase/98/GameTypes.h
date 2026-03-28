#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <vector>
#include <string>
#include <utility>

struct EnemyInfo {
    int id;
    int x;
    int y;
    double distance;
    int health;
};

struct AttackResult {
    bool enemyDefeated = false;
    bool playerDefeated = false;
    std::string message;
    int enemyHealthAfter = 0;
    int playerHealthAfter = 0;
};

struct SpellResult {
    bool success = false;
    std::string message;
    int enemiesHit = 0;
};

#endif