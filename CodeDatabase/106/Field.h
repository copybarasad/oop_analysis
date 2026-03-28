#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include <cstdlib>
#include <ctime>
#include <stdexcept>
#include <sstream>
#include <string>
#include "Cell.h"

class Field {
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> field; // двумерный массив клеток, образующий поле

public:
    Field(int w, int h);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;
    ~Field() = default;

    int getWidth() const { return width; };
    int getHeight() const { return height; };
    bool isValidPos(int x, int y) const;
    void randomGenerate();
    void clear();
    std::string toString() const;
    Cell& getCell(int x, int y) { return field[y][x]; }
};


#endif //FIELD_H