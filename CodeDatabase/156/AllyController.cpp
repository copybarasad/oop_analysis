//
// Created by Artem on 23.10.2025.
//

#include "AllyController.h"

void AllyController::doMove(GameMasterBase& gameMaster) {
    this->computeAndDoMove(typesToAttack, priorityOfAttack, gameMaster);
}
