#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <string>
#include "cell.h"

class Gamefield {
private:
    std::vector<std::vector<Cell>> grid;
    int height;
    int width;

    void initialize_grid();

public:
    Gamefield(int field_width, int field_height);
    Gamefield(const Gamefield& other);
    Gamefield(Gamefield&& other);
    Gamefield& operator=(const Gamefield& other);
    Gamefield& operator=(Gamefield&& other);

    int get_height() const;
    int get_width() const;
    bool is_position_accessible(const Position& position) const;
    Cell_type get_cell_type(const Position& position) const;
    std::vector<Position> get_adjacent_positions(const Position& position) const;
    bool is_valid_position(const Position& position) const;
    
    void create_from_data(int new_width, int new_height, const std::vector<std::string>& map_data);
    std::vector<std::string> get_map_data() const;
};

#endif