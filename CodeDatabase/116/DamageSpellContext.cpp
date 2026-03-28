#include "DamageSpellContext.h"
#include "IDamageSpell.h"
#include "ISpawnSpell.h"
#include "IUpgradeSpell.h"

void DamageSpellContext::visit(IDamageSpell& spell){
    spell.apply(*this);
    results.damage = spell.getDamage();
}

void DamageSpellContext::visit(ISpawnSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

void DamageSpellContext::visit(IUpgradeSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

Character* DamageSpellContext::getTarget(std::pair<int,int> target){
    
    auto type = world.getField().getCell(target.first,target.second).getType();
    if(type == Cell::CellType::ENEMY || type == Cell::CellType::BUILDING || type == Cell::CellType::FORTRESS){
        switch (type)
        {
        case Cell::CellType::ENEMY:
            results.woundedEnemies += 1;
            break;
        case Cell::CellType::BUILDING:
            results.woundedTowers += 1; 
            break;
        case Cell::CellType::FORTRESS:
            results.woundedFortresses += 1;
            break;
        default:
            break;
        }
        return world.getVillain(target);
    }
    else{
        return nullptr;
    }
}

bool DamageSpellContext::isTargetInRadius(int R){
    if(target.first < 0 || target.first >= world.getField().getWidth() || target.second < 0 || target.second >= world.getField().getHeight()){
        return false;
    }
    auto& player = world.getPlayer();
    if(std::sqrt(std::pow(player.getX() - target.first, 2) + std::pow(player.getY() - target.second,2)) > std::sqrt(2 * std::pow(R, 2))){
        return false;
    }
    return true;
}