#include "../include/Gamefield.h"
#include "../include/Cell.h"
#include <iostream>
#include <cstdlib>

Gamefield::Gamefield(int length, int width) {
    this->length = length;
    this->width = width;
    if (length < 10 || width < 10 || length > 25 || width > 25) {
        std::cout << "Incorrect gamefield size. Must be 10 - 25 in both dimensions.\n";
        exit(1);
    } else {
        gf.resize(width, std::vector<Cell>(length));

        for (int y = 0; y < width; ++y) {
            for (int x = 0; x < length; ++x) {
                gf[y][x].set_x(x);
                gf[y][x].set_y(y);

                if (rand() % 100 < 10) {
                    gf[y][x].set_passable(false);
                }
            }
        }
    }
}

Gamefield::Gamefield(int side)
    :Gamefield(side,side){};


Gamefield::Gamefield(const Gamefield &p) {
    length = p.length;
    width = p.width;
    gf = p.gf;
}

Gamefield::Gamefield(Gamefield&& p) noexcept {
    length = p.length;
    width = p.width;
    gf = std::move(p.gf);
    p.length = 0;
    p.width = 0;
}

Gamefield& Gamefield::operator=(const Gamefield& other) {
    if (this != &other) {
        length = other.length;
        width = other.width;
        gf = other.gf; 
    }
    return *this;
}

Gamefield& Gamefield::operator=(Gamefield&& other) noexcept {
    if (this != &other) {
        length = other.length;
        width = other.width;
        gf = std::move(other.gf);
        other.length = 0;
        other.width = 0;
    }
    return *this;
}

Cell* Gamefield::getCell(int x, int y) {
    if (x >= 0 && x < length && y >= 0 && y < width) {
        return &gf[y][x];
    }
    return nullptr;
}


int Gamefield::get_leng(){ return length; }
int Gamefield::get_width(){ return width; }

bool Gamefield::in_bounds(int x, int y) const{
    return x >= 0 && y >= 0 && x < width && y < length;
}

