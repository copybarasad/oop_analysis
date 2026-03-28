#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <iostream>
#include "Cell.h"
#include "Point.h"

class Field {
public:
    Field(int width, int height);

    void populateField(int impassable_count, int slowing_count, const Point& player_pos);
    void resize(int width, int height);
    
    int getWidth() const;
    int getHeight() const;
    CellType getCellType(int x, int y) const;
    bool isValid(int x, int y) const;

    void save(std::ostream& os) const;
    void load(std::istream& is);
    
private:
    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
};

#endif