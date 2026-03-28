#include "field.hpp"
#include <utility>
#include "constants.hpp"

int Field::width() const { return width_; }
int Field::height() const { return height_; }

Field::Field(int width, int height)
    : width_(width), height_(height) {
    if (width < FIELD_MIN_SIZE || height < FIELD_MIN_SIZE ||
        width > FIELD_MAX_SIZE || height > FIELD_MAX_SIZE)
        throw std::invalid_argument("Размер поля должен быть от 10 до 25");

    this->grid_.resize(this->height_, std::vector<Cell>(this->width_, Cell(CellType::kEmpty)));

    int numWalls = (this->width_ * this->height_) / WALL_PERCENTAGE_DIVISOR;
    for (int i = 0; i < numWalls; ++i) {
        int x = std::rand() % this->width_;
        int y = std::rand() % this->height_;
        this->grid_[y][x].SetType(CellType::kWall);
    }

    int numSlow = (this->width_ * this->height_) / SLOW_PERCENTAGE_DIVISOR;
    for (int i = 0; i < numSlow; ++i) {
        int x = std::rand() % this->width_;
        int y = std::rand() % this->height_;
        if (this->grid_[y][x].type() == CellType::kEmpty)
            this->grid_[y][x].SetType(CellType::kSlowCell);
    }
}

Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_), grid_(other.grid_) {}

Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    grid_ = other.grid_;
    return *this;
}

Field::Field(Field&& other) noexcept
    : width_(other.width_), height_(other.height_), grid_(std::move(other.grid_)) {
    other.width_ = FIELD_MIN;
    other.height_ = FIELD_MIN;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    grid_ = std::move(other.grid_);
    other.width_ = FIELD_MIN;
    other.height_ = FIELD_MIN;
    return *this;
}

Cell& Field::At(int x, int y) {
    if (x < FIELD_MIN || y < FIELD_MIN || x >= this->width_ || y >= this->height_)
        throw std::out_of_range("Координаты вне поля!");
    return this->grid_.at(y).at(x);
}

const Cell& Field::At(int x, int y) const { return this->grid_.at(y).at(x); }

void Field::Clear() {
    for (auto& row : this->grid_)
        for (auto& cell : row)
            cell.SetType(CellType::kEmpty);
}

void Field::Print() const {
    std::cout << "\n";
    for (int y = 0; y < this->height_; ++y) {
        for (int x = 0; x < this->width_; ++x)
            std::cout << this->grid_[y][x].Symbol() << ' ';
        std::cout << "\n";
    }
}