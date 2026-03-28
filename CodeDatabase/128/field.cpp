#include "field.hpp"
#include <stdexcept>
#include <cstdlib> 

Field::Field(int width, int height)
    : width_(width), height_(height), grid_(height, std::vector<Cell>(width)) 
{
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("Field: size must be between 10 and 25");
    }
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            if (rand() % 8 == 0) { 
                grid_[y][x].set(CellType::kWall);
            } else if (rand() % 15 == 0) {
                grid_[y][x].set(CellType::kSlow);
            }
        }
    }
}

Field::Field(const Field& other)
   : width_(other.width_),
     height_(other.height_),
     grid_(other.grid_) {}

Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    grid_ = other.grid_;
    return *this;
}

Field::Field(Field&& other) noexcept
   : width_(other.width_),
     height_(other.height_), 
     grid_(std::move(other.grid_))
{
    other.width_ = 0;
    other.height_ = 0; 
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    width_ = other.width_;
    height_ = other.height_;
    grid_ = std::move(other.grid_);
    other.width_ = 0;
    other.height_ = 0; 
    return *this;
}

Cell& Field::At(int x, int y) {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Field::At: invalid coordinates");
    }
    return grid_[y][x];
}

const Cell& Field::At(int x, int y) const {
    if (x < 0 || x >= width_ || y < 0 || y >= height_) {
        throw std::out_of_range("Field::At: invalid coordinates");
    }
    return grid_[y][x];
}
