#include "EnemyTower.h"
#include "DirectDamageSpell.h"

EnemyTower::EnemyTower(unsigned int h, unsigned int d, unsigned int attackDiameter, unsigned int cooldown)
        : Entity(h, d / 2, EntityTeam::ENEMY, 'T', 0, 1) {
    spellsHand.addSpell(std::make_unique<DirectDamageSpell>(attackDiameter, cooldown));
}

EnemyTower::EnemyTower(json &data) : Entity(data, EntityTeam::ENEMY) {}

std::unique_ptr<Entity> EnemyTower::clone() const {
    return std::make_unique<EnemyTower>(*this);
}

StepInfo EnemyTower::move(StepRequirements& arguments) {
    auto* spell = spellsHand.getFirstReadySpell();
    if (!spell) return {StepType::WALK, SUCCESSFUL_STEP};
    spellsHand.selectSpell(0);
    if (arguments.enemyPosition.first == -1)
        return {StepType::WALK, SUCCESSFUL_STEP};
    return {StepType::SPELL, arguments.enemyPosition};
}

void EnemyTower::onEnter(Entity *aggressor) { spellsHand.reload(); }
