#include "SpawnSpellContext.h"
#include "IDamageSpell.h"
#include "ISpawnSpell.h"
#include "IUpgradeSpell.h"

void SpawnSpellContext::visit(IDamageSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

void SpawnSpellContext::visit(ISpawnSpell& spell){
    spell.apply(*this);
}

void SpawnSpellContext::visit(IUpgradeSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

bool SpawnSpellContext::isTargetInRadius(int R){
    if(target.first < 0 || target.first >= world.getField().getWidth() || target.second < 0 || target.second >= world.getField().getHeight()){
        return false;
    }
    auto& player = world.getPlayer();
    if(std::sqrt(std::pow(player.getX() - target.first, 2) + std::pow(player.getY() - target.second,2)) > std::sqrt(2 * std::pow(R, 2))){
        return false;
    }
    return true;
}