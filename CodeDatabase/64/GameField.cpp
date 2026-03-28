#include "GameField.h"
#include "Player.h"
#include "Enemy.h"
#include <random>
#include <stdexcept>

GameField::GameField(int width, int height) 
    : fieldWidth(width), fieldHeight(height) {
    
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    
    obstacleGrid.resize(height, std::vector<bool>(width, false));
    createRandomObstacles();
}

void GameField::deepCopyFrom(const GameField& other) {
    fieldWidth = other.fieldWidth;
    fieldHeight = other.fieldHeight;
    
    obstacleGrid.resize(fieldHeight);
    for (int row = 0; row < fieldHeight; ++row) {
        obstacleGrid[row] = other.obstacleGrid[row];
    }
}

GameField::GameField(const GameField& other) {
    deepCopyFrom(other);
}

GameField::GameField(GameField&& other) noexcept
    : fieldWidth(other.fieldWidth), fieldHeight(other.fieldHeight), 
      obstacleGrid(std::move(other.obstacleGrid)) {
    
    other.fieldWidth = 0;
    other.fieldHeight = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        deepCopyFrom(other);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        fieldWidth = other.fieldWidth;
        fieldHeight = other.fieldHeight;
        obstacleGrid = std::move(other.obstacleGrid);
        other.fieldWidth = 0;
        other.fieldHeight = 0;
    }
    return *this;
}

int GameField::getFieldWidth() const {
    return fieldWidth;
}

int GameField::getFieldHeight() const {
    return fieldHeight;
}

bool GameField::isPositionWithinBounds(int x, int y) const {
    return x >= 0 && x < fieldWidth && y >= 0 && y < fieldHeight;
}

bool GameField::isCellPassable(int x, int y) const {
    if (!isPositionWithinBounds(x, y)) return false;
    return !obstacleGrid[y][x];
}

bool GameField::isObstacle(int x, int y) const {
    if (!isPositionWithinBounds(x, y)) return false;
    return obstacleGrid[y][x];
}

void GameField::setObstacle(int x, int y, bool blocked) {
    if (isPositionWithinBounds(x, y)) {
        obstacleGrid[y][x] = blocked;
    }
}

void GameField::createRandomObstacles() {
    std::random_device randomDevice;
    std::mt19937 generator(randomDevice());
    std::uniform_int_distribution<int> distX(0, fieldWidth - 1);
    std::uniform_int_distribution<int> distY(0, fieldHeight - 1);
    
    int numberOfObstacles = (fieldWidth * fieldHeight) / 10;
    
    for (int i = 0; i < numberOfObstacles; ++i) {
        int x = distX(generator);
        int y = distY(generator);
        
        if (x > 1 && x < fieldWidth - 2 && y > 1 && y < fieldHeight - 2) {
            obstacleGrid[y][x] = true;
        }
    }
}

char GameField::getCellRepresentation(int x, int y, const Player& player, const std::vector<Enemy>& enemies) const {
    if (player.isOnPosition(x, y)) {
        return player.isAlive() ? 'P' : 'X';
    }
    
    for (const auto& enemy : enemies) {
        if (enemy.isOnPosition(x, y) && enemy.isAlive()) {
            return 'E';
        }
    }
    
    if (obstacleGrid[y][x]) {
        return '#';
    }
    
    return '.';
}