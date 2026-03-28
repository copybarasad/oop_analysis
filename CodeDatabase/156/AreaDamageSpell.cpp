//
// Created by Artem on 24.10.2025.
//

#include "AreaDamageSpell.h"
#include "../../../../Exceptions/Notifications/CantCastSpellOnCellNotification.h"
#include "../../../../Exceptions/CoordinateException.h"
#include "../../../../Logger/Logger.h"
#include <cmath>
#include <iostream>

void AreaDamageSpell::castSpell(int gradeLevel, Field &field, Constants::XYPair from, Constants::XYPair to) const {
    Logger::info("Примеяем заклинание атаки по площади!");
    levelInfo level = this->levels.at(this->levels.size() < gradeLevel ? gradeLevel : (this->levels.size() - 1));

    try{
        field.isCoordsAvailable(to.x, to.y);
    }catch (const CoordinateException &e){
        throw CantCastSpellOnCellNotification("Не получится применить заклинание так как: " + std::string(e.what()));
    }
    int distance = ceil(sqrt(pow((from.x - to.x), 2) + pow((from.y - to.y), 2)));
    if (distance > level.range){
        throw CantCastSpellOnCellNotification("Не получиться применить заклинание, цель слишком далеко!");
    }

    for(Constants::dxdy dxdy : damageCoords){
        try{
            int damageX = to.x + dxdy.x, damageY = to.y + dxdy.y;
            field.isCoordsAvailable(damageX, damageY);
            {
                std::stringstream ss;
                ss << "Прилёт по x: " << damageY+1 << " y: " << damageX+1;
                Logger::info(ss.str());
            }

            if(field.getFieldCells()[damageX][damageY].hasEntityInCell()){
                {
                    std::stringstream ss;
                    ss << "В коориднатах из строки выше прилетело по: " << Constants::entityTypeToString(field.getFieldCells()[damageX][damageY].getEntityInCellType());
                    Logger::info(ss.str());
                }

                field.getFieldCells()[damageX][damageY].damageEntityInCell( level.damage);
            }
        }catch (const CoordinateException &e){}
    }
}

std::unique_ptr<Spell> AreaDamageSpell::clone() const {
    return std::make_unique<AreaDamageSpell>(*this);
}