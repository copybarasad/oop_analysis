#include "Field.h"
#include "EventLogger.h" 
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

Field::Field(int w, int h) : width(w), height(h), grid(nullptr) {
    if (w < 10) width = 10;
    if (w > 25) width = 25;
    if (h < 10) height = 10;
    if (h > 25) height = 25;

    allocateGrid();

    
    std::srand(std::time(nullptr));
    const double wallProbability = 0.15;
    const double trapProbability = 0.05;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i == 0 || j == 0 || i == height - 1 || j == width - 1) continue;
            
            double randomValue = static_cast<double>(std::rand()) / RAND_MAX;

            if (randomValue < wallProbability) {
                grid[i][j].setType(CellType::WALL);
            } else if (randomValue < wallProbability + trapProbability) {
                grid[i][j].setType(CellType::SLOW);
            }
        }
    }
}

Field::~Field() {
    deallocateGrid();
}

void Field::allocateGrid() {
    grid = new Cell*[height];
    for (int i = 0; i < height; ++i) {
        grid[i] = new Cell[width];
    }
}

void Field::deallocateGrid() {
    if (grid != nullptr) {
        for (int i = 0; i < height; ++i) {
            delete[] grid[i];
        }
        delete[] grid;
        grid = nullptr;
    }
}

void Field::copyFrom(const Field& other) {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            grid[i][j] = other.grid[i][j];
        }
    }
}

Field::Field(const Field& other) : width(other.width), height(other.height), grid(nullptr) {
    allocateGrid();
    copyFrom(other);
}

Field& Field::operator=(const Field& other) {
    if (this == &other) {
        return *this;
    }
    deallocateGrid();
    width = other.width;
    height = other.height;
    allocateGrid();
    copyFrom(other);
    return *this;
}

Field::Field(Field&& other) noexcept : width(other.width), height(other.height), grid(other.grid) {
    other.grid = nullptr;
    other.width = 0;
    other.height = 0;
}

Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) {
        return *this;
    }
    deallocateGrid();
    width = other.width;
    height = other.height;
    grid = other.grid;
    other.grid = nullptr;
    other.width = 0;
    other.height = 0;
    return *this;
}

bool Field::save(std::ofstream& out, EventLogger& logger) const {
    if (!out.good()) {
        logger.log(LogMsg::FIELD_SAVE_ERROR);
        return false;
    }
    out << width << " " << height << "\n";
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            out << static_cast<int>(grid[i][j].getType()) << " ";
        }
        out << "\n";
    }
    return true;
}

bool Field::load(std::ifstream& in, EventLogger& logger) {
    int w, h;
    if (!(in >> w >> h)) {
        logger.log(LogMsg::FIELD_LOAD_ERROR);
        return false;
    }
    
    if (w < 10 || w > 50 || h < 10 || h > 50) {
        logger.log(LogMsg::FIELD_INVALID_SIZE);
        return false; 
    }

    *this = Field(w, h); 

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int typeVal;
            if (!(in >> typeVal)) {
                logger.log(LogMsg::FIELD_LOAD_ERROR);
                return false;
            }
            grid[i][j].setType(static_cast<CellType>(typeVal));
        }
    }
    return true;
}

void Field::print() const {
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            std::cout << static_cast<char>(grid[i][j].getType()) << ' ';
        }
        std::cout << std::endl;
    }
}

bool Field::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

CellType Field::getCellType(int x, int y) const {
    if (!isValidPosition(x, y)) {
        return CellType::WALL;
    }
    return grid[y][x].getType();
}

void Field::setCellType(int x, int y, CellType type) {
    if (isValidPosition(x, y)) {
        grid[y][x].setType(type);
    }
}

int Field::getWidth() const {return width;}
int Field::getHeight() const {return height;}