#include "Gamefield.h"
#include <iostream>
#include <cstdlib>

Gamefield::Gamefield(int length, int width) : length(length), width(width) {
    if (length < 10 || width < 10 || length > 25 || width > 25) {
        std::cout << "Invalid field size. Creating field 10x10.\n";
        length = 10;
        width = 10;
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            Cell new_cell(j, i);
            if (rand() % 100 < 10) new_cell.set_passable(false);
            // if (rand() % 100 < 10) new_cell.set_slow(true);
            cells.push_back(new_cell);
        }
    }
}

Gamefield::Gamefield(const Gamefield &other) {
    length = other.length;
    width = other.width;
    cells = other.cells;
}

Gamefield::Gamefield(Gamefield&& other) noexcept {
    length = other.length;
    width = other.width;
    cells = std::move(other.cells);
    other.length = 0;
    other.width = 0;
}

Gamefield& Gamefield::operator=(const Gamefield& other) {
    if (this != &other) {
        length = other.length;
        width = other.width;
        cells = other.cells;
    }
    return *this;
}

Gamefield& Gamefield::operator=(Gamefield&& other) noexcept {
    if (this != &other) {
        length = other.length;
        width = other.width;
        cells = std::move(other.cells);
        other.length = 0;
        other.width = 0;
    }
    return *this;
}

int Gamefield::get_length() const { return length; }

int Gamefield::get_width() const { return width; }

Cell* Gamefield::get_cell(int x, int y) {
    if (x >= 0 && x < length && y >= 0 && y < width) {
        return &cells[y * length + x];
    }
    return nullptr;
}

void Gamefield::print() const {
    for (int y = 0; y < width; ++y) {
        for (int x = 0; x < length; ++x)
            std::cout << cells[y * length + x].get_symbol() << ' ';
        std::cout << "\n";
    }
}

Cell* Gamefield::get_random_cell() {
    int x, y;
    Cell* cell = nullptr;
    int count = 0;
    do {
        x = rand() % length;
        y = rand() % width;
        cell = get_cell(x, y);
        count++;
        if (count > 1000) return nullptr;
    } while (!cell || !cell->is_passable() || cell->is_occupied());
    return cell;
}