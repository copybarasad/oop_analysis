#include "TrapSpell.h"
#include "DamageSpellContext.h"
#include "SpawnSpellContext.h"
#include "EnemySpellContext.h"

std::string TrapSpell::getName(){return std::string("Заклинание ловушки(3)");}

void TrapSpell::cast(ISpellContext& context){
    context.visit(*this);
}

void TrapSpell::apply(SpawnSpellContext& context){
    auto target = context.getTargetCords();
    if(context.isTargetInRadius(maxR) && context.cellIsEmpty(context.getTargetCords())){
        auto& spawner = context.getSpawner();
        spawner.spawnTrap(context.getWorld(),context.getTargetCords().first, context.getTargetCords().second, getCurrentDamage());
        spellWasUsed = true;
    }
}