#include "ActionProcessor.h"
#include <iostream>

void ActionProcessor::processEnemyMoves(GameState& state) {
    physicalWorldSystem.processEnemyMoves(state);
}

bool ActionProcessor::processPlayerMove(GameState& state, int newX, int newY) {
    return physicalWorldSystem.processPlayerMove(state, newX, newY);
}