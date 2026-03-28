//
// Created by Artem on 08.10.2025.
//

#include "EnemyController.h"

void EnemyController::doMove(GameMasterBase& gameMaster) {
    this->computeAndDoMove(typesToAttack, priorityOfAttack, gameMaster);
}

