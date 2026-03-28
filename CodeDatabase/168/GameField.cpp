#include "GameField.h"
#include <random>
#include <algorithm>

GameField::GameField(int width, int height) : width(width), height(height) {
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }

    grid.resize(height, std::vector<Cell>(width));
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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double randVal = dis(gen);
            if (randVal < 0.15) {
                grid[y][x] = Cell(CellType::OBSTACLE);
            }
            else if (randVal < 0.25) {
                grid[y][x] = Cell(CellType::SLOWERING);
            }
        }
    }

    grid[0][0] = Cell(CellType::EMPTY);
    if (width > 1) {
        grid[0][1] = Cell(CellType::EMPTY);
    }
    if (height > 1) {
        grid[1][0] = Cell(CellType::EMPTY);
    }
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

bool GameField::isCellPassable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].is_Passable();
}

bool GameField::isCellEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].is_Empty();
}

bool GameField::isEnemyHere(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].has_Enemy();
}

CellType GameField::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return CellType::OBSTACLE;
    }
    return grid[y][x].get_type();
}

void GameField::setPlayerPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Player(true);
    }
}

void GameField::clearPlayerPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Player(false);
    }
}

void GameField::setEnemyPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Enemy(true);
    }
}

void GameField::clearEnemyPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Enemy(false);
    }
}

void GameField::setBuildingPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Building(true);
    }
}

void GameField::clearBuildingPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Building(false);
    }
}

void GameField::setTowerPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Tower(true);
    }
}

void GameField::clearTowerPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Tower(false);
    }
}

void GameField::setAllyPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Ally(true);
    }
}

void GameField::clearAllyPosition(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Ally(false);
    }
}

void GameField::setTrap(const Position& position, int damage) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].set_Trap(damage);
    }
}

void GameField::clearTrap(const Position& position) {
    if (isValidPosition(position.x, position.y)) {
        grid[position.y][position.x].clear_Trap();
    }
}

bool GameField::hasTrap(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].has_Trap();
}

int GameField::getTrapDamage(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return 0;
    }
    return grid[y][x].get_TrapDamage();
}

const Cell& GameField::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return grid[y][x];
}

Cell& GameField::getCellMutable(int x, int y) {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return grid[y][x];
}
