#include "cell.hpp"

char Cell::Symbol() const {
    switch (this->type_) {
        case CellType::kEmpty:  return '.';
        case CellType::kPlayer: return 'P';
        case CellType::kEnemy:  return 'E';
        case CellType::kWall:   return '#';
        case CellType::kPotionSmall: return '+';
        case CellType::kPotionLarge: return '*';
        case CellType::kEnemyBuilding: return '^';
        case CellType::kSlowCell: return '~';
        case CellType::kTrap: return '@';
        case CellType::kEnemyTower:    return 'B'; 
        case CellType::kAlly: return 'A';
    }
}
