#include "gamefield.h"
#include <cstdlib>
#include <ctime>
#include <string>

Gamefield::Gamefield(int field_width, int field_height) 
    : width(field_width), height(field_height) {
    initialize_grid();
}

Gamefield::Gamefield(const Gamefield& other) 
    : width(other.width), height(other.height) {
    grid = other.grid;
}

Gamefield::Gamefield(Gamefield&& other) 
    : width(other.width), height(other.height) {
    grid = std::move(other.grid);
}

Gamefield& Gamefield::operator=(const Gamefield& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

Gamefield& Gamefield::operator=(Gamefield&& other) {
    if (this != &other) {
        width = other.width;
        height = other.height;
        grid = std::move(other.grid);
    }
    return *this;
}

void Gamefield::initialize_grid() {
    grid.resize(height, std::vector<Cell>(width, Cell(Cell_type::EMPTY, Position(0, 0))));
    std::srand(std::time(nullptr));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Cell_type type = (std::rand() % 5 == 0) ? Cell_type::OBSTACLE : Cell_type::EMPTY;
            grid[y][x] = Cell(type, Position(x, y));
        }
    }
}

int Gamefield::get_height() const {
    return height;
}

int Gamefield::get_width() const {
    return width;
}

bool Gamefield::is_position_accessible(const Position& position) const {
    if (!is_valid_position(position)) return false;
    int x = position.get_x();
    int y = position.get_y();
    return grid[y][x].is_accessible();
}

Cell_type Gamefield::get_cell_type(const Position& position) const {
    if (!is_valid_position(position)) return Cell_type::OBSTACLE;
    int x = position.get_x();
    int y = position.get_y();
    return grid[y][x].get_type();
}

std::vector<Position> Gamefield::get_adjacent_positions(const Position& position) const {
    std::vector<Position> adjacent;
    Position directions[] = { Position(0, 1), Position(1, 0), Position(0, -1), Position(-1, 0) };
    
    for (const auto& dir : directions) {
        Position new_pos = position + dir;
        if (is_position_accessible(new_pos)) {
            adjacent.push_back(new_pos);
        }
    }
    return adjacent;
}

bool Gamefield::is_valid_position(const Position& position) const {
    int x = position.get_x();
    int y = position.get_y();
    return x >= 0 && x < width && y >= 0 && y < height;
}

void Gamefield::create_from_data(int new_width, int new_height, const std::vector<std::string>& map_data) {
    width = new_width;
    height = new_height;
    
    grid.clear();
    grid.resize(height, std::vector<Cell>(width, Cell(Cell_type::EMPTY, Position(0, 0))));
    
    for (int y = 0; y < height && y < map_data.size(); ++y) {
        for (int x = 0; x < width && x < map_data[y].size(); ++x) {
            Cell_type type = (map_data[y][x] == '#') ? Cell_type::OBSTACLE : Cell_type::EMPTY;
            grid[y][x] = Cell(type, Position(x, y));
        }
    }
}

std::vector<std::string> Gamefield::get_map_data() const {
    std::vector<std::string> map_data;
    for (int y = 0; y < height; ++y) {
        std::string row;
        for (int x = 0; x < width; ++x) {
            row += (grid[y][x].get_type() == Cell_type::OBSTACLE) ? '#' : '.';
        }
        map_data.push_back(row);
    }
    return map_data;
}