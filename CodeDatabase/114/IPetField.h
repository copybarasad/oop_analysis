#ifndef IPETFIELD_H
#define IPETFIELD_H

#include <vector>
#include "Pet.h"
#include "Cell.h"

class IPetField {
public:
    virtual ~IPetField() = default;

    virtual std::vector<Pet>& getPets() = 0;
    virtual bool isValidPosition(int x, int y) const = 0;
    virtual bool isPositionPassable(int x, int y) const = 0;
    virtual std::vector<std::vector<Cell>>& getCells() = 0;
};

#endif

