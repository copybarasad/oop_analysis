#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "entity.h"
#include "player.h"
#include "field.h"

class CombatSystem {
public:
    static bool dealDamage(Field& field, int targetId, int damage, int sourceId);
    
    static void handleEntityDeath(Field& field, int entityId, int killerId);
    
    static void processKillRewards(Field& field, Player& player);

private:
    static const int KILLS_FOR_SPELL = 5;
    static int killCounter_;
};

#endif