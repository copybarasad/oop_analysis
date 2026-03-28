#include "GameField.h"
#include <algorithm>
#include <chrono>

GameField::GameField(int width, int height)
    : width_(std::clamp(width, 10, 25)),
      height_(std::clamp(height, 10, 25)),
      grid_(height_, std::vector<Cell>(width_)),
      rng_(static_cast<unsigned int>(
          std::chrono::system_clock::now().time_since_epoch().count()))
{
    generateBlockedCells();
    generateSlowingCells();
}

GameField::GameField(const GameField& other)
    : width_(other.width_), height_(other.height_),
      grid_(other.grid_), rng_(std::random_device{}())
{
}

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
    }
    return *this;
}

GameField::GameField(GameField&& other) noexcept
    : width_(other.width_), height_(other.height_),
      grid_(std::move(other.grid_)),
      rng_(std::random_device{}())
{
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = std::move(other.grid_);
    }
    return *this;
}

int GameField::getWidth() const { return width_; }
int GameField::getHeight() const { return height_; }

CellType GameField::getCellType(int x, int y) const { return grid_[y][x].getType(); }

bool GameField::isCellPassable(int x, int y) const {
    return grid_[y][x].isPassable();
}

const Cell& GameField::getCell(int x, int y) const {
    return grid_[y][x];
}

void GameField::setCellType(int x, int y, CellType type) { grid_[y][x].setType(type); }

bool GameField::isValidPosition(int x, int y) const { return x >= 0 && x < width_ && y >= 0 && y < height_; }

const std::vector<std::vector<Cell>>& GameField::getGrid() const {
    return grid_;
}

Pos GameField::findFirstPassableCell() const {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (grid_[y][x].isPassable()) {
                return {x, y};
            }
        }
    }
    return {-1, -1};
}

void GameField::generateBlockedCells() {
    int blocked = width_ * height_ / 10;
    std::uniform_int_distribution<int> distX(0, width_-1);
    std::uniform_int_distribution<int> distY(0, height_-1);
    for (int i = 0; i < blocked; ++i)
        grid_[distY(rng_)][distX(rng_)].setType(CellType::BLOCKED);
}

void GameField::generateSlowingCells() {
    int slow = width_ * height_ / 10;
    std::uniform_int_distribution<int> distX(0, width_-1);
    std::uniform_int_distribution<int> distY(0, height_-1);
    for (int i = 0; i < slow; ++i)
        if (grid_[distY(rng_)][distX(rng_)].getType() == CellType::EMPTY)
            grid_[distY(rng_)][distX(rng_)].setType(CellType::SLOW);
}

std::vector<std::vector<int>> GameField::getCellTypesAsInt() const {
    std::vector<std::vector<int>> cellTypes(height_, std::vector<int>(width_));
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            cellTypes[y][x] = static_cast<int>(grid_[y][x].getType());
        }
    }
    return cellTypes;
}

void GameField::restoreFromCellTypes(const std::vector<std::vector<int>>& types) {
    // Ensure dimensions match
    if (static_cast<int>(types.size()) != height_ || 
        (height_ > 0 && static_cast<int>(types[0].size()) != width_)) {
        return; // Dimensions mismatch, cannot restore
    }
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            grid_[y][x].setType(static_cast<CellType>(types[y][x]));
        }
    }
}