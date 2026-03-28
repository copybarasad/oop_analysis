#pragma once
#include <vector>
#include "../Cell.h"

struct dataField{
    int width;
    int height;
    std::vector<std::vector<Cell>> grid;
};
