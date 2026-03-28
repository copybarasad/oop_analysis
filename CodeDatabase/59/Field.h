#pragma once

#include "Cell.h"
#include "Coordinates.h"
#include "Being.h"
#include <vector>
#include "json.hpp"
using json = nlohmann::json;

class Field {
private:
    std::vector<Cell> cells;
    int size_x;
    int size_y;

public:
    Field(int size_x, int size_y, float wall_density = 0.1f);

    Field(const Field& other);
    Field& operator=(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;

    int getWidth() const;
    int getHeight() const;
    bool inBounds(const Coordinates& coords) const;
    int get_idx(const Coordinates& coordinates) const;

    Cell::Type getCellType(const Coordinates& coords) const;
    void setCellType(const Coordinates& coords, Cell::Type t);

    bool isCellFree(const Coordinates& coords) const;

    Cell::Type moveBeing(Being& being, const Coordinates& offset);

    std::string to_string() const;
    void resize(int new_w, int new_h);

    json toJson() const;
    void fromJson(const json& j);
};