#pragma once
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include <vector>
#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Field {
public:
    enum class Cell { Empty, Enemy, Player, Wall, Trap, Tower };
private:
    std::vector<std::vector<Cell>> cells;
    int size_x;
    int size_y;
public:
    Field(int size_x, int size_y, const std::vector<Enemy>& enemies, const std::vector<EnemyTower>& towers, const Player& player, int walls_count);
    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;
    Field() : size_x(0), size_y(0) {}

    int getWidth() { return size_x; }
    int getHeight() { return size_y; }

    bool inBounds(std::pair<int, int> coords) {
        return coords.first >= 0 && coords.first < size_x && coords.second >= 0 && coords.second < size_y;
    }

    Cell getCellType(std::pair<int, int> coords) {
        if (inBounds(coords)) return cells[coords.second][coords.first];
        else return Cell::Empty;
    }

    void setCellType(std::pair<int, int> coords, Cell c) {cells[coords.second][coords.first] = c;}

    bool isCellEmpty(std::pair<int, int> coords) {
        if (!inBounds(coords)) return false;
        else return getCellType(coords) == Cell::Empty;
    }
    bool isCellWalkable(std::pair<int, int> coords) {
        if (!inBounds(coords)) return false;
        else return getCellType(coords) == Cell::Empty || getCellType(coords) == Cell::Trap;
    }

    Cell moveEntity(Entity& entity, std::pair<int, int> target);

    json to_json();
    bool from_json(json& j);
};
