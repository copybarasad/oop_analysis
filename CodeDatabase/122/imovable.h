#ifndef IMOVABLE_H
#define IMOVABLE_H

#include "cell.h"

class IMovable
{
public:
    virtual void move(const Cell& cell, int dx=0, int dy=0) = 0;
};

#endif // IMOVABLE_H
