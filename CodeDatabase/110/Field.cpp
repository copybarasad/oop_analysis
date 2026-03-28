#include "Field.hpp"

Field::Field (unsigned int widthValue, unsigned int heightValue, double blockedPercent, double slowedPercent) {
    width = widthValue >= 10 ? (widthValue > 25 ? 25 : widthValue) : 10;
    height = heightValue >= 10 ? (heightValue > 25 ? 25 : heightValue) : 10;

    initializeMap(blockedPercent, slowedPercent);
}

Field::Field (const Field& other) {
    copyField(other);
}

Field::Field (Field&& other) {
    moveField(std::move(other));
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        copyField(other);
    }
    return *this;
}

Field& Field::operator=(Field&& other) {
    if (this != &other) {
        moveField(std::move(other));
    }
    return *this;
}

unsigned int Field::getWidth () const { return width; }

unsigned int Field::getHeight () const { return height; }

Cell& Field::getCell (size_t x, size_t y) { return map[y][x]; }

const Cell& Field::getCell (size_t x, size_t y) const { return map[y][x]; }

void Field::initializeMap (double blockedPercent, double slowedPercent) {
    map.resize(height);
    for (size_t i = 0; i < height; i++) {
        map[i].resize(width);
        for (size_t j = 0; j < width; j++) {
            map[i][j] = Cell(j, i);
        }
    }

    generateBlockedCells (blockedPercent);
    generateSlowedCells (slowedPercent);
}

void Field::generateBlockedCells (double blockedPercent) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int totalCells = width * height;
    size_t blockedCells = (size_t)(totalCells * blockedPercent);

    for (size_t i = 0; i < blockedCells; i++) {
        size_t x = gen() % width;
        size_t y = gen() % height;

        map[y][x].setType(CellType::BLOCKED);
    }
}

void Field::generateSlowedCells (double slowedPercent) {
    std::random_device rd;
    std::mt19937 gen(rd());

    int totalCells = width * height;
    size_t slowedCells = (size_t)(totalCells * slowedPercent);

    for (size_t i = 0; i < slowedCells; i++) {
        size_t x = gen() % width;
        size_t y = gen() % height;

        map[y][x].setType(CellType::SLOWED);
    }
}

void Field::copyField(const Field& other) {
    width = other.width;
    height = other.height;
    map = other.map;
}

void Field::moveField(Field&& other) {
    width = other.width;
    height = other.height;
    map = std::move(other.map);

    other.width = 0;
    other.height = 0;
}