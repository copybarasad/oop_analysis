#pragma once
#include "Enums.h"
#include "Position.h"
#include "IGameContext.h"

class CombatManager {
public:
    explicit CombatManager(IGameContext& ctx) : ctx_(ctx) {}

    // Нанести урон цели, удалить если умерла, выдать награду игроку (если применимо)
    bool damageAndResolve(EntityId attacker, EntityId target, int damage);

private:
    IGameContext& ctx_;
};
