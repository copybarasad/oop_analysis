#include "game_field.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <ctime>
#include <algorithm>

GameField::GameField(size_t w, size_t h) : width(w), height(h) {
    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }

    grid.resize(height, std::vector<CellType>(width, CellType::Empty));
    initializeWalls();
}

void GameField::deepCopy(const GameField& other) {
    width = other.width;
    height = other.height;
    grid = other.grid;
    entities = other.entities;
    entityPositions = other.entityPositions;
}

GameField::GameField(const GameField& other) {
    deepCopy(other);
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height),
      grid(std::move(other.grid)), entities(std::move(other.entities)),
      entityPositions(std::move(other.entityPositions)) {
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        deepCopy(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
        entities = std::move(other.entities);
        entityPositions = std::move(other.entityPositions);

        other.width = 0;
        other.height = 0;
    }
    return *this;
}

void GameField::initializeWalls() {
    std::srand(std::time(0));

    for (size_t i = 0; i < height; ++i) {
        for (size_t j = 0; j < width; ++j) {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) {
                grid[i][j] = CellType::Wall;
            } else if (std::rand() % 8 == 0) {
                grid[i][j] = CellType::Wall;
            }
        }
    }
}

size_t GameField::getWidth() const {
    return width;
}

size_t GameField::getHeight() const {
    return height;
}

bool GameField::isWithinBounds(int x, int y) const {
    return x >= 0 && y >= 0 && x < static_cast<int>(width) && y < static_cast<int>(height);
}

bool GameField::isCellPassable(int x, int y) const {
    return isWithinBounds(x, y) && grid[y][x] != CellType::Wall;
}

CellType GameField::getCellType(int x, int y) const {
    if (!isWithinBounds(x, y)) {
        throw std::out_of_range("Coordinates out of field bounds");
    }
    return grid[y][x];
}

bool GameField::hasClearLineOfSight(int x1, int y1, int x2, int y2) const {
    if (!isWithinBounds(x1, y1) || !isWithinBounds(x2, y2)) {
        return false;
    }

    if (x1 == x2) {
        int startY = std::min(y1, y2);
        int endY = std::max(y1, y2);
        for (int y = startY + 1; y < endY; ++y) {
            if (!isCellPassable(x1, y)) {
                return false;
            }
        }
    } else if (y1 == y2) {
        int startX = std::min(x1, x2);
        int endX = std::max(x1, x2);
        for (int x = startX + 1; x < endX; ++x) {
            if (!isCellPassable(x, y1)) {
                return false;
            }
        }
    }
    return true;
}

std::pair<int, int> GameField::getRandomEmptyCell() const {
    int x, y;
    int attempts = 0;
    const int maxAttempts = 100;

    do {
        x = 1 + std::rand() % (width - 2);
        y = 1 + std::rand() % (height - 2);
        attempts++;

        if (attempts > maxAttempts) {
            throw std::runtime_error("Cannot find empty cell after multiple attempts");
        }
    } while (!isCellPassable(x, y) || getEntityAt(x, y));

    return std::pair<int, int>(x, y);
}

void GameField::addEntity(std::shared_ptr<Entity> entity, int x, int y) {
    if (!entity) {
        throw std::invalid_argument("Cannot add null entity");
    }
    if (!isWithinBounds(x, y) || !isCellPassable(x, y)) {
        throw std::invalid_argument("Invalid position for entity");
    }
    if (getEntityAt(x, y)) {
        throw std::invalid_argument("Position already occupied");
    }
    entities.push_back(entity);
    entityPositions.push_back(std::make_pair(x, y));
}

void GameField::removeEntity(int x, int y) {
    for (size_t i = 0; i < entityPositions.size(); ++i) {
        if (entityPositions[i].first == x && entityPositions[i].second == y) {
            entities.erase(entities.begin() + i);
            entityPositions.erase(entityPositions.begin() + i);
            break;
        }
    }
}

std::shared_ptr<Entity> GameField::getEntityAt(int x, int y) const {
    for (size_t i = 0; i < entityPositions.size(); ++i) {
        if (entityPositions[i].first == x && entityPositions[i].second == y && entities[i]->isAlive()) {
            return entities[i];
        }
    }
    return nullptr;
}

const std::vector<std::shared_ptr<Entity>>& GameField::getEntities() const {
    return entities;
}

bool GameField::moveEntity(int fromX, int fromY, int toX, int toY) {
    if (!isWithinBounds(toX, toY) || !isCellPassable(toX, toY) || getEntityAt(toX, toY)) {
        return false;
    }

    for (size_t i = 0; i < entityPositions.size(); ++i) {
        if (entityPositions[i].first == fromX && entityPositions[i].second == fromY) {
            entityPositions[i] = std::make_pair(toX, toY);
            return true;
        }
    }
    return false;
}

char GameField::getCellSymbol(int x, int y) const {
    if (!isWithinBounds(x, y)) return ' ';

    auto entity = getEntityAt(x, y);
    if (entity) {
        if (entity->getType() == "Player") return 'P';
        if (entity->getType() == "Enemy") return 'E';
        return '?';
    }

    switch (grid[y][x]) {
        case CellType::Empty: return '.';
        case CellType::Wall: return '#';
        default: return '?';
    }
}

void GameField::setCellType(int x, int y, CellType type) {
    if (!isWithinBounds(x, y)) {
        throw std::out_of_range("Coordinates out of field bounds");
    }
    grid[y][x] = type;
}

void GameField::display() const {
    for (int y = 0; y < static_cast<int>(height); ++y) {
        for (int x = 0; x < static_cast<int>(width); ++x) {
            std::cout << getCellSymbol(x, y) << " ";
        }
        std::cout << std::endl;
    }
}