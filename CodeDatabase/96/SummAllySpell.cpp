#include <vector>
#include <utility>
#include "SummAllySpell.h"
#include "AllyManager.h"

bool SummAllySpell::Cast(SpellContext& ctx) {
    bool result = 0;
    for(int i = 0; i < numAllies; ++i) {
        std::pair<int,int> spawnPos = ctx.field.FindFreeAdjacentPosition(ctx.playerPos);
        if(spawnPos != ctx.playerPos) {
            ctx.allies.AddAllyAtPos(100, 15, spawnPos);
            ctx.field.SetCellOccupied(spawnPos, true);
            result = 1;
        }
    }
    return result;
}

void SummAllySpell::enhance() {
    numAllies++;
}

std::shared_ptr<dataSpell> SummAllySpell::Save(){
    auto data = std::make_shared<summAllySpell>();
    data->type = SpellType::summAlly;
    data->numAllies = numAllies;
    return data;
}