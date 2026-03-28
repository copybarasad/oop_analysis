#include "EnhancingSpell.h"

std::string EnhancingSpell::getName(){return std::string("Заклинание улучшения(5)");}

void EnhancingSpell::cast(ISpellContext& context){
    context.visit(*this);
}

void EnhancingSpell::apply(UpgradeSpellContext& context){
    ISpell* spell = context.getSpell();
    for(int i = 0; i < lvl + 1; i++){
        if(spell){
            spell->upgradeCard();
            spellWasUsed = true;
        }
    }
}