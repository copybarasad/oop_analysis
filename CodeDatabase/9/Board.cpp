#include "Board.h"

Board::Board(int w, int h){
    if (w < 10 || w > 25 || h < 10 || h > 25){
        throw std::invalid_argument("Уменьшите размер поля/n");
    }
    allocate(w, h);
}


Board::Board(const Board& other){
    allocate(other.width_, other.height_);
    copyFrom(other);
}

Board::Board(Board&& other){
    width_ = other.width_;
    height_ = other.height_;
    cells_ = std::move(other.cells_);
    other.width_  = 0;
    other.height_ = 0;
}


Board& Board::operator=(const Board& other){
    if(this == &other){
        return *this;
    }
    allocate(other.width_, other.height_);
    copyFrom(other);
    return *this;
}


Board& Board::operator=(Board&& other) {
 if (this == &other) {
  return *this;
 }
 width_ = other.width_;
 height_ = other.height_;
 cells_ = std::move(other.cells_);
 other.width_ = 0;
 other.height_ = 0;
 return *this;
}


int Board::width() const{
    return width_;
}

int Board::height() const{
    return height_;
}

Cell& Board::at(std::pair<int, int> pos){
    return cells_[pos.second * width_ + pos.first];
}

int Board::index(int x, int y){
    return y * width_+ x;
}

void Board::allocate(int w, int h){
    width_ = w;
    height_ = h;
    cells_ = std::unique_ptr<Cell[]>(new Cell[w*h]);
}


void Board::copyFrom(const Board& other) {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            std::pair<int, int> pos = {x, y};
            at(pos) = const_cast<Board&>(other).at(pos);
            }
        }
}