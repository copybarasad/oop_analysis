#include "TrapSpell.h"
#include "GameField.h"

TrapSpell::TrapSpell() 
    : name("Ловушка"),
      description("Размещает ловушку, которая наносит 15 урона при активации"),
      damage(15), range(3) {}

bool TrapSpell::canCast(const Position& casterPos, const Position& targetPos, const GameField& field) const {
    if (!field.isPositionValid(targetPos)) return false;
    
    const Cell& cell = field.getCell(targetPos);
    return cell.isPassable() && !cell.isOccupied() && 
           PositionUtils::calculateDistance(casterPos, targetPos) <= range;
}

void TrapSpell::cast(const Position& casterPos, const Position& targetPos, GameField& field) {
    // Устанавливаем ловушку на клетку
    Cell& cell = field.getCell(targetPos);
    cell.setType(CellType::JOCKER);
    
    (void)casterPos;
}