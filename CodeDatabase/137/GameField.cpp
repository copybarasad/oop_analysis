#include "GameField.h"
#include <stdexcept>
#include <random>

GameField::GameField(int width, int height)
    : width_(width), height_(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }
    InitializeCells();
}

GameField::GameField(const GameField& other)
    : width_(other.width_),
      height_(other.height_),
      cells_(other.cells_) {}

GameField::GameField(GameField&& other) noexcept
    : width_(other.width_),
      height_(other.height_),
      cells_(std::move(other.cells_)) {
    other.width_ = 0;
    other.height_ = 0;
}

GameField::~GameField() = default;

GameField& GameField::operator=(const GameField& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = other.cells_;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = std::move(other.cells_);
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

void GameField::InitializeCells() {
    cells_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        cells_[y].resize(width_);
        for (int x = 0; x < width_; ++x) {
            cells_[y][x] = Cell(CellType::NORMAL);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> wall_dist(0, 100);
    std::uniform_int_distribution<> slow_dist(0, 100);

    for (int y = 1; y < height_ - 1; ++y) {
        for (int x = 1; x < width_ - 1; ++x) {
            if (wall_dist(gen) < 15) {
                cells_[y][x].SetType(CellType::WALL);
            } else if (slow_dist(gen) < 5) {
                cells_[y][x].SetType(CellType::SLOWING);
            }
        }
    }

    for (int x = 0; x < width_; ++x) {
        cells_[0][x].SetType(CellType::WALL);
        cells_[height_ - 1][x].SetType(CellType::WALL);
    }
    for (int y = 0; y < height_; ++y) {
        cells_[y][0].SetType(CellType::WALL);
        cells_[y][width_ - 1].SetType(CellType::WALL);
    }
}

int GameField::GetWidth() const {
    return width_;
}

int GameField::GetHeight() const {
    return height_;
}

Cell& GameField::GetCell(int x, int y) {
    if (!IsValidPosition(x, y)) {
        throw std::out_of_range("Cell position out of range");
    }
    return cells_[y][x];
}

const Cell& GameField::GetCell(int x, int y) const {
    if (!IsValidPosition(x, y)) {
        throw std::out_of_range("Cell position out of range");
    }
    return cells_[y][x];
}

bool GameField::IsValidPosition(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

bool GameField::IsPassable(int x, int y) const {
    if (!IsValidPosition(x, y)) {
        return false;
    }
    return cells_[y][x].IsPassable();
}

void GameField::Serialize(std::ostream& stream) const {
    stream << width_ << " " << height_ << "\n";

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            SerializeCell(stream, cells_[y][x]);
            if (x < width_ - 1) {
                stream << " ";
            }
        }
        stream << "\n";
    }
}

void GameField::Deserialize(std::istream& stream) {
    stream >> width_ >> height_;

    if (width_ < 10 || width_ > 25 || height_ < 10 || height_ > 25) {
        throw std::runtime_error("Invalid field dimensions in save file");
    }

    cells_.clear();
    cells_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        cells_[y].resize(width_);
        for (int x = 0; x < width_; ++x) {
            DeserializeCell(stream, cells_[y][x]);
        }
    }
}

void GameField::SerializeCell(std::ostream& stream, const Cell& cell) const {
    int cell_type = static_cast<int>(cell.GetType());
    stream << cell_type << " "
           << (cell.HasPlayer() ? 1 : 0) << " "
           << (cell.HasEnemy() ? 1 : 0) << " "
           << (cell.HasEnemyBuilding() ? 1 : 0) << " "
           << (cell.HasEnemyTower() ? 1 : 0) << " "
           << (cell.HasAlly() ? 1 : 0) << " "
           << (cell.HasTrap() ? 1 : 0) << " "
           << cell.GetTrapDamage();
}

void GameField::DeserializeCell(std::istream& stream, Cell& cell) {
    int type;
    int has_player, has_enemy, has_building, has_tower, has_ally, has_trap;
    int trap_damage;

    stream >> type >> has_player >> has_enemy >> has_building 
           >> has_tower >> has_ally >> has_trap >> trap_damage;

    if (stream.fail()) {
        throw std::runtime_error("Failed to read cell data from save file");
    }

    cell.SetType(static_cast<CellType>(type));
    cell.SetPlayer(has_player != 0);
    cell.SetEnemy(has_enemy != 0);
    cell.SetEnemyBuilding(has_building != 0);
    cell.SetEnemyTower(has_tower != 0);
    cell.SetAlly(has_ally != 0);
    cell.SetTrap(has_trap != 0, trap_damage);
}
