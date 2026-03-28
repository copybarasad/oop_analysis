#include "GameField.h"
#include <stdexcept>
#include <utility>

GameField::GameField(int w, int h) : width(w), height(h), playerX(0), playerY(0) {
    if (w < 10 || w > 25 || h < 10 || h > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    grid.resize(height);
    for (int i = 0; i < height; i++) {
        grid[i].resize(width);
    }
}

GameField::~GameField() {
}

GameField::GameField(const GameField& other)
    : width(other.width), height(other.height),
    playerX(other.playerX), playerY(other.playerY), grid(other.grid) {
}

GameField::GameField(GameField&& other) noexcept
    : width(other.width), height(other.height),
    playerX(other.playerX), playerY(other.playerY), grid(std::move(other.grid)) {
    other.width = 0;
    other.height = 0;
    other.playerX = 0;
    other.playerY = 0;
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        GameField temp(other);
        swap(*this, temp);
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width = other.width;
        height = other.height;
        playerX = other.playerX;
        playerY = other.playerY;
        grid = std::move(other.grid);
        other.width = 0;
        other.height = 0;
        other.playerX = 0;
        other.playerY = 0;
    }
    return *this;
}

void swap(GameField& first, GameField& second) noexcept {
    using std::swap;
    swap(first.width, second.width);
    swap(first.height, second.height);
    swap(first.playerX, second.playerX);
    swap(first.playerY, second.playerY);
    swap(first.grid, second.grid);
}

int GameField::getWidth() const {
    return width;
}

int GameField::getHeight() const {
    return height;
}

void GameField::setPlayerPosition(int x, int y) {
    if (isValidPosition(x, y)) {
        playerX = x;
        playerY = y;
    }
}

std::pair<int, int> GameField::getPlayerPosition() const {
    return { playerX, playerY };
}

bool GameField::movePlayer(int newX, int newY) {
    if (!isValidPosition(newX, newY)) {
        return false;
    }
    if (!isCellEmpty(newX, newY)) {
        return false;
    }
    playerX = newX;
    playerY = newY;
    return true;
}

char GameField::getCellContent(int x, int y) const {
    if (!isValidPosition(x, y)) return '.';
    return grid[y][x].getContent();
}

void GameField::setCellContent(int x, int y, char content) {
    if (isValidPosition(x, y)) {
        grid[y][x].setContent(content);
    }
}

bool GameField::isCellEmpty(int x, int y) const {
    if (!isValidPosition(x, y)) return true;
    return grid[y][x].isEmpty();
}

void GameField::clearCell(int x, int y) {
    if (isValidPosition(x, y)) {
        grid[y][x].clear();
    }
}

void GameField::clearAllCells() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x].clear();
        }
    }
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}