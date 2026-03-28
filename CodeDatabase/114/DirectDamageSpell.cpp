#include "DirectDamageSpell.h"
#include "Entity.h"
#include "Player.h"
#include <cmath>
#include <iostream>

#define DIRECT_DAMAGE 30

bool DirectDamageSpell::cast(int pendingBoost, Entity* target, ISpellField* field) {
    Player& player = field->getPlayer();
    int dx = std::abs(player.getX() - target->getX());
    int dy = std::abs(player.getY() - target->getY());
    int distance = dx + dy;

    if (distance > 5 * pendingBoost) {
        return false;
    }

    target->takeDamage(DIRECT_DAMAGE);
    return true;
}
