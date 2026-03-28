#include "GameField.h"

#include <stdexcept>

GameField::GameField(std::size_t width, std::size_t height)
    : width_(width),
      height_(height),
      cells_(width * height) {
    InitializeTerrain();
    EnsureInvariants();
}

GameField::GameField(const GameField &other)
    : width_(other.width_),
      height_(other.height_),
      cells_(other.cells_) {
    EnsureInvariants();
}

GameField::GameField(GameField &&other) noexcept
    : width_(other.width_),
      height_(other.height_),
      cells_(std::move(other.cells_)) {
    other.width_ = 0;
    other.height_ = 0;
}

GameField &GameField::operator=(const GameField &other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = other.cells_;
        EnsureInvariants();
    }
    return *this;
}

GameField &GameField::operator=(GameField &&other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = std::move(other.cells_);
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

GameField::Size GameField::GetSize() const {
    return Size{width_, height_};
}

bool GameField::IsInside(std::size_t x, std::size_t y) const {
    return x < width_ && y < height_;
}

// --- Террейн / сущности ---

Cell::Terrain GameField::GetTerrain(std::size_t x, std::size_t y) const {
    return cells_[GetIndex(x, y)].GetTerrain();
}

Cell::Occupant GameField::GetOccupant(std::size_t x, std::size_t y) const {
    return cells_[GetIndex(x, y)].GetOccupant();
}

int GameField::GetOccupantIndex(std::size_t x, std::size_t y) const {
    return cells_[GetIndex(x, y)].GetOccupantIndex();
}

void GameField::SetTerrain(std::size_t x, std::size_t y,
                           Cell::Terrain terrain) {
    cells_[GetIndex(x, y)].SetTerrain(terrain);
}

void GameField::SetOccupant(std::size_t x, std::size_t y,
                            Cell::Occupant occupant, int index) {
    Cell &cell = cells_[GetIndex(x, y)];
    cell.SetOccupant(occupant);
    cell.SetOccupantIndex(index);
}

void GameField::ClearCell(std::size_t x, std::size_t y) {
    Cell &cell = cells_[GetIndex(x, y)];
    cell.SetOccupant(Cell::Occupant::kNone);
    cell.SetOccupantIndex(-1);
}

bool GameField::FindPlayer(std::size_t &x, std::size_t &y) const {
    for (std::size_t row = 0; row < height_; ++row) {
        for (std::size_t col = 0; col < width_; ++col) {
            const Cell &cell = cells_[GetIndex(col, row)];
            if (cell.GetOccupant() == Cell::Occupant::kPlayer) {
                x = col;
                y = row;
                return true;
            }
        }
    }
    return false;
}

// --- Ловушки ---

bool GameField::HasTrap(std::size_t x, std::size_t y) const {
    return cells_[GetIndex(x, y)].HasTrap();
}

int GameField::GetTrapDamage(std::size_t x, std::size_t y) const {
    return cells_[GetIndex(x, y)].GetTrapDamage();
}

void GameField::PlaceTrap(std::size_t x, std::size_t y, int damage) {
    cells_[GetIndex(x, y)].SetTrap(damage);
}

void GameField::ClearTrap(std::size_t x, std::size_t y) {
    cells_[GetIndex(x, y)].ClearTrap();
}

// --- Внутренняя логика ---

std::size_t GameField::GetIndex(std::size_t x, std::size_t y) const {
    if (!IsInside(x, y)) {
        throw std::out_of_range("GameField::GetIndex: coordinates out of range");
    }
    return y * width_ + x;
}

void GameField::InitializeTerrain() {
    for (std::size_t y = 0; y < height_; ++y) {
        for (std::size_t x = 0; x < width_; ++x) {
            Cell &cell = cells_[GetIndex(x, y)];
            cell.SetOccupant(Cell::Occupant::kNone);
            cell.SetOccupantIndex(-1);

            if (x == y && x > 0 && x + 1 < width_ && y + 1 < height_) {
                cell.SetTerrain(Cell::Terrain::kBlocked);
            } else if (y == height_ - 2 && x > 0 && x + 1 < width_) {
                cell.SetTerrain(Cell::Terrain::kSlow);
            } else {
                cell.SetTerrain(Cell::Terrain::kEmpty);
            }
        }
    }
}

void GameField::EnsureInvariants() const {
    if (width_ == 0 || height_ == 0) {
        throw std::logic_error("GameField size must be positive.");
    }
    if (width_ * height_ != cells_.size()) {
        throw std::logic_error("GameField cells count invariant violated.");
    }
}
