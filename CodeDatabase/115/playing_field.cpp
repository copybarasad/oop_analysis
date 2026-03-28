#include "playing_field.h"
#include "cell.h"
#include "wall_generator.h"
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <iostream>

Playing_field::Playing_field(int width, int length) 
    : width(width+2), length(length+2){

    static bool seeded = false;
    if (!seeded) {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        seeded = true;
    }

    validate_size(width, length);
    initialize_grid();
}

Playing_field::Playing_field(const Playing_field& other) 
    : width(other.width), length(other.length), grid(other.grid) {}

Playing_field::Playing_field(Playing_field&& other) noexcept
    : width(other.width), length(other.length), grid(std::move(other.grid)){
    other.width = 0;
    other.length = 0;
}

Playing_field& Playing_field::operator=(const Playing_field& other){
    if (this != &other) {
        width = other.width;
        length = other.length;
        grid = other.grid; 
    }
    return *this;
}

Playing_field& Playing_field::operator=(Playing_field&& other) noexcept {   
    if (this != &other) {
        width = other.width;
        length = other.length;
        grid = std::move(other.grid);
        other.width = 0;
        other.length = 0;
    }
    return *this;
}

int Playing_field::get_width() const {return width;}
int Playing_field::get_length() const {return length;}

Cell& Playing_field::get_cell(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= length) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return grid[y][x];
}

const Cell& Playing_field::get_cell(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= length) {
        throw std::out_of_range("Cell coordinates out of range");
    }
    return grid[y][x];
}

void Playing_field::validate_size(int width, int length) const {
    if (width<10 || length<10 || width>25 || length>25) {
        throw std::invalid_argument("Invalid field size");
    }
}

void Playing_field::initialize_grid() {
    grid.resize(length, std::vector<Cell>(width));
    for (int y = 0; y < length; ++y) {
        for (int x = 0; x < width; ++x) {
            grid[y][x] = Cell(x, y);
        }
    }
    
    WallGenerator::generateWalls(*this);
}

