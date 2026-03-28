#include "UpgradeSpellContext.h"
#include "IUpgradeSpell.h" 

void UpgradeSpellContext::visit(IDamageSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

void UpgradeSpellContext::visit(ISpawnSpell& spell){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}

void UpgradeSpellContext::visit(IUpgradeSpell& spell){
    spell.apply(*this);
}