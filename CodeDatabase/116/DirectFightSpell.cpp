#include "DirectFightSpell.h"
#include "DamageSpellContext.h"
#include "EnemySpellContext.h"
#include "ISpellContext.h"  

std::string DirectFightSpell::getName(){return std::string("Заклинание прямого боя(1)");}

void DirectFightSpell::cast(ISpellContext& context){
    context.visit(*this);
}

void DirectFightSpell::apply(DamageSpellContext& context){
    auto targetCords = context.getTargetCords();
    if(context.isTargetInRadius(getCurrentR())){
        auto target = context.getTarget(targetCords);
        if(target){
            target->takeDamage(damage);
            spellWasUsed = true;
        }
    }
}

void DirectFightSpell::apply(EnemySpellContext& context){
    auto selfCords = context.getSelfCords();
    std::pair<int, int> startPoint = {selfCords.first - maxR, selfCords.second - maxR};
    for(int dx = 0; dx < 2*maxR + 1; dx++){
        for(int dy = 0; dy < 2*maxR + 1; dy++){
            auto* target = context.getTarget({startPoint.first + dx, startPoint.second + dy});
            if(target){
                target->takeDamage(damage);
            }
        }
    }
}