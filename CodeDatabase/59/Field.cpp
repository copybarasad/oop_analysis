#include "Field.h"
#include <random>
#include <algorithm>

Field::Field(int size_x, int size_y, float wall_density) {
    this->size_x = std::max(10, std::min(size_x, 25));
    this->size_y = std::max(10, std::min(size_y, 25));
    cells.reserve(this->size_x * this->size_y);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);
    for (int y = 0; y < this->size_y; ++y) {
        for (int x = 0; x < this->size_x; ++x) {
            if (dist(gen) < wall_density) {
                cells.emplace_back(Cell::Type::Wall);
            } else {
                cells.emplace_back(Cell::Type::Empty);
            }
        }
    }
}

Field::Field(const Field& other) {
    size_x = other.size_x;
    size_y = other.size_y;
    cells = other.cells;
}
Field& Field::operator=(const Field& other) {
    if (this == &other) return *this;
    size_x = other.size_x;
    size_y = other.size_y;
    cells = other.cells;
    return *this;
}
Field::Field(Field&& other) noexcept {
    size_x = other.size_x;
    size_y = other.size_y;
    cells = std::move(other.cells);
    other.size_x = other.size_y = 0;
}
Field& Field::operator=(Field&& other) noexcept {
    if (this == &other) return *this;
    size_x = other.size_x;
    size_y = other.size_y;
    cells = std::move(other.cells);
    other.size_x = other.size_y = 0;
    return *this;
}

int Field::getWidth() const { return size_x; }
int Field::getHeight() const { return size_y;
}

bool Field::inBounds(const Coordinates& coords) const {
    return coords.x >= 0 && coords.x < size_x && coords.y >= 0 && coords.y < size_y;
}

int Field::get_idx(const Coordinates& coordinates) const {
    return coordinates.y * size_x + coordinates.x;
}

Cell::Type Field::getCellType(const Coordinates& coords) const {
    if (inBounds(coords)) return cells[get_idx(coords)].getType();
    else return Cell::Type::Empty;
}

void Field::setCellType(const Coordinates& coords, Cell::Type t) {
    cells[get_idx(coords)].setType(t);
}

bool Field::isCellFree(const Coordinates& coords) const {
    if (!inBounds(coords)) return false;
    return getCellType(coords) == Cell::Type::Empty ||
    getCellType(coords) == Cell::Type::Trap;
}

Cell::Type Field::moveBeing(Being& being, const Coordinates& offset) {
    Coordinates target = being.getPosition() + offset;
    Cell::Type type = getCellType(target);
    if (!inBounds(target)) {
        return Cell::Type::Wall;
    }
    if (isCellFree(target)) {
        setCellType(target, getCellType(being.getPosition()));
        setCellType(being.getPosition(), Cell::Type::Empty);
        being.move(target);
    }
    return type;
}

std::string Field::to_string() const {
    std::string s;
    for (int y = 0; y < size_y; ++y) {
        for (int x = 0; x < size_x; ++x) {
            s += cells[y*size_x + x].to_string() + " ";
        }
        s += '\n';
    }
    return s;
}

void Field::resize(int new_w, int new_h) {
    size_x = std::max(10, std::min(new_w, 25));
    size_y = std::max(10, std::min(new_h, 25));
    cells.resize(size_x * size_y);
    std::fill(cells.begin(), cells.end(), Cell(Cell::Type::Empty));
}

json Field::toJson() const {
    json j;
    j["size_x"] = size_x;
    j["size_y"] = size_y;

    std::vector<int> cell_types;
    cell_types.reserve(cells.size());
    for(const auto& cell : cells) {
        cell_types.push_back(static_cast<int>(cell.getType()));
    }
    j["cells"] = cell_types;
    return j;
}

void Field::fromJson(const json& j) {
    j.at("size_x").get_to(size_x);
    j.at("size_y").get_to(size_y);
    auto cell_types = j.at("cells").get<std::vector<int>>();
    cells.clear();
    cells.reserve(cell_types.size());

    for(int type_int : cell_types) {
        cells.emplace_back(static_cast<Cell::Type>(type_int));
    }
}