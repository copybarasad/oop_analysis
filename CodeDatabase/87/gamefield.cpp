#include "gamefield.hpp"
#include <random>
#include <chrono>
#include <queue>

Gamefield::Gamefield(int field_height, int field_width): height(field_height), width(field_width){
    initialize_grid();
}

Gamefield::Gamefield(const Gamefield& other): grid(other.grid), height(other.height), width(other.width){}

Gamefield::Gamefield(Gamefield&& other): grid(std::move(other.grid)), height(other.height), width(other.width){
    other.height = 0;
    other.width = 0;
}

Gamefield& Gamefield::operator=(const Gamefield& other){
    if(this != &other){
        height = other.height;
        width = other.width;
        grid = other.grid;
    }
    return *this;
}

Gamefield& Gamefield::operator=(Gamefield&& other){
    if(this != &other){
        height = other.height;
        width = other.width;
        grid = std::move(other.grid);
        other.height = 0;
        other.width = 0;
    }
    return *this;
}

void Gamefield::initialize_grid(){
    for(int att = 0; att < 5; att++){
        grid.resize(height, std::vector<Cell>(width, Cell(Cell_type::EMPTY, Position(0, 0))));
        std::mt19937 gen(std::chrono::steady_clock::now().time_since_epoch().count());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                Position current_position(x, y);
                if(dis(gen) < 0.1 && !((x == 0 && y == 0) || (x == width - 1 && y == height - 1) || (x == width - 1 && y == 0) || (x == 0 && y == height - 1))){
                    grid[y][x] = Cell(Cell_type::BARRICADE, current_position);
                }
                else{
                    grid[y][x] = Cell(Cell_type::EMPTY, current_position);
                }
            }
        }
        if(is_connected_from_start()) return;
    }
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            grid[y][x] = Cell(Cell_type::EMPTY, Position(x, y));
        }
    }
}

bool Gamefield::is_valid_position(const Position& position) const{
    return position.get_x() >= 0 && position.get_x() < width && position.get_y() >= 0 && position.get_y() < height;
}

int Gamefield::get_height() const{
    return height;
}

int Gamefield::get_width() const{
    return width;
}

bool Gamefield::is_position_accessible(const Position& position) const{
    if(!is_valid_position(position)){
        return false;
    }
    return grid[position.get_y()][position.get_x()].is_accessible();
}

Cell_type Gamefield::get_cell_type(const Position& position) const{
    return grid[position.get_y()][position.get_x()].get_type();
}

std::vector<Position> Gamefield::get_adjacent_positions(const Position& position) const{
    std::vector<Position> adjacent_positions;
    std::vector<Position> directions = {Position(0, 1), Position(1, 0), Position(0, -1), Position(-1, 0)};
    for(auto& direction: directions){
        Position new_position = position + direction;
        if(is_valid_position(new_position) && is_position_accessible(new_position)){
            adjacent_positions.push_back(new_position);
        }
    }
    return adjacent_positions;
}

bool Gamefield::is_connected_from_start() const{
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::queue<Position> q;
    q.push(Position(0, 0));
    visited[0][0] = true;
    int reachable_count = 0;
    std::vector<Position> directions = {Position(0,1), Position(1,0), Position(0,-1), Position(-1,0)};
    while(!q.empty()){
        Position cur = q.front();
        q.pop();
        reachable_count++;
        for (auto& dir: directions){
            Position next = cur + dir;
            if(is_valid_position(next) && !visited[next.get_y()][next.get_x()] && is_position_accessible(next)){
                visited[next.get_y()][next.get_x()] = true;
                q.push(next);
            }
        }
    }
    int total_accessible = 0;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            if(is_position_accessible(Position(x, y))){
                total_accessible++;
            }
        }
    }
    return reachable_count == total_accessible;
}

bool Gamefield::is_line_clear(const Position& from, const Position& to) const{
    int dx = to.get_x() - from.get_x();
    int dy = to.get_y() - from.get_y();
    if(dx != 0 && dy != 0 && std::abs(dx) != std::abs(dy)) return false;
    int steps = std::max(std::abs(dx), std::abs(dy));
    int step_x = (dx > 0) - (dx < 0);
    int step_y = (dy > 0) - (dy < 0);
    Position current = from;
    for(int i = 0; i < steps; i++){
        current = Position(current.get_x() + step_x, current.get_y() + step_y);
        if(i == steps - 1) break;
        if(!is_position_accessible(current)) return false;
    }
    return true;
}

Gamefield::Gamefield(int field_height, int field_width, const std::vector<std::vector<Cell_type>>& grid_data): height(field_height), width(field_width){
    grid.resize(height, std::vector<Cell>(width));
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            Position pos(x, y);
            grid[y][x] = Cell(grid_data[y][x], pos);
        }
    }
}