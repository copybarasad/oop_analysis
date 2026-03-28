#include "field.hpp"
#include "entities/entity.hpp"
#include <algorithm>
#include <stdexcept>
#include <nlohmann/json.hpp>

Field::Field(int width, int height) {
    if (width < 10 || width > 25 || height < 10 || height > 25) {
        throw std::invalid_argument("incorrect field size");
    }

    this->width = width;
    this->height = height;
    build_field();
}

Field::Field(const Field &other) {
    width = other.width;
    height = other.height;
    for (Cell x : other.field) {
        field.push_back(x);
    }
}

Field::Field(Field &&other) {
    if (&other != this) {
        width = other.width;
        height = other.height;
        field.swap(other.field);
        other.field.clear();
    }
}

Field& Field::operator=(const Field &other) {
    width = other.width;
    height = other.height;
    field.clear();
    for (Cell x : other.field) {
        field.push_back(x);
    }
    return *this;
}

Field& Field::operator=(Field &&other) {
    if (&other != this) {
        width = other.width;
        height = other.height;
        field.swap(other.field);
        other.field.clear();
    }
    return *this;
}

void Field::build_field() {
    field.resize(width * height, Cell(CellType::Impassible));
    
    std::vector<Point> candidates;

    Point start = {rand() % width, rand() % height};
    candidates.push_back(start);
    
    int required_count = width * height * (100 - impassible_percent) / 100;
    int passible_count = 0;
    int step = 0;

    while (!candidates.empty() && passible_count != required_count) { 
        int i = rand() % (candidates.size() - step) + step;
        
        Point point = candidates[i];
        candidates.erase(candidates.begin() + i);
        
        field[point.y * width + point.x].set_type(CellType(rand() % 100 >= slow_percent ? 0 : 1));
        passible_count++;

        for (Point child : point.get_neighbors(width, height)) {
            if (std::find(candidates.begin(), candidates.end(), child) == candidates.end() &&
                field[child.y * width + child.x].get_type() == CellType::Impassible) {
                candidates.push_back(child);
            }
        }
        
        step = (step + 1) % candidates.size();
    }
}

Cell &Field::get_cell(Point point) {
    if (!point.inside_field(width, height)) {
        throw std::invalid_argument("incorrect cell coordinates");
    }
    return this->field[point.y * this->width + point.x];
}

int Field::get_width() {
    return width;
}
int Field::get_height() {
    return height;
}

Cell & Field::operator[](Point point) {
    return get_cell(point);
}

void Field::regenerate() {
    field.clear();
    build_field();
}

void Field::load_from_data(const std::vector<int>& cell_types, const std::vector<int>& trap_data) {
    if (cell_types.size() != width * height) return;
    
    field.clear();
    field.resize(width * height);
    
    for (int i = 0; i < cell_types.size(); i++) {
        field[i].set_type(static_cast<CellType>(cell_types[i]));
        if (i < trap_data.size() && trap_data[i] > 0) {
            field[i].set_trap_damage(trap_data[i]);
        }
    }
}

nlohmann::json Field::save() const {
    nlohmann::json j;
    j["width"] = width;
    j["height"] = height;
    
    std::vector<int> cell_types;
    std::vector<int> trap_data;
    
    for (const Cell& cell : field) {
        cell_types.push_back(static_cast<int>(cell.get_type()));
        trap_data.push_back(cell.get_trap_damage());
    }
    
    j["cell_types"] = cell_types;
    j["trap_data"] = trap_data;
    
    return j;
}
