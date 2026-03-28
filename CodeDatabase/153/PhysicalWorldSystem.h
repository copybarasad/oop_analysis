#ifndef PHYSICALWORLDSYSTEM_H
#define PHYSICALWORLDSYSTEM_H

#include "GameState.h"
#include "GameConstants.h"
#include <iostream>
#include <cstdlib>

class PhysicalWorldSystem {
public:
    bool processPlayerMove(GameState& state, int newX, int newY);

    void processEnemyMoves(GameState& state);

private:
    bool attackEnemyAt(GameState& state, int x, int y);
    bool movePlayerTo(GameState& state, int newX, int newY);
    void moveEnemyRandomly(GameState& state, Enemy& enemy);
    bool enemyCanAttackPlayer(const GameState& state, const Enemy& enemy);
};

#endif



