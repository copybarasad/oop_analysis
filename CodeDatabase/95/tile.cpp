#include "tile.hpp"

Tile::Tile(TileType type_) : type(type_) {
    walkable = (type_ != TileType::Wall);
}

bool Tile::isWalkable() const {
    return walkable;
}

TileType Tile::getType() const {
    return type;
}
