//
// Created by Artem on 02.10.2025.
//

#include "PlayerManager.h"
#include "../../../Exceptions/Notifications/SlowingCellNotification.h"
#include "../../../Logger/Logger.h"

void PlayerManager::moveTo(Constants::dxdy dxdy) {
    this->field.getFieldCells()[this->player->getXCoordinate() + dxdy.x][this->player->getYCoordinate() + dxdy.y].addEntityInCell(this->player);
    this->field.getFieldCells()[this->player->getXCoordinate()][this->player->getYCoordinate()].clearCell();
    this->player->setXCoordinate(this->player->getXCoordinate() + dxdy.x);
    this->player->setYCoordinate(this->player->getYCoordinate() + dxdy.y);

    if (this->field.getFieldCells()[this->player->getXCoordinate()][this->player->getYCoordinate()].hasCellEvent()){
        this->field.getFieldCells()[this->player->getXCoordinate()][this->player->getYCoordinate()].impactOnCreatureByCellEvent();
        if (this->isCreatureDisabled())
            throw SlowingCellNotification("Player on slowing cell!");
    }
}

void PlayerManager::attack(Constants::XYPair coords, AttackType type) { //должны гарантировать что мы в выбранной клетке есть сущность для атаки
    if (!this->field.getFieldCells()[coords.x][coords.y].hasEntityInCell())
        throw UnexpectedBehaviorException("In selected coords to attack must be a target to attack! But it nihchego netu");
    switch (type) {
        case AttackType::CloseRange: {
            //в случае с выбранной атакой ближнего боя мы не проверяем условие на то, что противник реально рядом, эта проверка в контроллереSPELLSSPELLS
            this->field.getFieldCells()[coords.x][coords.y].damageEntityInCell(this->player->getCloseRangeAttack().getDamage());
        }
            break;
        case AttackType::LongRange: {
            //также как в случае с ближней аткакой мы гарантируем что координаты в радиусе атаки
            this->field.getFieldCells()[coords.x][coords.y].damageEntityInCell(this->player->getLongRangeAttack().getDamage());
        }
            break;
    }
}

void PlayerManager::changeAttackType() noexcept {
    this->player->changeSelectedAttackType();
}

SpellHand &PlayerManager::getSpellHand() noexcept {
    return this->player->getSpellHand();
}

void PlayerManager::upgradePlayer(UpgradesType type) {
    switch (type) {
        case UpgradesType::UpdateDamage:
            this->player->setLongRangeAttack(LongRangeAttack(this->player->getLongRangeAttack().getDamage() + 1, this->player->getLongRangeAttack().getRange()+1));
            this->player->setCloseRangeAttack(CloseRangeAttack(this->player->getCloseRangeAttack().getDamage() + 2));
            this->player->setScore(this->player->getScore()-1);
            break;
        case UpgradesType::UpdateMovementDistance:
            this->player->setStepRange(this->player->getStepRange()+1);
            this->player->setScore(this->player->getScore()-1);
            break;
        case UpgradesType::UpdateHealth:
            this->player->changeHealthPoints(2);
            this->player->setScore(this->player->getScore()-1);
            break;
    }
}
