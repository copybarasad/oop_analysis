#pragma once

#include <vector>
#include <set>
#include <cmath>
#include "position.hpp"
#include "game_object.hpp"

class Field {
private:
    std::vector<std::vector<GameObject*>> field;
    int h, w;
public:
    Field() = default;
    Field(int rows, int cols);
    Field(const Field& to_copy);
    Field(Field&& to_move);
    // ~Field();

    Field &operator=(const Field &other);
    Field &operator=(Field &&other);
    GameObject* operator [](const Position &pos) const;
    GameObject*& operator [](const Position &pos);

    int height() const;
    int width() const;
    bool in_bounds(const Position &pos) const;

    std::vector<Position> free_cells(const Position &top_left, const Position &bottom_right) const;
    std::vector<Position> free_cells() const;

    std::vector<Position> filter(const std::set<char> &to_include) const;
    std::vector<Position> filter(const Position &top_left, const Position &bottom_right, const std::set<char> &to_include) const;
};