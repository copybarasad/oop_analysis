#include "game_field.h"
#include <stdexcept>
#include <random>

GameField::GameField(int fieldWidth, int fieldHeight) 
    : width(fieldWidth), height(fieldHeight) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field dimensions must be between 10x10 and 25x25");
    }
    
    grid.resize(height, std::vector<GameCell>(width));
    initializeField();
}

// Конструктор копирования
GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), grid(other.grid) {}

// Конструктор перемещения
GameField::GameField(GameField&& other) noexcept 
    : width(other.width), height(other.height), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
}

// Оператор присваивания копированием
GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

// Оператор присваивания перемещением
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double randomValue = dis(gen);
            if (randomValue < 0.15) { // 15% chance for obstacle
                grid[y][x] = GameCell(CellType::OBSTACLE);
            } else if (randomValue < 0.25) { // 10% chance for slow cell
                grid[y][x] = GameCell(CellType::SLOW);
            } else {
                grid[y][x] = GameCell(CellType::EMPTY);
            }
        }
    }
   
    grid[0][0] = GameCell(CellType::EMPTY);
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.getX() >= 0 && pos.getX() < width && 
           pos.getY() >= 0 && pos.getY() < height;
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

const GameCell& GameField::getCell(const Position& pos) const {
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Position is outside field boundaries");
    }
    return grid[pos.getY()][pos.getX()];
}

bool GameField::isPositionPassable(const Position& pos) const {
    if (!isValidPosition(pos)) {
        return false;
    }
    return getCell(pos).isPassable();
}

void GameField::setCellOccupied(const Position& pos, bool occupied) {
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Position is outside field boundaries");
    }
    
    grid[pos.getY()][pos.getX()].setIsOccupied(occupied);
}

void GameField::setCellType(const Position& pos, CellType type) {
    if (!isValidPosition(pos)) {
        throw std::out_of_range("Position is outside field boundaries");
    }
    grid[pos.getY()][pos.getX()].setType(type);
}

void GameField::clearAllOccupancy() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x].setIsOccupied(false);
        }
    }
}

std::vector<Position> GameField::getAdjacentPositions(const Position& pos) const {
    std::vector<Position> adjacent;
    Position directions[] = {
        Position(0, -1),  // UP
        Position(0, 1),   // DOWN  
        Position(-1, 0),  // LEFT
        Position(1, 0)    // RIGHT
    };
    
    for (const auto& dir : directions) {
        Position newPos = pos + dir;
        if (isValidPosition(newPos) && isPositionPassable(newPos)) {
            adjacent.push_back(newPos);
        }
    }
    
    return adjacent;
}
