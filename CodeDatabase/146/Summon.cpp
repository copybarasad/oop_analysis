#include "Summon.h"
#include "GameArea.h"

bool Summon::attack(Coords target_coords, GameArea& gamearea)
{
    std::string target_type = gamearea.get_type(target_coords);
    if (target_type == "Enemy" || target_type == "EnemyBuilding" || target_type == "Tower") {
        return gamearea.apply_damage(target_coords, damage_calculation(), coords, true);
    }
    return false;
}

