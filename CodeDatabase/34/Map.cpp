#include "Map.h"
#include "../utils/RandomGenerator.h"
#include "Exceptions.h"
#include <stdexcept>

namespace Game {

Map::Map(int width, int height) :
    mapWidth(width),
    mapHeight(height)
{
    if (mapWidth < 10 || mapWidth > 25 || mapHeight < 10 || mapHeight > 25) {
        throw std::invalid_argument("Map dimensions must be between 10x10 and 25x25.");
    }
    setupInitialMap();
    generateTerrain();
}

Map::Map(std::istream& is) {
    deserialize(is);
}

Map::~Map() {}

Map::Map(const Map& other) :
    mapWidth(other.mapWidth),
    mapHeight(other.mapHeight)
{
    deepCopy(other);
}

Map& Map::operator=(const Map& other) {
    if (this != &other) {
        mapWidth = other.mapWidth;
        mapHeight = other.mapHeight;
        deepCopy(other);
    }
    return *this;
}

void Map::deepCopy(const Map& other) {
    cells.clear();
    cells.resize(mapHeight);
    for (int r = 0; r < mapHeight; ++r) {
        cells[r].resize(mapWidth);
        for (int c = 0; c < mapWidth; ++c) {
            cells[r][c] = std::make_unique<MapCell>(other.cells[r][c]->getType());
        }
    }
}

void Map::setupInitialMap() {
    cells.resize(mapHeight);
    for (int r = 0; r < mapHeight; ++r) {
        cells[r].resize(mapWidth);
        for (int c = 0; c < mapWidth; ++c) {
            cells[r][c] = std::make_unique<MapCell>(CellType::Empty);
        }
    }
}

void Map::generateTerrain() {
    Utils::RandomGenerator& rng = Utils::RandomGenerator::getInstance();
    int numImpassable = (mapWidth * mapHeight) / 10;
    for (int i = 0; i < numImpassable; ++i) {
        Utils::Point p(rng.getRandomInt(0, mapHeight - 1), rng.getRandomInt(0, mapWidth - 1));
        cells[p.row][p.column]->setType(CellType::Impassable);
    }
    int numSlowing = (mapWidth * mapHeight) / 15;
    for (int i = 0; i < numSlowing; ++i) {
        Utils::Point p(rng.getRandomInt(0, mapHeight - 1), rng.getRandomInt(0, mapWidth - 1));
        if (cells[p.row][p.column]->getType() == CellType::Empty) {
            cells[p.row][p.column]->setType(CellType::Slowing);
        }
    }
}

int Map::getWidth() const { return mapWidth; }
int Map::getHeight() const { return mapHeight; }

const MapCell& Map::getCell(const Utils::Point& position) const {
    if (!isValidPosition(position)) throw std::out_of_range("Position out of map bounds.");
    return *cells[position.row][position.column];
}

MapCell& Map::getCell(const Utils::Point& position) {
    if (!isValidPosition(position)) throw std::out_of_range("Position out of map bounds.");
    return *cells[position.row][position.column];
}

bool Map::isValidPosition(const Utils::Point& position) const {
    return position.row >= 0 && position.row < mapHeight &&
           position.column >= 0 && position.column < mapWidth;
}

void Map::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&mapWidth), sizeof(mapWidth));
    os.write(reinterpret_cast<const char*>(&mapHeight), sizeof(mapHeight));
    for (int r = 0; r < mapHeight; ++r) {
        for (int c = 0; c < mapWidth; ++c) {
            cells[r][c]->serialize(os);
        }
    }
}

void Map::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&mapWidth), sizeof(mapWidth));
    is.read(reinterpret_cast<char*>(&mapHeight), sizeof(mapHeight));
    if (is.fail()) throw FileReadException("Failed to read map dimensions.");

    setupInitialMap();
    for (int r = 0; r < mapHeight; ++r) {
        for (int c = 0; c < mapWidth; ++c) {
            cells[r][c]->deserialize(is);
            if (is.fail()) throw FileReadException("Failed to read map cell data.");
        }
    }
}

}