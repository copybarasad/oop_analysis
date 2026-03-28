#include "map.hpp"
#include <algorithm>

Map::Map(int w, int h) {
    width = std::clamp(w, 10, 25);
    height = std::clamp(h, 10, 25);
    tiles.resize(height, std::vector<Tile>(width, Tile(TileType::Grass)));
}

Map::Map(const Map& other_map) : width(other_map.width), height(other_map.height), tiles(other_map.tiles) {}


Map::Map(Map&& moved_map) noexcept : width(moved_map.width), height(moved_map.height), tiles(std::move(moved_map.tiles))
{
    moved_map.width = 0;
    moved_map.height = 0;
}

Map& Map::operator=(const Map& other_map) {
    if (&other_map != this) {
        width = other_map.width;
        height = other_map.height;
        tiles = other_map.tiles;
    }
    return *this;
}

Map& Map::operator=(Map&& moved_map) noexcept {
    if (&moved_map != this) {
        width = moved_map.width;
        height = moved_map.height;
        tiles = std::move(moved_map.tiles);
        
        moved_map.width = 0;
        moved_map.height = 0;
    }
    return *this;
}

int Map::getWidth() const {
    return width;
}

int Map::getHeight() const {
    return height;
}

const Tile& Map::getTile(int x, int y) const {
    return tiles[y][x];
}

void Map::setTile(int x, int y, TileType type) {
    tiles[y][x] = Tile(type);
}

std::vector<Position> Map::freeCells() const {
    std::vector<Position> freeTiles;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (tiles[y][x].isWalkable())
                freeTiles.push_back({x, y});
        }
    }

    return freeTiles;
}
