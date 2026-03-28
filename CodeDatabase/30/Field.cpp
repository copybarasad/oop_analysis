#include <stdexcept>
#include <random>
#include "../include/Field.h"

void Field::validateSize(int rows, int cols) const {
    if (rows < 10 || rows > 25 || cols < 10 || cols > 25) {
        throw std::invalid_argument("Field size must be between 10 and 25 inclusive");
    }
}

Field::Field(int rows, int cols) : rows_(rows), cols_(cols), grid_(rows, std::vector<Cell>(cols)) {
    validateSize(rows, cols);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 99);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            if (dist(gen) < 15) {
                grid_[r][c].setPassable(false);
            } else if (dist(gen) < 25) {
                grid_[r][c].setSlowing(true);
            }
        }
    }
}

Field::Field(const Field& other) : rows_(other.rows_), cols_(other.cols_), grid_(other.grid_) {}

bool Field::inBounds(const Position& p) const {
    return p.row >= 0 && p.row < rows_ && p.col >= 0 && p.col < cols_;
}

const Cell& Field::cellAt(const Position& p) const {
    if (!inBounds(p)) throw std::out_of_range("Position out of bounds");
    return grid_[p.row][p.col];
}

Cell& Field::cellAt(const Position& p) {
    if (!inBounds(p)) throw std::out_of_range("Position out of bounds");
    return grid_[p.row][p.col];
}

int Field::rows() const { return rows_; }

int Field::cols() const { return cols_; }

std::optional<Position> Field::randomFreePosition() const {
    std::vector<Position> candidates;
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (grid_[r][c].isPassable()) {
                candidates.push_back({r, c});
            }
        }
    }
    if (candidates.empty()) return std::nullopt;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, static_cast<int>(candidates.size()) - 1);
    return candidates[dist(gen)];
}

void Field::save(std::ostream& os) const {
    os << rows_ << " " << cols_ << "\n";
    for (const auto& row : grid_) {
        for (const auto& cell : row) {
            cell.save(os);
        }
    }
}

void Field::load(std::istream& is) {
    is >> rows_ >> cols_;
    validateSize(rows_, cols_);
    grid_.assign(rows_, std::vector<Cell>(cols_));
    for (auto& row : grid_) {
        for (auto& cell : row) {
            cell.load(is);
        }
    }
}

