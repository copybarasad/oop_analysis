#include "GameField.h"
#include <random>
#include <algorithm>

Cell::Cell() : type(CellType::EMPTY), hasWanderer(false), hasBeast(false), hasBuilding(false), hasTrap(false), trapDamage(0) {}

CellType Cell::getType() const { return type; }
void Cell::setType(CellType newType) { type = newType; }

bool Cell::isEmpty() const { 
    return !hasWanderer && !hasBeast && !hasBuilding; 
}

bool Cell::isPassable() const { 
    return type != CellType::OBSTACLE && isEmpty(); 
}

bool Cell::getHasWanderer() const { return hasWanderer; }
void Cell::setHasWanderer(bool value) { hasWanderer = value; }

bool Cell::getHasBeast() const { return hasBeast; }
void Cell::setHasBeast(bool value) { hasBeast = value; }

bool Cell::getHasBuilding() const { return hasBuilding; }
void Cell::setHasBuilding(bool value) { hasBuilding = value; }

bool Cell::getHasTrap() const { return hasTrap; }
void Cell::setHasTrap(bool value) { hasTrap = value; }

int Cell::getTrapDamage() const { return trapDamage; }
void Cell::setTrapDamage(int damage) { trapDamage = damage; }

void Cell::triggerTrap() {
    hasTrap = false;
    trapDamage = 0;
}

void Cell::clear() {
    hasWanderer = false;
    hasBeast = false;
    hasBuilding = false;
    hasTrap = false;
    trapDamage = 0;
}

GameField::GameField(int fieldWidth, int fieldHeight) 
    : width(std::clamp(fieldWidth, 10, 25)), height(std::clamp(fieldHeight, 10, 25)) {
    initializeField();
}

GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), grid(other.grid) {
}

GameField::GameField(GameField&& other) noexcept 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameField::initializeField() {
    grid.resize(height, std::vector<Cell>(width));
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double randVal = dis(gen);
            if (randVal < 0.15) {
                grid[y][x].setType(CellType::OBSTACLE);
            } else if (randVal < 0.25) {
                grid[y][x].setType(CellType::SLOW);
            }
        }
    }
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

Cell& GameField::getCell(int x, int y) {
    return grid[y][x];
}

const Cell& GameField::getCell(int x, int y) const {
    return grid[y][x];
}

bool GameField::isCellPassable(int x, int y) const {
    return isValidPosition(x, y) && grid[y][x].isPassable();
}

bool GameField::findEmptyPosition(int& x, int& y) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> xDis(0, width - 1);
    std::uniform_int_distribution<> yDis(0, height - 1);
    
    for (int attempt = 0; attempt < 100; ++attempt) {
        x = xDis(gen);
        y = yDis(gen);
        if (getCell(x, y).isEmpty() && isCellPassable(x, y)) {
            return true;
        }
    }
    return false;
}

bool GameField::moveEntity(int fromX, int fromY, int toX, int toY, bool isWanderer) {
    if (!isValidPosition(toX, toY) || !grid[toY][toX].isPassable()) {
        return false;
    }
    
    grid[fromY][fromX].clear();
    
    if (isWanderer) {
        grid[toY][toX].setHasWanderer(true);
    } else {
        grid[toY][toX].setHasBeast(true);
    }
    
    return true;
}

bool GameField::hasLineOfSight(int fromX, int fromY, int toX, int toY) const {
    if (!isValidPosition(fromX, fromY) || !isValidPosition(toX, toY)) {
        return false;
    }

    int dx = toX - fromX;
    int dy = toY - fromY;

    int stepX = (dx > 0) - (dx < 0);
    int stepY = (dy > 0) - (dy < 0);

    int currentX = fromX + stepX;
    int currentY = fromY + stepY;

    while (currentX != toX || currentY != toY) {
        if (!isValidPosition(currentX, currentY)) {
            return false;
        }

        const Cell& cell = getCell(currentX, currentY);
        if (cell.getType() == CellType::OBSTACLE || cell.getHasBuilding() || cell.getHasBeast()) {
            return false;
        }

        currentX += stepX;
        currentY += stepY;
    }

    return true;
}

void GameField::resize(int newWidth, int newHeight) {
    width = std::clamp(newWidth, 10, 25);
    height = std::clamp(newHeight, 10, 25);
    
    std::vector<std::vector<Cell>> oldGrid = std::move(grid);
    
    grid.resize(height, std::vector<Cell>(width));
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (y < (int)oldGrid.size() && x < (int)oldGrid[y].size()) {
                grid[y][x] = oldGrid[y][x];
            } else {
                grid[y][x] = Cell();
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<> dis(0.0, 1.0);
                
                double randVal = dis(gen);
                if (randVal < 0.15) {
                    grid[y][x].setType(CellType::OBSTACLE);
                } else if (randVal < 0.25) {
                    grid[y][x].setType(CellType::SLOW);
                }
            }
        }
    }
}