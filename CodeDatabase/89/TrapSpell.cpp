#include "TrapSpell.h"
#include "GameGrid.h"
#include "MovementSystem.h"
#include <iostream>

TrapSpell::TrapSpell(int damage) : damage(damage) {}

bool TrapSpell::cast(Player& player, Direction direction,
                     std::vector<std::unique_ptr<Enemy>>& enemies,
                     std::vector<std::unique_ptr<EnemyTower>>& towers,
                     GameGrid& grid) {
    int px = player.getPosition().first;
    int py = player.getPosition().second;

    int tx = px, ty = py;
    switch (direction) {
        case Direction::Up:    ty -= 1; break;
        case Direction::Down:  ty += 1; break;
        case Direction::Left:  tx -= 1; break;
        case Direction::Right: tx += 1; break;
    }

    if (!grid.isValidCoordinate(tx, ty)) {
        std::cout << "Trap placement out of bounds." << std::endl;
        return false;
    }

    if (!grid.isCellEmpty(tx, ty)) {
        std::cout << "Cannot place trap on occupied cell." << std::endl;
        return false;
    }

    grid.updateCell(tx, ty, CellType::Trap);
    std::cout << "Trap placed at (" << tx << ", " << ty << ")" << std::endl;
    return true;
}