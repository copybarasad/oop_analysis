#include "trap_spell.h"
#include "game_map.h"

bool TrapSpell::cast(GameMap& map, Position casterPos, Position targetPos) {
    int distance = abs(casterPos.getX() - targetPos.getX()) + 
                   abs(casterPos.getY() - targetPos.getY());
    
    if (distance > range) { 
        return false;
    }

    if (!map.isPositionValid(targetPos)) {
        return false;
    }
    if(map.getCell(targetPos).getType() == MapCell::Type::EMPTY && !map.getCell(targetPos).isUsed()){
        MapCell& cell = map.getCell(casterPos);
        Entity* entity = cell.getEntity();
        map.getCell(targetPos).setType(MapCell::Type::TRAP);
        map.getCell(targetPos).setTrapDamage(damage * entity->getLevel());
        return true;
    }
    return false;

}