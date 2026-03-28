#include "AreaDamageSpell.h"
#include "DamageSpellContext.h"

std::string AreaDamageSpell::getName(){return std::string("Заклинание урона по площади(2)");}

void AreaDamageSpell::cast(ISpellContext& context){
    context.visit(*this);
}

void AreaDamageSpell::apply(DamageSpellContext& context){
    auto target = context.getTargetCords();
    if(context.isTargetInRadius(maxR)){
        for(int dx = 0; dx < squareSide; dx++){
            for(int dy = 0; dy < squareSide; dy++){
                auto opponent = context.getTarget({target.first + dx, target.second + dy});
                if(opponent){
                    opponent->takeDamage(damage);
                }
            }
        }
        spellWasUsed = true;
    }
}

void AreaDamageSpell::apply(EnemySpellContext& context){
    throw std::invalid_argument("В карточку заклинания подан неверный контекст!");
}