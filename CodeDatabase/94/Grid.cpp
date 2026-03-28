#include "Grid.h"
#include <random>
#include <stdexcept>

Grid::Grid(int width, int height) : width(width), height(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    
    grid.resize(height, std::vector<Cell>(width));
    initializeWalls();
}

Grid::Grid(const Grid& other) : width(other.width), height(other.height), grid(other.grid) {}

Grid::Grid(Grid&& other) noexcept 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {}

Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

Grid& Grid::operator=(Grid&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
    }
    return *this;
}

void Grid::initializeWalls() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (dis(gen) < 0.15) {
                setCellType(Position(x, y), CellType::WALL);
            }
        }
    }
    
    for (int i = 0; i < 10; ++i) {
        Position emptyPos = findRandomEmptyPosition();
        setCellType(emptyPos, CellType::EMPTY);
    }
}

CellType Grid::getCellType(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return CellType::WALL;
    }
    return grid[pos.y()][pos.x()].getType();
}

bool Grid::isValidPosition(const Position& pos) const {
    return pos.x() >= 0 && pos.x() < width && pos.y() >= 0 && pos.y() < height;
}

void Grid::setCellType(const Position& pos, CellType cellType) {
    if (isValidPosition(pos)) {
        grid[pos.y()][pos.x()].setType(cellType);
    }
}

bool Grid::isPositionPassable(const Position& pos) const {
    return isValidPosition(pos) && grid[pos.y()][pos.x()].getIsPassable();
}

bool Grid::isPositionEmpty(const Position& pos) const {
    return isValidPosition(pos) && grid[pos.y()][pos.x()].getType() == CellType::EMPTY;
}

Position Grid::findRandomEmptyPosition() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distX(0, width - 1);
    std::uniform_int_distribution<> distY(0, height - 1);
    
    Position pos;
    int attempts = 0;
    do {
        pos = Position(distX(gen), distY(gen));
        attempts++;
    } while (!isPositionEmpty(pos) && attempts < 100);
    
    if (attempts >= 100) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Position sequentialPos(x, y);
                if (isPositionEmpty(sequentialPos)) {
                    return sequentialPos;
                }
            }
        }
    }
    
    return pos;
}