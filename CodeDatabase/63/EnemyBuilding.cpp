#include "EnemyBuilding.h"
#include "SummonSpell.h"

EnemyBuilding::EnemyBuilding(unsigned int h, unsigned int d, unsigned int castDiameter, unsigned int cooldown)
        : Entity(h, d, EntityTeam::ENEMY, 'B', 5, 1) {
    spellsHand.addSpell(std::make_unique<SummonSpell>(castDiameter, cooldown));
}

EnemyBuilding::EnemyBuilding(json &data) : Entity(data, EntityTeam::ENEMY) {}

std::unique_ptr<Entity> EnemyBuilding::clone() const {
    return std::make_unique<EnemyBuilding>(*this);
}

StepInfo EnemyBuilding::move(StepRequirements& arguments) {
    auto* spell = spellsHand.getFirstReadySpell();
    if (!spell) return {StepType::WALK, SUCCESSFUL_STEP};
    spellsHand.selectSpell(0);

    auto [x, y] = getPosition();
    auto [tx, ty] = arguments.enemyPosition;
    auto [dx, dy] = arguments.wayToEnemy;
    if (tx - x == dx && ty - y == dy)
        return {StepType::WALK, SUCCESSFUL_STEP};
    if (dx == dy)
        return {StepType::WALK, SUCCESSFUL_STEP};
    std::pair<int,int> target = {x + dx, y + dy};
    return {StepType::SPELL, target};
}

void EnemyBuilding::onEnter(Entity *aggressor) { spellsHand.reload(); }
