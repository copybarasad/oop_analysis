#ifndef FIELD_HPP
#define FIELD_HPP

#include "cell.hpp"
#include "point.hpp"
#include <vector>
#include <nlohmann/json_fwd.hpp>

class Field {
    int width;
    int height;
    std::vector<Cell> field;
    const int slow_percent {5};
    const int impassible_percent {50};

    void build_field(); 

public:
    Field(int width, int height);
    Field(const Field &other);
    Field(Field &&other);
    Field& operator=(const Field &other);
    Field& operator=(Field &&other);
    int get_width();
    int get_height();
    Cell &operator[](Point point);
    Cell &get_cell(Point point);
    void regenerate();
    void load_from_data(const std::vector<int>& cell_types, const std::vector<int>& trap_data);
    nlohmann::json save() const;
};

#endif
