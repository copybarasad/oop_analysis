#include "cell.h"
#include <fstream>


Cell::Cell() : cellType(CellType::EMPTY) {}

CellType Cell::getType() const {
    return cellType;
}

void Cell::setType(CellType type) {
    cellType = type;
}

bool Cell::isEmpty() const {
    return cellType == CellType::EMPTY;
}

void Cell::save(std::ostream& out) const {
    uint8_t type = static_cast<uint8_t>(cellType);
    out.write(reinterpret_cast<const char*>(&type), sizeof(type));
}

void Cell::load(std::istream& in) {
    uint8_t type;
    in.read(reinterpret_cast<char*>(&type), sizeof(type));
    cellType = static_cast<CellType>(type);
}