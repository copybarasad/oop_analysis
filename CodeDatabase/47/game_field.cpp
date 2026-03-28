#include "game_field.h"
#include <cstdlib>

GameField::GameField(int width, int height) {
    this->width = width;
    this->height = height;

    if (this->width < 10) {
        this->width = 10;
    }
    if (this->width > 25) {
        this->width = 25;
    }
    if (this->height < 10) {
        this->height = 10;
    }
    if (this->height > 25) {
        this->height = 25;
    }

    grid.resize(this->height, std::vector<CellType>(this->width, CellType::EMPTY));

    int obstacleCount = (this->width * this->height) * 12 / 100;

    for (int i = 0; i < obstacleCount; ++i) {
        int x = std::rand() % this->width;
        int y = std::rand() % this->height;
        
        bool canPlace = true;
        
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int checkX = x + dx;
                int checkY = y + dy;
                
                if (checkX >= 0 && checkX < this->width && checkY >= 0 && checkY < this->height) {
                    if (grid[checkY][checkX] == CellType::IMPASSABLE) {
                        canPlace = false;
                        break;
                    }
                }
            }
            if (!canPlace) {
                break;
            }
        }
        
        if (canPlace && grid[y][x] != CellType::IMPASSABLE) {
            grid[y][x] = CellType::IMPASSABLE;
        } 
        else {
            i--;
        }
    }

    int centerX = this->width / 2;
    int centerY = this->height / 2;
    grid[centerY][centerX] = CellType::EMPTY;

    int slowingCount = (this->width * this->height) * 5 / 100;
    for (int i = 0; i < slowingCount; ++i) {
        int x = std::rand() % this->width;
        int y = std::rand() % this->height;
        
        if (grid[y][x] == CellType::EMPTY) {
            grid[y][x] = CellType::SLOWING;
        }
    }
}

GameField::GameField(const GameField& other) 
    : width(other.width), height(other.height), grid(other.grid) {
}

GameField::GameField(GameField&& other) noexcept 
    : width(std::move(other.width)), 
      height(std::move(other.height)), 
      grid(std::move(other.grid)) {

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
        width = std::move(other.width);
        height = std::move(other.height);
        grid = std::move(other.grid);
        
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

int GameField::getHeight() const {
    return height;
}

int GameField::getWidth() const {
    return width;
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

bool GameField::isPassable(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return false;
    }

    return grid[y][x] != CellType::IMPASSABLE;
}

CellType GameField::getCellType(int x, int y) const {
    if (isValidPosition(x, y)) {
        return grid[y][x];
    }
    return CellType::IMPASSABLE;
}

void GameField::setCellType(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        grid[y][x] = type;
    }
}

void GameField::resize(int newWidth, int newHeight) {
    if (newWidth < 10) newWidth = 10;
    if (newWidth > 25) newWidth = 25;
    if (newHeight < 10) newHeight = 10;
    if (newHeight > 25) newHeight = 25;
    
    width = newWidth;
    height = newHeight;
    
    grid.resize(height);
    for (auto& row : grid) {
        row.resize(width, CellType::EMPTY);
    }
}