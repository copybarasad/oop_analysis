#include "game_field.h"
#include <algorithm>
#include <random>
#include <stdexcept>

namespace rpg {
    GameField::GameField(size_t width, size_t height,
                         double impassable_density,
                         double slowing_density)
        : width_(std::clamp(width, kMinFieldSize, kMaxFieldSize)),
          height_(std::clamp(height, kMinFieldSize, kMaxFieldSize)),
          cells_(nullptr) {
        AllocateCells();
        GenerateRandomCells(impassable_density, slowing_density);
    }

    GameField::~GameField() {
        DeallocateCells();
    }

    GameField::GameField(const GameField &other)
        : width_(other.width_),
          height_(other.height_),
          cells_(nullptr) {
        AllocateCells();
        DeepCopy(other);
    }

    GameField &GameField::operator=(const GameField &other) {
        if (this != &other) {
            DeallocateCells();

            width_ = other.width_;
            height_ = other.height_;

            AllocateCells();
            DeepCopy(other);
        }
        return *this;
    }

    GameField::GameField(GameField &&other) noexcept
        : width_(other.width_),
          height_(other.height_),
          cells_(other.cells_) {
        other.cells_ = nullptr;
        other.width_ = kMinFieldSize;
        other.height_ = kMinFieldSize;
    }

    GameField &GameField::operator=(GameField &&other) noexcept {
        if (this != &other) {
            DeallocateCells();

            width_ = other.width_;
            height_ = other.height_;
            cells_ = other.cells_;

            other.cells_ = nullptr;
            other.width_ = kMinFieldSize;
            other.height_ = kMinFieldSize;
        }
        return *this;
    }

    [[nodiscard]] size_t GameField::GetWidth() const {
        return width_;
    }

    [[nodiscard]] size_t GameField::GetHeight() const {
        return height_;
    }

    [[nodiscard]] bool GameField::IsInBounds(const size_t x, const size_t y) const {
        return x < width_ && y < height_;
    }

    [[nodiscard]] bool GameField::IsPassable(const size_t x, const size_t y) const {
        if (!IsInBounds(x, y)) {
            return false;
        }
        return cells_[y][x].IsPassable();
    }

    [[nodiscard]] bool GameField::IsSlowing(const size_t x, const size_t y) const {
        if (!IsInBounds(x, y)) {
            return false;
        }
        return cells_[y][x].IsSlowing();
    }

    [[nodiscard]] const Cell &GameField::GetCell(size_t x, size_t y) const {
        if (!IsInBounds(x, y)) {
            throw std::out_of_range("Cell coordinates out of bounds");
        }
        return cells_[y][x];
    }

    void GameField::SetCellType(const size_t x, const size_t y, const CellType type) const {
        if (!IsInBounds(x, y)) {
            throw std::out_of_range("Cell coordinates out of bounds");
        }
        cells_[y][x].SetType(type);
    }

    void GameField::AllocateCells() {
        cells_ = new Cell *[height_];
        for (size_t i = 0; i < height_; ++i) {
            cells_[i] = new Cell[width_];
        }
    }

    void GameField::DeallocateCells() {
        if (cells_ != nullptr) {
            for (size_t i = 0; i < height_; ++i) {
                delete[] cells_[i];
            }
            delete[] cells_;
            cells_ = nullptr;
        }
    }

    void GameField::DeepCopy(const GameField &other) const {
        for (size_t y = 0; y < height_; ++y) {
            for (size_t x = 0; x < width_; ++x) {
                cells_[y][x] = other.cells_[y][x];
            }
        }
    }

    void GameField::GenerateRandomCells(double impassable_density,
                                        double slowing_density) const {
        impassable_density = std::clamp(impassable_density, 0.0, kMaxImpassableDensity);
        slowing_density = std::clamp(slowing_density, 0.0, kMaxSlowingDensity);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);

        for (size_t y = 0; y < height_; ++y) {
            for (size_t x = 0; x < width_; ++x) {
                double rand_val = dis(gen);

                if (rand_val < impassable_density) {
                    cells_[y][x].SetType(CellType::kImpassable);
                } else if (rand_val < impassable_density + slowing_density) {
                    cells_[y][x].SetType(CellType::kSlowing);
                } else {
                    cells_[y][x].SetType(CellType::kNormal);
                }
            }
        }
    }

    void GameField::ClearArea(size_t center_x, size_t center_y) {
        const int cx = static_cast<int>(center_x);
        const int cy = static_cast<int>(center_y);

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int x = cx + dx;
                int y = cy + dy;

                if (x >= 0 && y >= 0 &&
                    x < static_cast<int>(width_) &&
                    y < static_cast<int>(height_)) {
                    cells_[y][x].SetType(CellType::kNormal);
                }
            }
        }
    }
}
