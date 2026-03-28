#include "include/CombatManager.h"
#include "../../entities/include/Entity.h"
#include "../../entities/include/Player.h"

bool CombatManager::damageAndResolve(EntityId attacker, EntityId target, int damage) {
    auto* tgt = ctx_.entity(target);
    if (!tgt) return false;
    tgt->takeDamage(damage);
    if (!tgt->isAlive()) {
        // Награда: если атакующий — игрок и цель враг/башня/здание
        auto* att = ctx_.entity(attacker);
        if (att && att->type() == EntityType::Player) {
            auto* p = static_cast<Player*>(att);
            p->giveKillReward();
        }
        return ctx_.remove(target);
    }
    return true;
}
