#include "Field.h"
#include <stdexcept>
#include <random>

Field::Field(int width, int height) : width_(width), height_(height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field dimensions must be between 10x10 and 25x25.");
    }
    initialize();
}

void Field::initialize() {
    grid_.assign(height_, std::vector<Cell>(width_));
    std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> distX(0, width_ - 1);
    std::uniform_int_distribution<int> distY(0, height_ - 1);

    int obstacle_count = (width_ * height_) / 10;
    for (int i = 0; i < obstacle_count; ++i) {
        int x, y;
        do { x = distX(rng); y = distY(rng); } while (getCellType(x, y) != CellType::kEmpty);
        setCellType(x, y, CellType::kObstacle);
    }

    int trap_count = (width_ * height_) / 20;
    for (int i = 0; i < trap_count; ++i) {
        int x, y;
        do { x = distX(rng); y = distY(rng); } while (getCellType(x, y) != CellType::kEmpty);
        setCellType(x, y, CellType::kSlowingTrap);
    }
}

int Field::getWidth() const { return width_; }
int Field::getHeight() const { return height_; }

CellType Field::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) return CellType::kObstacle;
    return grid_[y][x].getType();
}

void Field::setCellType(int x, int y, CellType type) {
    if (isValidPosition(x, y)) grid_[y][x].setType(type);
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void Field::save(std::ostream& os) const {
    os << width_ << " " << height_ << "\n";
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            os << static_cast<int>(cell.getType()) << " ";
        }
        os << "\n";
    }
}

void Field::load(std::istream& is) {
    is >> width_ >> height_;
    grid_.assign(height_, std::vector<Cell>(width_));

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int typeVal;
            is >> typeVal;
            grid_[y][x].setType(static_cast<CellType>(typeVal));
        }
    }
}