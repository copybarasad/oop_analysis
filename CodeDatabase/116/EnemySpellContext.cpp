#include "EnemySpellContext.h"
#include "IDamageSpell.h"
#include "ISpawnSpell.h"
#include "IUpgradeSpell.h"

void EnemySpellContext::visit(IDamageSpell& spell){
    spell.apply(*this);
    results.damage = spell.getDamage();
}

void EnemySpellContext::visit(ISpawnSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

void EnemySpellContext::visit(IUpgradeSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

Character* EnemySpellContext::getTarget(std::pair<int,int> target){
    if(target.first < 0 || target.first >= world.getField().getWidth() || target.second < 0 || target.second >= world.getField().getHeight()){
        return nullptr;
    }
    auto type = world.getField().getCell(target.first,target.second).getType();
    if(type == Cell::CellType::PLAYER || type == Cell::CellType::ALLY){
        switch (type)
        {
        case Cell::CellType::PLAYER:
            results.woundedPlayers += 1;
            break;
        case Cell::CellType::ALLY:
            results.woundedAllies += 1; 
            break;
        default:
            break;
        }
        return world.getHero(target);
    }
    else{
        return nullptr;
    }
}