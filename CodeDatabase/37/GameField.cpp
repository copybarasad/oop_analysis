#include "GameField.h"
#include <cstdlib>
#include <ctime>

GameField::GameField(int w, int h) : width(w), height(h) {
    if (width < 10) width = 10;
    if (width > 25) width = 25;
    if (height < 10) height = 10;
    if (height > 25) height = 25;
    
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    generateBlockedCells();
}

void GameField::generateBlockedCells() {
    std::srand(std::time(nullptr));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (std::rand() % 5 == 0) {
                grid[y][x] = CellType::BLOCKED;
            }
        }
    }
    grid[0][0] = CellType::EMPTY;
}


GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), grid(other.grid) {}


GameField::GameField(GameField&& other) 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = other.height = 0;
}


GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}


GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = other.height = 0;
    }
    return *this;
}

bool GameField::isCellPassable(const Position& pos) const {
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return false;
    }
    return grid[pos.y][pos.x] == CellType::EMPTY;
}

CellType GameField::getCellType(const Position& pos) const {
    if (pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) {
        return CellType::BLOCKED;
    }
    return grid[pos.y][pos.x];
}