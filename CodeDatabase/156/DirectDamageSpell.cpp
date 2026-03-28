//
// Created by Artem on 24.10.2025.
//

#include "DirectDamageSpell.h"
#include "../../../EntityType.h"
#include "../../../../Exceptions/Notifications/CantCastSpellOnCellNotification.h"
#include "../../../../Exceptions/CoordinateException.h"
#include "../../../../Logger/Logger.h"
#include <cmath>
#include <iostream>

void DirectDamageSpell::castSpell(int gradeLevel, Field& field, Constants::XYPair from, Constants::XYPair to) const {
    Logger::info("Применяем заклинание урона в точку");
    levelInfo level = this->levels.at(gradeLevel < this->levels.size() ? gradeLevel : (this->levels.size() - 1));

    try{
        field.isCoordsAvailable(to.x, to.y);
    }catch (const CoordinateException &e){
        throw CantCastSpellOnCellNotification("Не получится применить заклинание так как: " + std::string(e.what()));
    }
    int distance = floor(sqrt(pow((from.x - to.x), 2) + pow((from.y - to.y), 2)));
    if (distance > level.range){
        throw CantCastSpellOnCellNotification("Не получиться применить заклинание, цель слишком далеко!");
    }

    {
        std::stringstream ss;
        ss << "Прилёт в точку x: " << to.y+1 << " y: " << to.x+1;
        Logger::info(ss.str());
    }

    if (field.getFieldCells()[to.x][to.y].hasEntityInCell()){
        switch (field.getFieldCells()[to.x][to.y].getEntityInCellType()) {
            case EntityType::EnemyEnt:
            case EntityType::EnemySpawnerBuildingEnt:
            case EntityType::EnemyDefenceTower:
            case EntityType::PlayerEnt:
            case EntityType::Ally:
            {
                field.getFieldCells()[to.x][to.y].damageEntityInCell(level.damage);
                Logger::info("В коориднатах из строки выше прилетело по: " + Constants::entityTypeToString(field.getFieldCells()[to.x][to.y].getEntityInCellType()) + " на " + std::to_string(level.damage) + "hp");
            }
                break;
            default:
                throw CantCastSpellOnCellNotification("По координатам x: " + std::to_string(to.x) + " y: " + std::to_string(to.y) + " нету ничего хорошего (вообще ничего по логике?)");
        }
    }
}

std::unique_ptr<Spell> DirectDamageSpell::clone() const {
    return std::make_unique<DirectDamageSpell>(*this);
}
