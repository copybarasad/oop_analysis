#ifndef FIELD_H
#define FIELD_H
#include <iostream>
#include <utility>
#include <vector>
#include "constants.h"
#include "Cell.h"

class Creature;

class Field{
    int width = MIN(MAX(WIDTH, 10), 25);
    int height = MIN(MAX(HEIGHT, 10), 25);
    std::vector<std::vector<Cell>> info_field;
    public:
        Field();
        Field(int w, int h);
        int getWidth() const;
        int getHeight() const;
        ~Field() = default;
        Field(const Field& other) = default; 
        Field& operator=(const Field& other) = default;
        Field(Field&& other) noexcept = default;
        Field& operator=(Field&& other) noexcept = default;
        void resize(int w, int h);
        Cell* get_cell(int x, int y);
        bool place_object_field(int x, int y, ObjectType obj);
        bool place_object_field(int x, int y, ObjectType obj, Creature *creature);
        void remove_object_field(int x, int y);
        void print_field() const;
        void save(std::ofstream& ofs) const;
        void load(std::ifstream& ifs);
        friend class Game;
};

#endif