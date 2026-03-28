#include "TrapSpell.h"
#include "Player.h"
#include "Cell.h"

bool TrapSpell::cast(int, Entity*, ISpellField* field) {
    Player& player = field->getPlayer();
    int x = player.getX();
    int y = player.getY();
    field->getCells()[y][x].setType(CellType::TRAP);
    return true;
}
