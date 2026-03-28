#ifndef COMBAT_SYSTEM_H
#define COMBAT_SYSTEM_H

#include "game_objects.h"
#include "player.h"
#include "field.h"
#include "freezy_system.h"
class CombatSystem {
public:
    static void attack(GameObject* attacker, GameObject* target);
    static bool player_attack(Player* player, Field& field, int dx, int dy);

private:
    static bool is_range(GameObject* attacker, GameObject* target);
    static int calculate_damage(GameObject* attacker, GameObject* target);
};

#endif