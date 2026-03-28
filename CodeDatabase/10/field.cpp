#include "field.h"
#include "player.h"
#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
using namespace std;

void Field::setLength(int y){
    if (y < constants::FIELD_MIN_SIZE || y > constants::FIELD_MAX_SIZE){
        throw invalid_argument("Недопустимый размер поля");
    }
    this->length = y;
    for (auto& row : grid){
        row.resize(length);
    }
};

void Field::setWidth(int x){
    if (x < constants::FIELD_MIN_SIZE || x > constants::FIELD_MAX_SIZE){
        throw invalid_argument("Недопустимый размер поля");
    }
    this->width = x;
    grid.resize(width, vector<Cell>(length));
};

Field::Field(int x, int y) : width(0), length(0){
    setWidth(x);
    setLength(y);
}

void Field::validateObstaclePercentage(int percentage){
    if (percentage < 0 || percentage > 100){
        throw invalid_argument("Недопустимый процент препятствий");
    }
}

Field::Field(int x, int y, int obstaclePercentage) : Field(x, y) {
    validateObstaclePercentage(obstaclePercentage);
    generateObstacles(obstaclePercentage);
};

Field::Field(const Field& other) : width(other.width), length(other.length){
    grid.resize(width);
    for (int i = 0; i < width; i++){
        grid[i].resize(length);
        for(int j = 0; j < length; j++){
            grid[i][j] = other.grid[i][j];
        }
    }
};

Field::Field(Field&& other) noexcept : width(other.width), length(other.length), grid(move(other.grid)){
    other.width = 0;
    other.length = 0;
    other.grid.clear();
};

Field& Field::operator=(const Field& other){
    if (this != &other){
        grid.clear();
        width = other.width;
        length = other.length;
        grid.resize(width);
        for (int i = 0; i < width; i++){
            grid[i].resize(length);
            for(int j = 0; j < length; j++){
                grid[i][j] = other.grid[i][j];
            }
        } 
    }
    return *this;
};

Field& Field::operator=(Field&& other) noexcept{
    if (this != &other){
        grid.clear();
        width = other.width;
        length = other.length;
        grid = move(other.grid);
        other.grid.clear();
        other.width = 0;
        other.length = 0;
    }
    return *this;
};

void Field::generateObstacles(int obstaclePercentage){
    int obstacleCount = (width * length) * obstaclePercentage / 100;
    vector<pair<int, int>> availableCells;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < length; j++){
            if (grid[i][j].isEmpty()){
                availableCells.push_back({i, j});
            }
        }
    }
    obstacleCount = min(obstacleCount, (int)availableCells.size());
    
    for (int i = 0; i < obstacleCount * 0.7; i++){
        if (availableCells.empty()) break;
        int randomIndex = rand() % availableCells.size();
        int x = availableCells[randomIndex].first;
        int y = availableCells[randomIndex].second;
        grid[x][y].setObstacle();
        availableCells[randomIndex] = availableCells.back();
        availableCells.pop_back();
    }
    
    int trapCount = obstacleCount * 0.3;
    for (int i = 0; i < trapCount; i++){
        if (availableCells.empty()) break;
        int randomIndex = rand() % availableCells.size();
        int x = availableCells[randomIndex].first;
        int y = availableCells[randomIndex].second;
        grid[x][y].setTrap();
        availableCells[randomIndex] = availableCells.back();
        availableCells.pop_back();
    }
};

bool Field::isValidPosition(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < length);
};

bool Field::isCellFree(int x, int y) const {
    return isValidPosition(x, y) && grid[x][y].isPassable();
};

int Field::getWidth() const { return width;};
int Field::getLength() const { return length;};
Cell& Field::getCell(int x, int y) {
    if (!isValidPosition(x, y)){
        throw out_of_range("Координаты за пределами поля");
    }
    return grid[x][y];
};

const Cell& Field::getCell(int x, int y) const {
    if (!isValidPosition(x, y)){
        throw out_of_range("Координаты за пределами поля");
    }
    return grid[x][y];
};

bool Field::placePlayer(int x, int y){
    bool result = false;
    if (isValidPosition(x, y) && getCell(x, y).isPassable()){
        getCell(x, y).setPlayer();
        result = true;
    }
    return result;
}

bool Field::placeEnemy(int x, int y){
    bool result = false;
    if (isValidPosition(x, y) && getCell(x, y).isPassable()){
        getCell(x, y).setEnemy();
        result = true;
    }
    return result;
}
