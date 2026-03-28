#include "Map.h"
#include "../Environment/Block.h"

Map::Map(size_t width, size_t height, size_t layers)
    : cells_(height, std::vector<Cell>(width, Cell(layers))),
      width_(width), height_(height), layers_(layers) {
    if (5 > width || width > 25 || 5 > height || height > 25) {
        throw std::runtime_error("The map sides must be between 5 and 25");
    }
}

Map::Map(size_t width, size_t height, size_t layers, std::vector<Coordinates> blocks)
    : cells_(height, std::vector<Cell>(width, Cell(layers))),
      width_(width), height_(height), layers_(layers) {
    if (5 > width || width > 25 || 5 > height || height > 25) {
        throw std::runtime_error("The map sides must be between 5 and 25");
    }
    for (Coordinates i: blocks) {
        Block *block = new Block();
        this->addToCell(i, block);
    }
}


Coordinates Map::getSize() const {
    return Coordinates(this->width_, this->height_, this->layers_);
}

Object *Map::getFromCell(const Coordinates &pos) const {
    if (pos.x_ < this->width_ && pos.y_ < this->height_ && this->layers_) {
        return cells_[pos.y_][pos.x_].layers_[pos.layer_];
    }
    return nullptr;
}

bool Map::isOccupied(const Coordinates &pos) const {
    if (pos.x_ < this->width_ && pos.y_ < this->height_ && this->layers_) {
        return getFromCell(pos) != nullptr;
    }
    return true;
}

int Map::addToCell(const Coordinates &pos, Object *obj) {
    if (!isOccupied(pos) && obj != nullptr) {
        obj->setPosition(pos);
        cells_[pos.y_][pos.x_].layers_[pos.layer_] = obj;
        return 0;
    }
    return 1;
}

void Map::removeFromCell(const Coordinates &pos) {
    if (isOccupied(pos)) {
        cells_[pos.y_][pos.x_].layers_[pos.layer_] = nullptr;
    }
}

int Map::moveFromTo(const Coordinates &from, const Coordinates &to) {
    if (!isOccupied(to) && getFromCell(from) != nullptr) {
        addToCell(to, getFromCell(from));
        getFromCell(to)->setPosition(to);
        removeFromCell(from);
        return 0;
    }
    return 1;
}
