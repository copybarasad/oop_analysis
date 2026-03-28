#include "Field.h"
#include "Exceptions.h"
#include <random>
#include <algorithm>

Field::Field(int width, int height) : width_(width), height_(height) {
    resize(width, height);
}

void Field::resize(int width, int height) {
    if (width < 10 || width > 50 || height < 10 || height > 50) {
         width = std::max(10, std::min(width, 50));
         height = std::max(10, std::min(height, 50));
    }
    width_ = width;
    height_ = height;
    grid_.assign(height_, std::vector<Cell>(width_));
}

int Field::getWidth() const {
    return width_;
}

int Field::getHeight() const {
    return height_;
}

CellType Field::getCellType(int x, int y) const {
    if (!isValid(x, y)) {
        throw OutOfBoundsException(x, y, width_, height_);
    }
    return grid_[y][x].getType();
}

bool Field::isValid(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

void Field::populateField(int impassable_count, int slowing_count, const Point& player_pos) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_w(0, width_ - 1);
    std::uniform_int_distribution<> dis_h(0, height_ - 1);
    
    for(auto& row : grid_) for(auto& cell : row) cell.setType(CellType::kEmpty);

    auto place_cells = [&](int count, CellType type) {
        for (int i = 0; i < count; ++i) {
            int x, y;
            int attempts = 0;
            do {
                x = dis_w(gen);
                y = dis_h(gen);
                attempts++;
            } while ( ((x == player_pos.x && y == player_pos.y) || grid_[y][x].getType() != CellType::kEmpty) && attempts < 100);
            
            if (attempts < 100) grid_[y][x].setType(type);
        }
    };
    
    place_cells(impassable_count, CellType::kImpassable);
    place_cells(slowing_count, CellType::kSlowing);
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
    if (!(is >> width_ >> height_)) {
        throw FileFormatException("Unknown Source", "Failed to read field dimensions.");
    }
    
    if (width_ <= 0 || height_ <= 0 || width_ > 100 || height_ > 100) {
        throw FileFormatException("Unknown Source", "Invalid field dimensions read: " + std::to_string(width_) + "x" + std::to_string(height_));
    }

    resize(width_, height_);
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            int typeVal;
            if (!(is >> typeVal)) {
                throw FileFormatException("Unknown Source", "Failed to read cell data at position (" + std::to_string(x) + "," + std::to_string(y) + ")");
            }
            grid_[y][x].setType(static_cast<CellType>(typeVal));
        }
    }
}