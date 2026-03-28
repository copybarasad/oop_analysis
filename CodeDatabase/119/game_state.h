#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <memory>
#include "spell.h"

struct PlayerState {
    int posX, posY;
    int hp, xp, maxHp;
    int handMaxSize, spellCount;
    std::vector<std::pair<int, int>> spells; // typeId, dmg
};

struct EnemyState {
    int posX, posY;
    int hp;
    bool isAlive;
};

struct GameStateData {
    int currentLevel;
    std::vector<std::vector<int>> field;
    int fieldWidth, fieldHeight;
    PlayerState player;
    EnemyState enemy;
    EnemyState enemy2;
    EnemyState enemyTower;
};

#endif
