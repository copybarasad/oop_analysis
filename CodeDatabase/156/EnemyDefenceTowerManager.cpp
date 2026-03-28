//
// Created by Artem on 30.10.2025.
//

#include "EnemyDefenceTowerManager.h"

bool EnemyDefenceTowerManager::isCoolDowned() const noexcept {
    return this->counter == this->tower->getAttackInterval();
}

void EnemyDefenceTowerManager::goToCoolDown() noexcept {
    this->counter = 0;
}

void EnemyDefenceTowerManager::doCoolDownStep() noexcept {
    this->counter = this->isCoolDowned() ? counter : (counter+1);
}

int EnemyDefenceTowerManager::getAttackRadius() const noexcept {
    return this->tower->getAttackRadius();
}

void EnemyDefenceTowerManager::attackCoordinates(Constants::XYPair coords) {
    try {
        this->tower->getDamageSpell().castSpell(0, this->field, this->getEntityCoords(), coords);
    }catch (UniversalStringException& e){
        throw UnexpectedBehaviorException("ошибка в EnemyDefenceTowerManager при касте заклинания, ошибка здесь вызывать не должна, что-то не досмотрел в controller!!!");
    }
}


