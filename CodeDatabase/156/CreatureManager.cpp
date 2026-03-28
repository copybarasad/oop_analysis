#include "CreatureManager.h"

int CreatureManager::getStepRange() const noexcept {
    return this->creature->getStepRange();
}

bool CreatureManager::isCreatureDisabled() const noexcept {
    return this->creature->isDisabled();
}

void CreatureManager::enableCreature() noexcept {
    this->creature->setDisabledFlag(false);
}

EntityType CreatureManager::getEntityType() noexcept {
    return this->creature->getType();
}
