#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Hero.hpp"
#include "Field.hpp"
#include "Enemy.hpp"
#include "Ally.hpp"
#include "Building.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include "Trap.hpp"
#include "SpellHand.hpp"
#include <vector>
#include <memory>

struct GameState {
    int currentLevel;
    int score;
    Field field;
    Hero player;
    std::vector<Enemy> enemies;
    std::vector<Ally> allies;
    std::vector<std::unique_ptr<Building>> buildings;
    std::vector<Trap> traps;
    void createLevel(int level);

public:
    GameState(int level = 1);
    void resetForNewLevel(int level);
};

#endif