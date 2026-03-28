#include "GameMap.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>

GameMap::GameMap(int width, int height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Map size must be 10x10 to 25x25");
    }
    
    this->width = width;
    this->height = height;
    initGrid();
}

GameMap::GameMap(const GameMap& other) 
    : width(other.width), height(other.height), grid(other.grid) {}

GameMap::GameMap(GameMap&& other) noexcept 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

GameMap& GameMap::operator=(const GameMap& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

GameMap& GameMap::operator=(GameMap&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameMap::initGrid() {
    grid.resize(height, std::vector<CellType>(width, CellType::EMPTY));
    // 15% клеток стены
    srand(time(nullptr));
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (rand() % 100 < 15) {
                grid[y][x] = CellType::WALL;
            }
        }
    }
    // Cтартовая позиция игрока свободна
    int centerX = width / 2;
    int centerY = height / 2;
    grid[centerY][centerX] = CellType::EMPTY;
}

bool GameMap::isValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int GameMap::getWidth() const { return width; }
int GameMap::getHeight() const { return height; }

CellType GameMap::getCell(int x, int y) const {
    if (!isValid(x, y)) throw std::out_of_range("Position out of bounds");
    return grid[y][x];
}

void GameMap::setCell(int x, int y, CellType type) {
    if (!isValid(x, y)) throw std::out_of_range("Position out of bounds");
    grid[y][x] = type;
}

void GameMap::clearCell(int x, int y) {
    setCell(x, y, CellType::EMPTY);
}

bool GameMap::isEmpty(int x, int y) const {
    return getCell(x, y) == CellType::EMPTY;
}

bool GameMap::isWall(int x, int y) const {
    return getCell(x, y) == CellType::WALL;
}

bool GameMap::inBounds(int x, int y) const {
    return isValid(x, y);
}

void GameMap::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char symbol;
            switch (grid[y][x]) {
                case CellType::EMPTY: symbol = '.'; break;
                case CellType::PLAYER: symbol = 'P'; break;
                case CellType::ENEMY: symbol = 'E'; break;
                case CellType::WALL: symbol = '#'; break;
                case CellType::TRAP: symbol = 't'; break;
                case CellType::TOWER: symbol = 'T'; break;
                case CellType::ALLY: symbol = 'A'; break; 
            }
            std::cout << symbol << ' ';
        }
        std::cout << std::endl;
    }
}