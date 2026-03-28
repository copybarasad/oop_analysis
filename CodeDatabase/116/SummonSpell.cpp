#include "SummonSpell.h"

std::string SummonSpell::getName(){return std::string("Заклинание призыва(4)");}

void SummonSpell::cast(ISpellContext& context){
    context.visit(*this);
}

void SummonSpell::apply(SpawnSpellContext& context){
    auto target = context.getTargetCords();
    if(context.isTargetInRadius(maxR) && context.cellIsEmpty(context.getTargetCords())){
        auto& spawner = context.getSpawner();
        spawner.spawnAlly(context.getWorld(),context.getTargetCords().first, context.getTargetCords().second);
        spellWasUsed = true;
    }
    std::pair<int, int> startPoint = {target.first - 1, target.second - 1};
    int bonusAllycount = 0;

    for(int x = 0; x < 3; x++){
        for(int y = 0; y < 3; y++){
            if(context.isTargetInRadius(maxR + 5) && context.cellIsEmpty({startPoint.first + x, startPoint.second + y}) && bonusAllycount < lvl){
                bonusAllycount += 1;
                auto& spawner = context.getSpawner();
                spawner.spawnAlly(context.getWorld(),startPoint.first + x, startPoint.second + y);
                spellWasUsed = true;
            }
        }
    }
}