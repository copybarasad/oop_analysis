#ifndef GAME_CELL_H
#define GAME_CELL_H

#include <vector>

class Object;

class Cell {
private:
    std::vector<Object *> layers_;

public:
    explicit Cell(const size_t amount_of_layers) : layers_(amount_of_layers) {
    }

    friend class Map;
};

#endif
