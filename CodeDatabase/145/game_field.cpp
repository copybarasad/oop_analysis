#include "game_field.h"
#include "enemy.h"
#include "enemy_building.h"
#include "enemy_tower.h"
#include "player.h"
#include "ally.h"
#include "exceptions.h"
#include <cstdlib>
#include <stdexcept>
#include <algorithm>

GameField::GameField(size_t width, size_t height) 
    : width(width), height(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw GameLogicException("Field dimensions (width=" + std::to_string(width) + 
                                ", height=" + std::to_string(height) + 
                                ") are invalid: must be between 10x10 and 25x25");
    }
    
    grid.resize(height, std::vector<Cell>(width));
    
    generateImpassableCells((width * height) / 10);  
    generateSlowingCells((width * height) / 20);     
}

GameField::GameField(const GameField& other) {
    deepCopy(other);
}

GameField::GameField(GameField&& other)
	: width(other.width), height(other.height), grid(std::move(other.grid)), traps(std::move(other.traps)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        deepCopy(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        traps = std::move(other.traps);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

size_t GameField::getWidth() const {
    return width;
}

size_t GameField::getHeight() const {
    return height;
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < (int)width && y >= 0 && y < (int)height;
}

bool GameField::isPassable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].isPassable();
}

bool GameField::isSlowing(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].isSlowing();
}

bool GameField::isEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }
    return grid[y][x].isEmpty();
}

void GameField::setCell(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(type);
    }
}

void GameField::generateImpassableCells(int count) {
    for (int i = 0; i < count; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        grid[y][x].setType(CellType::IMPASSABLE);
    }
}

void GameField::generateSlowingCells(int count) {
    for (int i = 0; i < count; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        if (grid[y][x].getType() == CellType::EMPTY) {
            grid[y][x].setType(CellType::SLOWING);
        }
    }
}

void GameField::deepCopy(const GameField& other) {
    width = other.width;
    height = other.height;
    grid = other.grid;
    traps = other.traps;
}

void GameField::placeTrap(int x, int y, int damage) {
    if (isValidPosition(x, y)) {
        traps.push_back({x, y, damage});
    }
}

const std::vector<Trap>& GameField::getTraps() const {
    return traps;
}

void GameField::removeTrap(int x, int y) {
    traps.erase(
        std::remove_if(traps.begin(), traps.end(),
            [x, y](const Trap& trap) { return trap.x == x && trap.y == y; }),
        traps.end()
    );
}

bool GameField::hasTrapAt(int x, int y) const {
    for (const auto& trap : traps) {
        if (trap.x == x && trap.y == y) {
            return true;
        }
    }
    return false;
}

int GameField::getTrapDamageAt(int x, int y) const {
    for (const auto& trap : traps) {
        if (trap.x == x && trap.y == y) {
            return trap.damage;
        }
    }
    return 0;
}

bool GameField::isOccupied(int x, int y,
                           const std::vector<Enemy>& enemies,
                           const std::vector<EnemyBuilding>& buildings,
                           const std::vector<EnemyTower>& towers,
                           const Player* player,
                           const std::vector<Ally>* allies) const {
    if (!isValidPosition(x, y)) return true; 

    for (const auto& e : enemies) {
        if (e.getPosition() == std::make_pair(x, y)) return true;
    }

    for (const auto& b : buildings) {
        if (b.getPosition() == std::make_pair(x, y)) return true;
    }

    for (const auto& t : towers) {
        if (t.getPosition() == std::make_pair(x, y)) return true;
    }

    if (player != nullptr) {
        if (player->getPosition() == std::make_pair(x, y)) return true;
    }

    if (allies != nullptr) {
        for (const auto& a : *allies) {
            if (a.getPosition() == std::make_pair(x, y)) return true;
        }
    }

    return false;
}

bool GameField::isStructureAt(int x, int y,
                              const std::vector<EnemyBuilding>& buildings,
                              const std::vector<EnemyTower>& towers) const {
    if (!isValidPosition(x, y)) return false;
    
    for (const auto& building : buildings) {
        if (building.getPosition() == std::make_pair(x, y)) return true;
    }
    
    for (const auto& tower : towers) {
        if (tower.getPosition() == std::make_pair(x, y)) return true;
    }
    
    return false;
}