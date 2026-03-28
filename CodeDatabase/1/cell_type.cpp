#include "cell_type.h"

bool isPassable(CellType cellType) {
    return cellType != CellType::WALL;
}

bool hasSpecialEffect(CellType cellType) {
    return cellType == CellType::CHAPEL ||
           cellType == CellType::VAMPIRE_LAIR ||
           cellType == CellType::ARMORY ||
           cellType == CellType::DRACULA_TOMB;
}

std::string cellTypeToString(CellType cellType) {
    switch (cellType) {
        case CellType::EMPTY: return "EMPTY";
        case CellType::WALL: return "WALL";
        case CellType::CHAPEL: return "CHAPEL";
        case CellType::VAMPIRE_LAIR: return "VAMPIRE_LAIR";
        case CellType::ARMORY: return "ARMORY";
        case CellType::DRACULA_TOMB: return "DRACULA_TOMB";
        default: return "UNKNOWN";
    }
}