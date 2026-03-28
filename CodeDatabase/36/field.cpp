#include "field.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>

Field::Field(int width, int height)
    : width_(width), height_(height) {
    if (width_ < 10 || width_ > 50 || height_ < 10 || height_ > 50) {
        throw std::runtime_error("Field size must be between 10 and 50");
    }
    grid_.assign(height_, std::vector<Cell>(width_));
    Initialize();
}

Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_), grid_(other.grid_) {}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = other.grid_;
    }
    return *this;
}

Field::Field(Field&& other) noexcept
    : width_(other.width_), height_(other.height_), grid_(std::move(other.grid_)) {
    other.width_ = 0;
    other.height_ = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        grid_ = std::move(other.grid_);

        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

int Field::GetWidth() const { return width_; }
int Field::GetHeight() const { return height_; }

bool Field::IsInBounds(int r, int c) const {
    return r >= 0 && r < height_ && c >= 0 && c < width_;
}

Cell& Field::CellAt(int r, int c) {
    if (!IsInBounds(r, c)) throw std::out_of_range("CellAt out of bounds");
    return grid_[r][c];
}

const Cell& Field::CellAt(int r, int c) const {
    if (!IsInBounds(r, c)) throw std::out_of_range("CellAt out of bounds");
    return grid_[r][c];
}

void Field::Print() const {
    std::cout << "======================================\n";
    std::cout << "=              GAME MAP              =\n";
    std::cout << "======================================\n";
    std::cout << "\n";

    if (width_ > 20 || height_ > 20) {
        std::cout << "Map too large to display fully.\n";
        std::cout << "Field size: " << width_ << "x" << height_ << "\n";
    } else {
        std::cout << " ";
        for (int i = 0; i < width_; i++) {
            std::cout << " " << i%10;
        }
        std::cout << "\n";
        for (int r = 0; r < height_; ++r) {
            std::cout << r%10 << ' ';
            for (int c = 0; c < width_; ++c) {
                char ch = '_';
                switch (grid_[r][c].GetType()) {
                    case CellType::Player: ch = 'P'; break;
                    case CellType::Enemy: ch = 'E'; break;
                    case CellType::Wall: ch = '#'; break;
                    case CellType::EnemyBase: ch = 'B'; break;
                    case CellType::Slow: ch = '~'; break;
                    case CellType::Trap: ch = 'T'; break;
                    case CellType::Ally: ch = 'A'; break;
                    case CellType::Tower: ch = 'W'; break;
                    default: ch = '_';
                }
                std::cout << ch;
                std::cout << ' ';
            }
            std::cout << '\n';
        }
    }

    std::cout << "\n================ LEGEND ================\n";
    std::cout << "P = Player    E = Enemy    B = Enemy Base\n";
    std::cout << "# = Wall      ~ = Slow     T = Trap\n";
    std::cout << "A = Ally      _ = Empty    W = Tower\n";
    std::cout << "=======================================\n";
}

void Field::Initialize() {
    for (auto &row : grid_)
        for (auto &cell : row)
            cell.Clear();

    for (int i = 0; i < 3; ++i) {
        int r = rand() % height_;
        int c = rand() % width_;
        grid_[r][c].SetType(CellType::Wall);
    }

    for (int i = 0; i < 2; ++i) {
        int r = rand() % height_;
        int c = rand() % width_;
        if (grid_[r][c].GetType() == CellType::Empty)
            grid_[r][c].SetType(CellType::Slow);
    }
}

void Field::Initialize(int level) {
    for (auto &row : grid_)
        for (auto &cell : row)
            cell.Clear();

    int num_walls = 3 + 2 * (level - 1);
    for (int i = 0; i < num_walls; ++i) {
        int r = rand() % height_;
        int c = rand() % width_;
        grid_[r][c].SetType(CellType::Wall);
    }

    int num_slow = 2 + 2 * (level - 1);
    for (int i = 0; i < num_slow; ++i) {
        int r = rand() % height_;
        int c = rand() % width_;
        if (grid_[r][c].GetType() == CellType::Empty)
            grid_[r][c].SetType(CellType::Slow);
    }
}
