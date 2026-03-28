#include "DirectDamageSpell.hpp"
#include "../core/GameContext.hpp"
#include "../core/EntityManager.hpp"
#include "../core/FieldCell.hpp"
#include "../entities/Entity.hpp"

bool DirectDamageSpell::cast(GameContext& ctx, int userIndex, int power) {
    if (!countOfItem) {
        return false;
    }
    std::vector<int> allEnemy;
    std::vector<int> enemyIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::ENEMY);
    std::vector<int> barracksIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::BARRACKS);
    int barracksIndex = -1;
    if (!barracksIndexes.empty()) {
        barracksIndex = barracksIndexes[0];
    }
    std::vector<int> towerIndexes = ctx.entityManager.getIndexesWithEntity(Entity::entityType::TOWER);
    int towerIndex = -1;
    if (!towerIndexes.empty()) {
        towerIndex = towerIndexes[0];
    }
    int playerIndex = ctx.entityManager.getIndexesWithEntity(Entity::entityType::PLAYER)[0];
    if (ctx.entityManager[userIndex]->getType() == Entity::entityType::PLAYER) {
        allEnemy = enemyIndexes;
        if (barracksIndex != -1) {
            allEnemy.push_back(barracksIndex);
        }
        if (towerIndex != -1) {
            allEnemy.push_back(towerIndex);
        }
    } else {
        allEnemy.push_back(playerIndex);
    }
    for (int enemyIndex : allEnemy) {
        if (ctx.cells[userIndex].getDistance(ctx.cells[enemyIndex]) <= baseDistance * powerOfSpell + power * 4) {
            Entity* user = ctx.entityManager[userIndex];
            int userInt = user->getInt();
            ctx.entityManager[enemyIndex]->causeDamage((userInt + baseDamage) * (powerOfSpell + userInt/10));
            animateCast(ctx, enemyIndex);
            return true;
        }
    }
    return false;
}


void DirectDamageSpell::animateCast(GameContext& ctx, int target) {
    int upIndex = target - GlobalGameConfig::fieldWidth;
    int downIndex = target + GlobalGameConfig::fieldWidth;
    int leftIndex = target - 1;
    int rightIndex = target + 1;
    if (upIndex >= 0) {
        std::shared_ptr<IState> upCastEffect = std::make_shared<AttackEffect>('&', 0, 4);
        ctx.cells[upIndex].returnCellState().setTemporaryState(move(upCastEffect));
    }
    if (downIndex < GlobalGameConfig::fieldWidth * GlobalGameConfig::fieldHeight) {
        std::shared_ptr<IState> downCastEffect = std::make_shared<AttackEffect>('&', 0, 4);
        ctx.cells[downIndex].returnCellState().setTemporaryState(move(downCastEffect));
    }
    if (leftIndex >= 0) {
        std::shared_ptr<IState> leftCastEffect = std::make_shared<AttackEffect>('&', 0, 4);
        ctx.cells[leftIndex].returnCellState().setTemporaryState(move(leftCastEffect));
    }
    if (rightIndex < GlobalGameConfig::fieldWidth * GlobalGameConfig::fieldHeight) {
        std::shared_ptr<IState> rightCastEffect = std::make_shared<AttackEffect>('&', 0, 4);
        ctx.cells[rightIndex].returnCellState().setTemporaryState(move(rightCastEffect));
    }
}


void DirectDamageSpell::setBaseDamage(int newDamage) {
    baseDamage = newDamage;
}


void DirectDamageSpell::setBaseDistance(int newDistance) {
    baseDistance = newDistance;
}

int DirectDamageSpell::getBaseDistance() {
    return baseDistance;
}

SpellSaveData DirectDamageSpell::getSpellSaveData() {
    SpellSaveData data;
    data.countOfItem = this->countOfItem;
    data.powerOfSpell = this->powerOfSpell;
    data.baseDamage = this->baseDamage;
    data.baseDistance = this->baseDistance;
    data.trapLevel = 0;
    data.trapLevel = 0;
    return data;
}