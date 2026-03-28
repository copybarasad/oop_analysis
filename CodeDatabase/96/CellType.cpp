#include "CellType.h"

std::string typeCelltoString(CellType type) {
    switch (type) {
        case CellType::Empty: return "Empty";
        case CellType::Wall:  return "Wall";
        case CellType::Stun:  return "Stun";
        default: return "Unknown";
    }
}

CellType stringToTypeCell(const std::string& s) {
    if (s == "Empty") return CellType::Empty;
    if (s == "Wall")  return CellType::Wall;
    if (s == "Stun")  return CellType::Stun;
}