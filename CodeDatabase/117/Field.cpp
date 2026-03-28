#include "Field.h"
#include "../Characters/Character.h"

Field::Field(int width, int height){
    this->width = std::clamp(width, MIN_FIELD_SIZE, MAX_FIELD_SIZE);
    this->height = std::clamp(height, MIN_FIELD_SIZE, MAX_FIELD_SIZE);

    this->cells = new Cell* [height];
    for(int i = 0; i < height; ++i){
        cells[i] = new Cell [width];
    }
}

Field::Field():Field(DEFAULT_FIELD_SIZE, DEFAULT_FIELD_SIZE){}

Field::Field(const Field& other): Field(other.width, other.height){
    for(int i = 0; i < this->height; ++i){
        for(int j = 0; j < this->width; ++j){
            this->cells[i][j] = other.cells[i][j];
        }
    }
}

Field& Field::operator = (const Field& other){
    if(this != &other){
        Field temp(other);
        std::swap(this->width, temp.width);
        std::swap(this->height, temp.height);
        std::swap(this->cells, temp.cells);
    }
    return *this;
}

Field::Field(Field&& other): height(0), width(0), cells(nullptr){
    std::swap(this->height, other.height);
    std::swap(this->width, other.width);
    std::swap(this->cells, other.cells);
}

Field& Field::operator= (Field&& other){
    if (this != &other){
        std::swap(this->height, other.height);
        std::swap(this->width, other.width);
        std::swap(this->cells, other.cells);
    }
    return *this;
}

Field::~Field(){
    for(int i = 0; i < this->height; ++i){
        delete[] this->cells[i];
    }
    delete[] this->cells;
}

int Field::getHeight() const{
    return this->height;
}

int Field::getWidth() const{
    return this->width;
}

bool Field::isValidPos(int x, int y) const{
    return (x <= this->width && y <= this->height && x > 0 && y > 0);
}

bool Field::isValidPos(std::pair<int, int> position) const{
    return this->isValidPos(position.first, position.second);
}

bool Field::canMoveTo(int x, int y) const{
    if (!this->isValidPos(x, y)){
        return false;
    }
    return this->cells[y - 1][x - 1].isFree();
}

bool Field::canMoveTo(std::pair<int, int> position) const{
    return this->canMoveTo(position.first, position.second);
}

Cell& Field::getCell(int x, int y){
    if (this->isValidPos(x, y)){
        Cell& cell = this->cells[y - 1][x - 1];
        return cell;
    }
    throw std::out_of_range("Coordinates out of range, when tried to get cell");
}

Cell& Field::getCell(std::pair<int, int> position){
    return Field::getCell(position.first, position.second);
}

void Field::setTypeToCells(std::pair<int, int> point1, std::pair<int, int> point2, CellType type){
    if(!(this->isValidPos(point1) && this->isValidPos(point2))){
        return;
    }

    int xmax = std::max(point1.first, point2.first);
    int xmin = std::min(point1.first, point2.first);
    int ymax = std::max(point1.second, point2.second);
    int ymin = std::min(point1.second, point2.second);

    for (int i = ymin; i <= ymax; ++i){
        for (int j = xmin; j <= xmax; ++j){
            Cell& cell = this->getCell(j, i);
            cell.setCellType(type);
        }
    }
}

std::pair<int, int> Field::freePosAround(std::pair<int, int> position) const{
    std::pair<int, int> freePos = {0, 0};
    for(int i = position.first - 1; i < position.first + 2; ++i){
        for(int j = position.second - 1; j < position.second + 2; ++j){
            if(this->canMoveTo(i, j) && i != j){
                freePos = std::make_pair(i, j);
                return freePos;
            }
        }
    }
    return freePos;
}