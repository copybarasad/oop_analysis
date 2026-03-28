#include "game_field.h"
#include "enemy_tower.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <limits>
#include <sstream>
#include "save_manager.h"

GameField::GameField(int w, int h)
    : width(w), height(h) {
    if (width < 10 || height < 10 || width > 25 || height > 25) {
        std::cout << "Warning: Field size must be between 10x10 and 25x25. Using 15x15.\n";
        width = 15;
        height = 15;
    }
    initializeGrid();
}

GameField::GameField() : width(0), height(0) {}

void GameField::initializeGrid() {
    grid.clear();
    grid.reserve(height);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height; ++y) {
        std::vector<Cell> row;
        row.reserve(width);
        for (int x = 0; x < width; ++x) {
            CellType type = (dis(gen) < 0.2 && !(x == 0 && y == 0))
                          ? CellType::OBSTACLE
                          : CellType::EMPTY;
            row.emplace_back(x, y, type);
        }
        grid.push_back(std::move(row));
    }

    if (width > 1 && height > 1) {
        Cell& right = grid[0][1];
        Cell& down = grid[1][0];
        if (!right.isPassable() && !down.isPassable()) {
            if (dis(gen) < 0.5) {
                right = Cell(1, 0, CellType::EMPTY);
            } else {
                down = Cell(0, 1, CellType::EMPTY);
            }
        }
    }
}

void GameField::resizeEmpty(int w, int h) {
    width = w;
    height = h;
    grid.clear();
    grid.reserve(h);
    for (int y = 0; y < h; ++y) {
        std::vector<Cell> row;
        row.reserve(w);
        for (int x = 0; x < w; ++x) {
            row.emplace_back(x, y, CellType::EMPTY);
        }
        grid.push_back(std::move(row));
    }
    traps_.clear();
}

bool GameField::isPositionValid(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

const Cell& GameField::getCell(int x, int y) const {
    if (!isPositionValid(x, y)) {
        static const Cell invalid(0, 0, CellType::OBSTACLE);
        return invalid;
    }
    return grid[y][x];
}

bool GameField::isPositionPassable(int x, int y) const {
    return isPositionValid(x, y) && grid[y][x].isPassable();
}

bool GameField::isPositionEmpty(int x, int y) const {
    return isPositionValid(x, y) && grid[y][x].isEmpty();
}

void GameField::addTrap(const Trap& trap) {
    if (isPositionPassable(trap.getPosition().x, trap.getPosition().y)) {
        traps_.push_back(trap);
    }
}

void GameField::removeInactiveTraps() {
    traps_.erase(
        std::remove_if(traps_.begin(), traps_.end(),
            [](const Trap& t) { return !t.isActive(); }),
        traps_.end()
    );
}

bool GameField::isTowerAt(int x, int y, const std::vector<EnemyTower>& towers) const {
    for (const auto& tower : towers) {
        Position p = tower.getPosition();
        if (p.x == x && p.y == y) {
            return true;
        }
    }
    return false;
}

void GameField::save(std::ostream& os) const {
    os << width << " " << height << "\n";

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            os << (grid[y][x].getType() == CellType::OBSTACLE ? "1" : "0");
            if (x < width - 1) os << " ";
        }
        os << "\n";
    }

    os << traps_.size() << "\n";
    for (const auto& trap : traps_) {
        trap.save(os);
    }
}

void GameField::load(std::istream& is) {
    int w, h;
    is >> w >> h;
    if (is.fail() || w < 1 || h < 1 || w > 100 || h > 100) {
        throw SaveDataException("Invalid field size in save file");
    }
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    resizeEmpty(w, h);

    for (int y = 0; y < h; ++y) {
        std::string line;
        std::getline(is, line);
        if (line.empty()) {
            y--;
            continue;
        }
        std::istringstream iss(line);
        for (int x = 0; x < w; ++x) {
            int cellVal;
            if (!(iss >> cellVal)) {
                throw SaveDataException("Failed to read cell at (" + std::to_string(x) + "," + std::to_string(y) + ")");
            }
            CellType type = (cellVal == 1) ? CellType::OBSTACLE : CellType::EMPTY;
            grid[y][x] = Cell(x, y, type);
        }
    }

    size_t trapCount;
    is >> trapCount;
    if (is.fail() || trapCount > 1000) {
        throw SaveDataException("Invalid trap count");
    }
    is.ignore();

    traps_.clear();
    traps_.reserve(trapCount);
    for (size_t i = 0; i < trapCount; ++i) {
        Trap trap;
        trap.load(is);
        traps_.push_back(std::move(trap));
    }
}