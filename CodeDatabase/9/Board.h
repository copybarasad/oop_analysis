#ifndef BOARD_H
#define BOARD_H

#include <cstddef>
#include <memory>
#include <stdexcept>
#include "Cell.h"


class Board{
public:
    Board(int w, int h);
    Board(const Board& other);
    Board(Board&& other);

    Board& operator=(const Board& other);
    Board& operator=(Board&& other);
    ~Board() = default;

    int width() const;
    int height() const;

    Cell& at(std::pair<int, int> pos);

private:
    int width_;
    int height_;
    std::unique_ptr<Cell[]>cells_;
    int index(int x, int y);
    void allocate(int w, int h);
    void copyFrom(const Board& other);

};


#endif 