#ifndef PLACE_H
#define PLACE_H
#include <vector>
#include "player.h"
#include "cell.h"

class Place{
private:
    int rows;
    int cols;
    std::vector<std::vector<Cell>> field;

public:
    Place(int rows_ = 10, int cols_ = 10);

    int get_rows() const;

    int get_cols() const;

    Cell::Type get_cell(int x, int y);

    void change_cell(Cell::Type type, int x, int y);

    int check_cell(int x, int y);

    void create_splecial_cell(Cell::Type type);

    void render();

    Place(const Place &other);

    Place(Place&& other) noexcept;
    Place& operator=(const Place& other);

    Place& operator=(Place&& other) noexcept;
    ~Place();
};

#endif