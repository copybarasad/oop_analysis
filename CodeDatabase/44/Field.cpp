#include "Field.h"
#include "EnemyTower.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdexcept>

constexpr int kMinFieldSize = 10;
constexpr int kMaxFieldSize = 25;

constexpr int kRandomRange = 100;
constexpr int kWallProbPct = 5;
constexpr int kSlowProbPct = 5;
constexpr int kTowerProbPct = 5;
constexpr int kSafeZone = 4;

constexpr const char* kReset = "\033[0m";
constexpr const char* kRed = "\033[31m";
constexpr const char* kGreen = "\033[32m";
constexpr const char* kMagenta = "\033[35m";
constexpr const char* kCyan = "\033[36m";
constexpr const char* kGray = "\033[90m";
constexpr const char* kYellow = "\033[33m";

Field::Field(int width, int height, int num_buildings, int enemy_health, int enemy_damage) {
    if (width < kMinFieldSize || width > kMaxFieldSize || height < kMinFieldSize || height > kMaxFieldSize) {
        throw std::invalid_argument("Field dimensions must be between 10 and 25");
    }

    width_ = width;
    height_ = height;
    num_buildings_ = num_buildings;
    spawn_enemy_health_ = enemy_health;
    spawn_enemy_damage_ = enemy_damage;
    grid_ = std::vector<std::vector<Cell>>(height_, std::vector<Cell>(width_));

    for (int i = 0; i < num_buildings; ++i) {
        auto candidates = GetAvailableCells();

        if (candidates.empty()) {
            throw std::runtime_error("No available cells to place building/tower");
        }

        int idx = rand() % candidates.size();
        auto [bx, by] = candidates[idx];

        enemy_buildings_.emplace_back(bx, by, spawn_enemy_health_, spawn_enemy_damage_);
        grid_[by][bx].SetType(CellType::BUILDING);
    }

    auto candidates = GetAvailableCells();

    if (candidates.empty()) {
        throw std::runtime_error("No available cells to place building/tower");
    }

    int idx = rand() % candidates.size();
    auto [tx, ty] = candidates[idx];

    enemy_towers_.emplace_back(tx, ty);
    grid_[ty][tx].SetType(CellType::TOWER);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (!IsCellEmpty(x, y)) {
                continue;
            }

            if (x < kSafeZone && y < kSafeZone) {
                grid_[y][x].SetType(CellType::EMPTY);
                continue;
            }

            int r = rand() % kRandomRange;

            if (r < kWallProbPct && AreNeighborsEmpty(x, y)) {
                grid_[y][x].SetType(CellType::WALL);
            } else if (r < kWallProbPct + kSlowProbPct && AreNeighborsEmpty(x, y)) {
                grid_[y][x].SetType(CellType::SLOW);
            } else if (r < kWallProbPct + kSlowProbPct + kTowerProbPct
                    && AreNeighborsEmpty(x, y)) {
                enemy_towers_.emplace_back(x, y);
                grid_[y][x].SetType(CellType::TOWER);
            } else {
                grid_[y][x].SetType(CellType::EMPTY);
            }
        }
    }
}

std::vector<std::pair<int,int>> Field::GetAvailableCells() const{
    std::vector<std::pair<int,int>> candidates;

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (x < kSafeZone && y < kSafeZone) continue;
            if (!IsCellEmpty(x, y)) continue;
            if (!AreNeighborsEmpty(x, y)) continue;
            candidates.emplace_back(x, y);
        }
    }

    return candidates;
}

Field::Field(const Field& other)
    : width_(other.width_),
      height_(other.height_),
      grid_(other.grid_),
      enemy_buildings_(other.enemy_buildings_),
      enemy_towers_(other.enemy_towers_) {
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
        enemy_buildings_ = other.enemy_buildings_;
        enemy_towers_ = other.enemy_towers_;

    }
    return *this;
}

Field::Field(Field&& other) noexcept
    : width_(other.width_),
      height_(other.height_),
      grid_(std::move(other.grid_)),
      enemy_buildings_(std::move(other.enemy_buildings_)),
      enemy_towers_(std::move(other.enemy_towers_)) {
    other.width_ = 0;
    other.height_ = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = std::move(other.grid_);
        enemy_buildings_ = std::move(other.enemy_buildings_);
        enemy_towers_ = std::move(other.enemy_towers_);

        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

std::vector<EnemyTower>& Field::GetTowers() {
    return enemy_towers_;
}

const std::vector<EnemyTower>& Field::GetTowers() const{
    return enemy_towers_;
}

std::vector<EnemyBuilding>& Field::GetBuildings() {
    return enemy_buildings_;
}

const std::vector<EnemyBuilding>& Field::GetBuildings() const {
    return enemy_buildings_;
}


int Field::GetWidth() const {
    return width_; 
}

int Field::GetHeight() const {
    return height_;
}

int Field::GetNumBuildings() const {
    return enemy_buildings_.size();
}

void Field::ClearCell(int x, int y) {
    if (IsInside(x, y)) {
        grid_[y][x].SetType(CellType::EMPTY);
    }
}

bool Field::IsInside(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool Field::IsCellEmpty(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        return false;
    }
    return grid_[y][x].GetType() == CellType::EMPTY;
}

bool Field::AreNeighborsEmpty(int x, int y) const {
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) {
                continue;
            }

            int nx = x + dx;
            int ny = y + dy;

            if (IsInside(nx, ny) && !IsCellEmpty(nx, ny)) {
                return false;
            }
        }
    }
    return true;
}

void Field::Display() const {
    std::cout << "    ";
    for (int j = 0; j < width_; ++j) {
        if (j < 10) {
            std::cout << " " << j << " ";
        } else {
            std::cout << j << " ";
        }
    }
    std::cout << "\n";
    
    std::cout << "  +";
    for (int j = 0; j < width_; ++j) {
        std::cout << "---";
    }
    std::cout << "--+\n";
    
    for (int i = 0; i < height_; ++i) {
        if (i < 10) {
            std::cout << " " << i << "|  ";
        } else {
            std::cout << i << "|  ";
        }
        
        for (int j = 0; j < width_; ++j) {
            const CellType type = grid_[i][j].GetType();
            const char* color = kReset;
            char symbol = '.';
            
            switch (type) {
                case CellType::EMPTY: color = kGray; symbol = '.'; break;
                case CellType::ENEMY: color = kRed; symbol = 'E'; break;
                case CellType::PLAYER: color = kGreen; symbol = 'P'; break;
                case CellType::WALL: color = kGray; symbol = '#'; break;
                case CellType::SLOW: color = kCyan; symbol = '~'; break;
                case CellType::BUILDING: color = kMagenta; symbol = 'B'; break;
                case CellType::TRAP: color = kYellow; symbol = '^'; break;
                case CellType::TOWER: color = kMagenta; symbol = 'T'; break;
            }
            std::cout << color << symbol << kReset << "  ";
        }
        std::cout << "|\n";
    }
    
    std::cout << "  +";
    for (int j = 0; j < width_; ++j) {
        std::cout << "---";
    }
    std::cout << "--+\n";
}

CellType Field::GetCellType(int x, int y) const {
    if (!IsInside(x, y)) {
        throw std::out_of_range("Координаты вне поля");
    }
    return grid_[y][x].GetType();
}

void Field::SetCellType(int x, int y, CellType type) {
    if (!IsInside(x, y)) {
        throw std::out_of_range("Координаты вне поля");
    }
    grid_[y][x].SetType(type);
}

int Field::CellTypeToInt(CellType t) { 
    return static_cast<int>(t);
}
CellType Field::IntToCellType(int v) {
    return static_cast<CellType>(v);
}