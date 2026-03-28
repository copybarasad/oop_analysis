#include "Enemy.h"
#include "GameArea.h"
bool Enemy::attack(Coords target_coords, GameArea& gamearea)
{
    std::string target_type = gamearea.get_type(target_coords);
    if (target_type == "Player" || target_type == "Summon") {
        return gamearea.apply_damage(target_coords, damage_calculation(), coords, true);
    }
    return false;
}

