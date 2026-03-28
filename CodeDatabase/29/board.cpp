#include "board.hpp"
#include <random>

Board::Board(int width, int height, double wall_density, unsigned seed)
    : width_(width), height_(height), cells_() {
    if (width_ < 10 || width_ > 25 || height_ < 10 || height_ > 25) {
        throw std::invalid_argument("Board: width/height must be in [10,25]");
    }
    if (wall_density < 0.0 || wall_density > 0.5) {
        throw std::invalid_argument("Board: wall_density must be in [0.0, 0.5]");
    }

    cells_.assign(static_cast<size_t>(width_) * static_cast<size_t>(height_), Cell(CellType::kFloor));

    std::mt19937 gen(seed ? seed : std::random_device{}());
    std::uniform_real_distribution<double> dist(0.0, 1.0);

    int floor_count = 0;
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            double r = dist(gen);
            if (r < wall_density) {
                cells_[Index(x, y)].SetType(CellType::kWall);
            } else {
                cells_[Index(x, y)].SetType(CellType::kFloor);
                ++floor_count;
            }
        }
    }
    if (floor_count == 0) {
        cells_[0].SetType(CellType::kFloor);
    }
}

Board::Board(const Board& other)
    : width_(other.width_), height_(other.height_), cells_(other.cells_) {}

Board& Board::operator=(const Board& other) {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    cells_ = other.cells_;
    return *this;
}

Board::Board(Board&& other) noexcept
    : width_(other.width_), height_(other.height_), cells_(std::move(other.cells_)) {
    other.width_ = 0;
    other.height_ = 0;
}

Board& Board::operator=(Board&& other) noexcept {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    cells_ = std::move(other.cells_);
    other.width_ = 0;
    other.height_ = 0;
    return *this;
}

Board::~Board() = default;

int Board::Width() const noexcept { return width_; }

int Board::Height() const noexcept { return height_; }

bool Board::InBounds(int x, int y) const noexcept {
    return x >= 0 && y >= 0 && x < width_ && y < height_;
}

bool Board::IsPassable(int x, int y) const {
    if (!InBounds(x, y)) return false;
    return cells_[Index(x, y)].IsPassable();
}

void Board::SetWall(int x, int y) {
    if (!InBounds(x, y)) throw std::out_of_range("SetWall: out of bounds");
    cells_[Index(x, y)].SetType(CellType::kWall);
}

void Board::SetFloor(int x, int y) {
    if (!InBounds(x, y)) throw std::out_of_range("SetFloor: out of bounds");
    cells_[Index(x, y)].SetType(CellType::kFloor);
}

CellType Board::GetCellType(int x, int y) const {
    if (!InBounds(x, y)) throw std::out_of_range("GetCellType: out of bounds");
    return cells_[Index(x, y)].Type();
}

int Board::Index(int x, int y) const noexcept { return y * width_ + x; }