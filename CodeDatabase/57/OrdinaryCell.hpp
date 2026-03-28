#pragma once
#include "Cell.hpp"

class OrdinaryCell : public Cell {
public:
    OrdinaryCell();
    std::string getRandomGrassTexture();
}; 