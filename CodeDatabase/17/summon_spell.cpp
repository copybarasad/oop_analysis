#include "summon_spell.h"
#include "game_map.h"

bool SummonSpell::cast(GameMap& map, Position casterPos, Position targetPos) {
    int distance = abs(casterPos.getX() - targetPos.getX()) + 
                   abs(casterPos.getY() - targetPos.getY());
    
    if (distance > range) { 
        return false;
    }

    if (!map.isPositionValid(targetPos)) {
        return false;
    }

    MapCell& targetCell = map.getCell(targetPos);
    
    if (targetCell.isUsed() || targetCell.getType() == MapCell::Type::WALL) {
        return false;
    }
    return true;
}