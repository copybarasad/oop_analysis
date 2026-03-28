#include "Field.h"
#include <random>
#include <algorithm>

Field::Field(int size_x, int size_y, const std::vector<Enemy>& enemies, const std::vector<EnemyTower>& towers, const Player& player, int walls_count) {
    this->size_x = std::max(10, std::min(size_x, 25));
    this->size_y = std::max(10, std::min(size_y, 25));
    cells = std::vector(size_y, std::vector(size_x, Cell::Empty));

    std::pair<int, int> cur_pos = player.getPosition();
    cells[cur_pos.second][cur_pos.first] = Cell::Player;
    for (auto& enemy : enemies) {
        cur_pos = enemy.getPosition();
        cells[cur_pos.second][cur_pos.first] = Cell::Enemy;
    }
    for (auto& tower : towers) {
        cur_pos = tower.getPosition();
        cells[cur_pos.second][cur_pos.first] = Cell::Tower;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist_x(0, size_x - 1);
    std::uniform_int_distribution<> dist_y(0, size_y - 1);
    int max_walls = size_x * size_y - enemies.size() - 5;
    walls_count = std::min(max_walls, walls_count);
    while (walls_count > 0) {
        std::pair<int, int> coords = std::make_pair(dist_x(gen), dist_y(gen));
        if (isCellEmpty(coords)) {
            cells[coords.second][coords.first] = Cell::Wall;
            walls_count--;
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

Field::Cell Field::moveEntity(Entity& entity, std::pair<int, int> target) {
    if (!inBounds(target)) return Cell::Wall;
    
    Cell target_cell_type = getCellType(target);
    if (isCellWalkable(target)) {
        setCellType(target, getCellType(entity.getPosition()));
        setCellType(entity.getPosition(), Cell::Empty);
        entity.setPosition(target);
        return target_cell_type;
    }
    return target_cell_type;
}

json Field::to_json() {
    json j;
    j["width"] = size_x;
    j["height"] = size_y;

    std::vector<std::string> rows;
    for(int y=0; y<size_y; y++) {
        std::string row = "";
        for(int x=0; x<size_x; x++) {
            row += std::to_string(static_cast<int>(cells[y][x]));
        }
        rows.push_back(row);
    }
    j["cells"] = rows;
    return j;
}

bool Field::from_json(json& j) {
    try {
        size_x = j.at("width");
        size_y = j.at("height");
        cells.assign(size_y, std::vector<Cell>(size_x));

        std::vector<std::string> rows = j.at("cells");
        for(int y=0; y<size_y; y++) {
            for(int x=0; x<size_x; x++) {
                int val = rows[y][x] - '0';
                cells[y][x] = static_cast<Cell>(val);
            }
        }
        return true;
    } catch(...) { return false; }
}
