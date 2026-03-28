#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include <vector>
#include <random>

class Field {
private:
    std::vector<std::vector<Cell> > grid;
    int width;
    int height;
    mutable std::mt19937 randomGenerator;

public:
    Field(int fieldWidth, int fieldHeight);

    Field(const Field &other);

    Field(Field &&other) noexcept;

    Field &operator=(const Field &other);

    Field &operator=(Field &&other) noexcept;

    ~Field() = default;

    int getWidth() const;

    int getHeight() const;

    const Cell &getCell(int x, int y) const;

    void setCell(int x, int y, CellType type);

    bool isValidPosition(int x, int y) const;

    bool isPassable(int x, int y) const;

private:
    void initializeField();

    void copyFrom(const Field &other);

    int randomInt(int min, int max) const;
};

#endif
