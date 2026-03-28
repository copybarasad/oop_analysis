#include "GameField.h"
#include <random>
#include <algorithm>
#include <stdexcept>

template<typename T>
T clamp_value(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

GameField::GameField(int fieldWidth, int fieldHeight)
    : width(clamp_value(fieldWidth, 3, 25)),
      height(clamp_value(fieldHeight, 3, 25)) {
    initializeField();
}

void GameField::initializeField() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    cells.resize(height);
    for (int i = 0; i < height; ++i) {
        cells[i].resize(width);
        for (int j = 0; j < width; ++j) {
            double randVal = dis(gen);
            if (randVal < 0.15) {
                cells[i][j].setType(CellType::OBSTACLE);
            } else if (randVal < 0.25) {
                cells[i][j].setType(CellType::SLOW);
            }
        }
    }
    
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cells[i][j].setType(CellType::EMPTY);
        }
    }
}

GameField::GameField(const GameField& other)
    : width(other.width), height(other.height) {
    cells.resize(height);
    for (int i = 0; i < height; ++i) {
        cells[i].resize(width);
        for (int j = 0; j < width; ++j) {
            cells[i][j] = other.cells[i][j];
        }
    }
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height),
      cells(std::move(other.cells)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = other.cells;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        cells = std::move(other.cells);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

int GameField::getWidth() const { return width; }
int GameField::getHeight() const { return height; }

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

const GameCell& GameField::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return cells[y][x];
}

GameCell& GameField::getCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        throw std::out_of_range("Position out of field bounds");
    }
    return cells[y][x];
}

bool GameField::isCellPassable(int x, int y) const {
    return isValidPosition(x, y) && getCell(x, y).isPassable();
}

std::vector<std::pair<int, int>> GameField::getAdjacentPositions(int x, int y) const {
    std::vector<std::pair<int, int>> positions;

    if (isValidPosition(x, y-1)) {
        positions.emplace_back(x, y-1);
    }
    if (isValidPosition(x, y+1)) {
        positions.emplace_back(x, y+1);
    }
    if (isValidPosition(x-1, y)) {
        positions.emplace_back(x-1, y);
    }
    if (isValidPosition(x+1, y)) {
        positions.emplace_back(x+1, y);
    }
    
    return positions;
}

bool GameField::hasEnemyAt(int x, int y) const {
    return isValidPosition(x, y) && getCell(x, y).hasEnemy();
}

std::optional<Enemy> GameField::getEnemyAt(int x, int y) const {
    if (!hasEnemyAt(x, y)) return std::nullopt;
    return getCell(x, y).getEnemy();
}

void GameField::removeEnemyAt(int x, int y) {
    if (isValidPosition(x, y)) {
        cells[y][x].removeEnemy();
    }
}

void GameField::addEnemyAt(int x, int y, Enemy enemy) {
    if (isValidPosition(x, y)) {
        if (!hasEnemyAt(x, y) && getCell(x, y).getType() != CellType::OBSTACLE) {
            cells[y][x].setEnemy(std::make_unique<Enemy>(enemy));
        }
    }
}


void GameField::placeTrap(int x, int y) {
    if (isValidPosition(x, y) && isCellPassable(x, y)) {
        trapPositions.emplace_back(x, y);
    }
}

bool GameField::hasTrapAt(int x, int y) const {
    for (const auto& pos : trapPositions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

void GameField::triggerTrap(int x, int y) {
    trapPositions.erase(
        std::remove_if(trapPositions.begin(), trapPositions.end(),
            [x, y](const auto& pos) { return pos.first == x && pos.second == y; }),
        trapPositions.end()
    );
}

void GameField::placeAlly(int x, int y) {
    if (isValidPosition(x, y) && isCellPassable(x, y) && !hasEnemyAt(x, y) && !hasAllyAt(x, y)) {
        allyPositions.emplace_back(x, y);
    }
}

bool GameField::hasAllyAt(int x, int y) const {
    for (const auto& pos : allyPositions) {
        if (pos.first == x && pos.second == y) {
            return true;
        }
    }
    return false;
}

void GameField::removeAlly(int x, int y) {
    allyPositions.erase(
        std::remove_if(allyPositions.begin(), allyPositions.end(),
            [x, y](const auto& pos) { return pos.first == x && pos.second == y; }),
        allyPositions.end()
    );
}

void GameField::setCellType(int x, int y, CellType cellType) {
    if (isValidPosition(x, y)) {
        cells[y][x].setType(cellType);
    }
}

void GameField::clearField() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            cells[y][x].setType(CellType::EMPTY);
            if (cells[y][x].hasEnemy()) {
                cells[y][x].removeEnemy();
            }
        }
    }
    trapPositions.clear();
    allyPositions.clear();
}
