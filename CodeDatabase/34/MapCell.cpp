#include "MapCell.h"

namespace Game {

MapCell::MapCell(CellType type) : cellType(type) {}

MapCell::~MapCell() {}

CellType MapCell::getType() const {
    return cellType;
}

void MapCell::setType(CellType newType) {
    cellType = newType;
}

bool MapCell::isPassable() const {
    return cellType != CellType::Impassable;
}

bool MapCell::isSlowing() const {
    return cellType == CellType::Slowing;
}

void MapCell::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&cellType), sizeof(cellType));
}

void MapCell::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&cellType), sizeof(cellType));
}

}