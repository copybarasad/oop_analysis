#include "i_levelup_spell.h"
#include "game_map.h"

bool ILevelUpSpell::cast(GameMap& map, Position casterPos, Position targetPos) {
    MapCell& cell = map.getCell(casterPos);
    Entity* entity = cell.getEntity();
    entity->levelUp();
    return true;
}