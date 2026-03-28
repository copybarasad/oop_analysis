#include "Field.hpp"
#include <random>
#include <algorithm>
#include <stdexcept>

Field::Field() : width_(0), height_(0) {
    initializeCells();
}

Field::Field(int width, int height, int startX, int startY)
    : width_(width), height_(height)
{
    if (width_ < 10 || width_ > 25 || height_ < 10 || height_ > 25) {
        throw std::invalid_argument("Field size must be between 10x10 and 25x25");
    }

    if (!isValidPosition(startX, startY)) {
        throw std::invalid_argument("Invalid start position");
    }

    initializeCells();
    placeImpassableCells();
    placeSlowingCells();

    if (!cells_[startY][startX].isPassable()) {
        throw std::invalid_argument("Start position is impassable");
    }

    cells_[startY][startX].setHasPlayer(true);
}

Field::Field(const Field& other)
    : width_(other.width_), height_(other.height_), cells_(other.cells_) {}

Field::Field(Field&& other) noexcept
    : width_(other.width_), height_(other.height_), cells_(std::move(other.cells_)) {
    other.width_ = 0;
    other.height_ = 0;
}

Field& Field::operator=(const Field& other) {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = other.cells_;
    }
    return *this;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this != &other) {
        width_ = other.width_;
        height_ = other.height_;
        cells_ = std::move(other.cells_);
        other.width_ = 0;
        other.height_ = 0;
    }
    return *this;
}

void Field::initializeCells() {
    cells_.resize(height_);
    for (int y = 0; y < height_; y++) {
        cells_[y].resize(width_);
        for (int x = 0; x < width_; x++) {
            cells_[y][x] = Cell(CellType::Empty);
        }
    }
}

void Field::placeImpassableCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width_ * height_ - 1);
    
    int impassableCount = (width_ * height_) / 10;

    for (int i = 0; i < impassableCount; i++) {
        int pos = dis(gen);
        int x = pos % width_;
        int y = pos / width_;

        if (isValidPosition(x, y) && cells_[y][x].getType() == CellType::Empty) {
            cells_[y][x].setType(CellType::Impassable);
        }
    }
}

void Field::placeSlowingCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width_ * height_ - 1);
    
    int slowingCount = (width_ * height_) / 15;

    for (int i = 0; i < slowingCount; i++) {
        int pos = dis(gen);
        int x = pos % width_;
        int y = pos / width_;

        if (isValidPosition(x, y) && cells_[y][x].getType() == CellType::Empty) {
            cells_[y][x].setType(CellType::Slowing);
        }
    }
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

int Field::getWidth() const {
    return width_; 
}

int Field::getHeight() const { 
    return height_; 
}

const Cell& Field::getCell(int x, int y) const {
    if (!isValidPosition(x, y)) {
        throw std::invalid_argument("Invalid cell coordinates");
    }
    return cells_[y][x];
}

Cell& Field::getCell(int x, int y) {
    if (!isValidPosition(x, y)) {
        throw std::invalid_argument("Invalid cell coordinates");
    }
    return cells_[y][x];
}

Direction Field::getRandomDirection() const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    
    switch (dis(gen)) {
        case 0: return Direction::Up;
        case 1: return Direction::Down;
        case 2: return Direction::Left;
        case 3: return Direction::Right;
        default: return Direction::Up;
    }
}

void Field::serialize(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&width_), sizeof(width_));
    os.write(reinterpret_cast<const char*>(&height_), sizeof(height_));
    
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            CellType type = cells_[y][x].getType();
            os.write(reinterpret_cast<const char*>(&type), sizeof(type));
            
            bool hasPlayer = cells_[y][x].hasPlayer();
            bool hasEnemy = cells_[y][x].hasEnemy();
            bool hasAlly = cells_[y][x].hasAlly();
            bool hasBuilding = cells_[y][x].hasBuilding();
            
            os.write(reinterpret_cast<const char*>(&hasPlayer), sizeof(hasPlayer));
            os.write(reinterpret_cast<const char*>(&hasEnemy), sizeof(hasEnemy));
            os.write(reinterpret_cast<const char*>(&hasAlly), sizeof(hasAlly));
            os.write(reinterpret_cast<const char*>(&hasBuilding), sizeof(hasBuilding));
        }
    }
}

void Field::deserialize(std::istream& is) {
    is.read(reinterpret_cast<char*>(&width_), sizeof(width_));
    is.read(reinterpret_cast<char*>(&height_), sizeof(height_));
    
    cells_.resize(height_);
    for (int y = 0; y < height_; ++y) {
        cells_[y].resize(width_);
        for (int x = 0; x < width_; ++x) {
            CellType type;
            is.read(reinterpret_cast<char*>(&type), sizeof(type));
            cells_[y][x] = Cell(type);
            
            bool hasPlayer, hasEnemy, hasAlly, hasBuilding;
            is.read(reinterpret_cast<char*>(&hasPlayer), sizeof(hasPlayer));
            is.read(reinterpret_cast<char*>(&hasEnemy), sizeof(hasEnemy));
            is.read(reinterpret_cast<char*>(&hasAlly), sizeof(hasAlly));
            is.read(reinterpret_cast<char*>(&hasBuilding), sizeof(hasBuilding));
            
            cells_[y][x].setHasPlayer(hasPlayer);
            cells_[y][x].setHasEnemy(hasEnemy);
            cells_[y][x].setHasAlly(hasAlly);
            cells_[y][x].setHasBuilding(hasBuilding);
        }
    }
}