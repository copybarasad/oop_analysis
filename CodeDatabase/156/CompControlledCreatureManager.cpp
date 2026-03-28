//
// Created by Artem on 02.10.2025.
//

#include "CompControlledCreatureManager.h"
#include "../../../Exceptions/Notifications/SlowingCellNotification.h"

void CompControlledCreatureManager::moveTo(Constants::dxdy dxdy) {
    int nX = this->compControlledCreature->getXCoordinate() + dxdy.x;
    int nY = this->compControlledCreature->getYCoordinate() + dxdy.y;
    this->field.getFieldCells()[nX][nY].addEntityInCell(this->compControlledCreature);
    this->field.getFieldCells()[this->compControlledCreature->getXCoordinate()][this->compControlledCreature->getYCoordinate()].clearCell();
    this->compControlledCreature->setXCoordinate(nX);
    this->compControlledCreature->setYCoordinate(nY);

    if (this->field.getFieldCells()[nX][nY].hasCellEvent()) {
        this->field.getFieldCells()[nX][nY].impactOnCreatureByCellEvent();
        if (this->isCreatureDisabled()) //если ивент задизейблил нашу сущность, то проверяем и останавливаем дальнейшие ходы!
            throw SlowingCellNotification("compControlledCreature on slowing cell!");
    }
}

void CompControlledCreatureManager::attack(Constants::XYPair coordinates) {
    try{
        this->field.getFieldCells()[coordinates.x][coordinates.y].damageEntityInCell(this->compControlledCreature->getDamage());
    }catch (NoEntityInCellNotification &e){
        throw;
    }
}

int CompControlledCreatureManager::getChanceToDetectHostile() noexcept {
    return this->compControlledCreature->getChanceToDetectHostile();
}