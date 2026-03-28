#ifndef GAMEFIELD_HPP
#define GAMEFIELD_HPP
#include "cell.hpp"
#include <vector>

class Gamefield{
    private:
        std::vector<std::vector<Cell>> grid;
        int height;
        int width;
        void initialize_grid();
        bool is_valid_position(const Position& position) const;
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
        bool is_connected_from_start() const;
        bool is_line_clear(const Position& from, const Position& to) const;

        Gamefield(int height, int width, const std::vector<std::vector<Cell_type>>& grid_data);
};


#endif