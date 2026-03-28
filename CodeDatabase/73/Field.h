#ifndef GAMELB1_FIELD_H
#define GAMELB1_FIELD_H

#include <vector>
#include <iostream>
#include "Cell.h"

class Field {
public:
    Field(int width, int height);
    Field() : width_(0), height_(0) {}
    Field(const Field& other) = default;
    Field& operator=(const Field& other) = default;

    int getWidth() const;
    int getHeight() const;

    CellType getCellType(int x, int y) const;
    void setCellType(int x, int y, CellType type);
    bool isValidPosition(int x, int y) const;

    void save(std::ostream& os) const;
    void load(std::istream& is);

private:
    void initialize();

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};

#endif //GAMELB1_FIELD_H