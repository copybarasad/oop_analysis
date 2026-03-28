#include <vector>
#include <utility>
#include <memory>
#include "TrapSpell.h"
#include <iostream>


bool TrapSpell::Cast(SpellContext& ctx) {
    std::vector<std::pair<int,int>> result;
    if(!ctx.field.inBounds(ctx.target) || ctx.target == ctx.playerPos) {
        return 0;
    }
    if(ctx.field.GetTrapDamageAtPos(ctx.target)) {
        return 0;
    }
    int dx = ctx.target.first - ctx.playerPos.first;
    dx = dx*dx;
    int dy = ctx.target.second - ctx.playerPos.second;
    dy = dy*dy;
    int r2 = range*range;
    std::cout << dx+dy << ' ' << r2 << '\n';
    if(dx+dy > r2) {
        return 0;
    }
    ctx.field.SetTrapAtPos(ctx.target, damage);
    return 1;
}

void TrapSpell::enhance() {
    damage += 5;
}

std::shared_ptr<dataSpell> TrapSpell::Save(){
    auto data = std::make_shared<trapSpell>();
    data->type = SpellType::trap;
    data->damage = damage;
    data->range = range;
    return data;
}