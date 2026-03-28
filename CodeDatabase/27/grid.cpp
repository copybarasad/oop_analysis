#include "grid.h"
#include <cstdlib>
#include "unit.h" 
#include "enemy_tower.h"

Grid::Grid(int length, int width) {
    if (length < 10 || length > 25 || width < 10 || width > 25) {
        if (length < 10) length = 10; else if (length > 25) length = 25;
        if (width  < 10) width  = 10; else if (width  > 25) width  = 25;
    }

    this->length = length;
    this->width = width;
    for (int i = 0; i < this->width; i++) {
        for (int j = 0; j < this->length; j++) {
            Point new_point;
            new_point.set_x(j);
            new_point.set_y(i);
            new_point.set_parent_grid(this);
            if (rand() % 100 < 10) {
                new_point.set_passable(false);
            }
            grd.push_back(new_point);
        }
    }
}

//Copy Constructor
Grid::Grid(const Grid &p) {
    length = p.length;
    width = p.width;
    grd = p.grd;

    for (Point& point : grd) {
        point.set_parent_grid(this);
        if (Unit* unit = point.get_occupier()) {
            unit->set_parent_grid(this);
        }
    }
}

//Move Constructor
Grid::Grid(Grid&& p) noexcept {
    length = p.length;
    width = p.width;
    grd = std::move(p.grd);

    for (Point& point : grd) {
        point.set_parent_grid(this);
        if (Unit* unit = point.get_occupier()) {
            unit->set_parent_grid(this);
        }
    }

    p.length = 0;
    p.width = 0;
} 

//Copy Assignment
Grid& Grid::operator=(const Grid& other) {
    if (this != &other) {
        length = other.length;
        width = other.width;
        grd = other.grd; 

        // Fix point and unit links
        for (Point& point : grd) {
            point.set_parent_grid(this);
            if (Unit* unit = point.get_occupier()) {
                unit->set_parent_grid(this);
            }
        }
    }
    return *this;
}

//Move Assignment
Grid& Grid::operator=(Grid&& other) noexcept {
    if (this != &other) {
        length = other.length;
        width = other.width;
        grd = std::move(other.grd);

        // Fix point and unit links
        for (Point& point : grd) {
            point.set_parent_grid(this);
            if (Unit* unit = point.get_occupier()) {
                unit->set_parent_grid(this);
            }
        }
        
        other.length = 0;
        other.width = 0;
    }
    return *this;
}

Point* Grid::getPoint(int x, int y) {
    if (x >= 0 && x < length && y >= 0 && y < width) {
        return &grd[y * length + x];
    }
    return nullptr;
}

const Point* Grid::getPoint(int x, int y) const {
    return const_cast<Grid*>(this)->getPoint(x, y);
}

void Grid::print_grid() const {
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < length; ++j) {
            const Point& p = grd[i * length + j];
            if (p.get_occupier() != nullptr) {
                if (p.is_occupied_by_player()) {
                } else if (p.is_occupied_by_enemy()) {
                } else {
                    if (dynamic_cast<const EnemyTower*>(p.get_occupier())) {
                    } else {
                    }
                }
            } else if (!p.is_passable()) {
            } else {
                if (p.has_trap()) {
                } else {
                }
            }
        }
    }
}

int Grid::get_leng() const { return length; }
int Grid::get_width() const { return width; }
