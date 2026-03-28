#include "game_field.h"
#include <random>
#include <algorithm>

GameField::GameField(int width, int height)
    : width_(width), height_(height), cells_(height, std::vector<Cell>(width)),
      traps_(height, std::vector<Trap>(width)) {
    if (width_ < 10 || width_ > 25 || height_ < 10 || height_ > 25)
        throw std::invalid_argument("Размер поля должен быть от 10 до 25");
    GenerateObstacles();
}

bool GameField::IsValidPosition(const Position& pos) const {
    return IsWithinBounds(pos);
}

bool GameField::IsWithinBounds(const Position& pos) const {
    return pos.x >= 0 && pos.x < width_ && pos.y >= 0 && pos.y < height_;
}

bool GameField::IsWalkable(const Position& pos) const {
    if (!IsWithinBounds(pos)) return false;
    return cells_[pos.y][pos.x].GetType() != CellType::Obstacle;
}

void GameField::SetCell(const Position& pos, CellType type) {
    if (IsWithinBounds(pos)) {
        cells_[pos.y][pos.x].SetType(type);
    }
}

CellType GameField::GetCellType(const Position& pos) const {
    if (IsWithinBounds(pos)) {
        return cells_[pos.y][pos.x].GetType();
    }
    return CellType::Obstacle;
}

void GameField::GenerateObstacles() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (int y = 0; y < height_; ++y)
        for (int x = 0; x < width_; ++x)
            if (dis(gen) < 0.15) cells_[y][x].SetType(CellType::Obstacle);

    for (int y = 0; y < std::min(height_, 3); ++y)
        for (int x = 0; x < std::min(width_, 3); ++x)
            cells_[y][x].SetType(CellType::Empty);
}

bool GameField::PlaceTrap(const Position& pos, int damage) {
    if (!IsWithinBounds(pos) || !IsWalkable(pos)) return false;
    traps_[pos.y][pos.x] = Trap(damage);
    return true;
}

bool GameField::HasTrap(const Position& pos) const {
    if (!IsWithinBounds(pos)) return false;
    return traps_[pos.y][pos.x].IsActive();  
}

int GameField::TriggerTrap(const Position& pos) {
    if (!HasTrap(pos)) return 0;
    int dmg = traps_[pos.y][pos.x].GetDamage();
    traps_[pos.y][pos.x].Trigger();  
    return dmg;
}

int GameField::GetTrapDamage(const Position& pos) const {
    if (!IsWithinBounds(pos)) return 0;
    return traps_[pos.y][pos.x].GetDamage();
}
