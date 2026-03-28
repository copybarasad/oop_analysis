#include "Field.h"
#include "Cell.h"
#include "Logger.h"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>

Field::Field(int w, int h, int numOfUnwalkableCells)
    : width(w), height(h) {
    srand(static_cast<unsigned>(time(nullptr)));

    cells.reserve(height);
    for (int y = 0; y < height; ++y) {
        std::vector<Cell> row;
        row.reserve(width);
        for (int x = 0; x < width; ++x) {
            row.emplace_back(x, y, true);
        }
        cells.push_back(std::move(row));
    }

    for (int i = 0; i < numOfUnwalkableCells; i++) {
        int x = rand() % width;
        int y = rand() % height;

        if (!cells[y][x].isWalkable()) {
            i--;
            continue;
        }
        cells[y][x].setWalkable(false);
    }
}

// --- Копирующий конструктор ---
Field::Field(const Field& other)
    : width(other.width),
    height(other.height),
    cells(other.cells),
    traps(other.traps) {
}

// --- Перемещающий конструктор ---
Field::Field(Field&& other) noexcept
    : width(other.width),
    height(other.height),
    cells(std::move(other.cells)),
    traps(std::move(other.traps)) {
    other.width = 0;
    other.height = 0;
}

// --- Копирующее присваивание ---
Field& Field::operator=(const Field& other) {
    if (this == &other)
        return *this;

    width = other.width;
    height = other.height;
    cells = other.cells;
    traps = other.traps;
    return *this;
}

// --- Перемещающее присваивание ---
Field& Field::operator=(Field&& other) noexcept {
    if (this == &other)
        return *this;

    width = other.width;
    height = other.height;
    cells = std::move(other.cells);
    traps = std::move(other.traps);

    other.width = 0;
    other.height = 0;
    return *this;
}

bool Field::isCellWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height || hasTowerAt(x, y))
        return false;
    return cells[y][x].isWalkable();
}

// --- Traps ---
void Field::spawnTrap(const Coords& pos, int damage) {
    traps.push_back({ pos, damage });
}

void Field::removeTrapAt(const Coords& pos) {
    traps.erase(std::remove_if(traps.begin(), traps.end(),
        [&](const Trap& t) { return t.pos.x == pos.x && t.pos.y == pos.y; }),
        traps.end());
}

int Field::checkTrapDamage(const Coords& pos) const {
    for (const auto& trap : traps) {
        if (trap.pos.x == pos.x && trap.pos.y == pos.y) {
            return trap.damage;
        }
    }
    return 0;
}

bool Field::hasTrapAt(int x, int y) const {
    for (const auto& trap : traps) {
        if (trap.pos.x == x && trap.pos.y == y)
            return true;
    }
    return false;
}

// --- Towers ---
void Field::addTower(const Tower& tower) {
    towers.push_back(tower);
}

std::vector<Tower>& Field::getTowers() {
    return towers;
}

bool Field::hasTowerAt(int x, int y) const {
    for (const auto& t : towers)
        if (t.getCoords().x == x && t.getCoords().y == y)
            return true;
    return false;
}

void Field::log() const {
    Logger::log() << "Field: " << width << "x" << height
        << " | traps amount: " << traps.size()
        << " Towers:" << "\n";
    for (const auto& t : towers)
        Logger::log() << "    Tower: {" << t.getCoords().x << ',' << t.getCoords().y << "}, damage: " << t.getDamage() << ". Range: " << t.getRange() << "\n";
}

json Field::serialize() const {
    json j;
    j["width"] = width;
    j["height"] = height;

    // --- Клетки ---
    j["cells"] = json::array();
    for (const auto& row : cells) {
        json rowJson = json::array();
        for (const auto& cell : row) {
            rowJson.push_back(cell.serialize());
        }
        j["cells"].push_back(rowJson);
    }

    // --- Ловушки ---
    j["traps"] = json::array();
    for (const auto& trap : traps) {
        j["traps"].push_back(trap.serialize());
    }

    // --- Башни ---
    j["towers"] = json::array();
    for (const auto& tower : towers) {
        j["towers"].push_back(tower.serialize());
    }

    return j;
}

void Field::deserialize(const json& j) {
    width = j.at("width").get<int>();
    height = j.at("height").get<int>();

    cells.clear();
    cells.reserve(height);
    for (const auto& rowJson : j.at("cells")) {
        std::vector<Cell> row;
        row.reserve(width);
        for (const auto& cellJson : rowJson) {
            Cell cell(0, 0, true);
            cell.deserialize(cellJson);
            row.push_back(cell);
        }
        cells.push_back(std::move(row));
    }

    traps.clear();
    for (const auto& trapJson : j.at("traps")) {
        Trap t;
        t.deserialize(trapJson);
        traps.push_back(t);
    }

    towers.clear();
    for (const auto& towerJson : j.at("towers")) {
        Tower t({ 0, 0 }, 0, 0, 0);
        t.deserialize(towerJson);
        towers.push_back(t);
    }
}
