#include "field.h"
#include "player.h"
#include "enemy.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <algorithm>
using namespace std;

//сеттеры
void Field::setlength(int y){
    if (y < 10 || y > 25){
        throw invalid_argument("Incorrect size of field");
    }
    this->length = y;
    for (auto& i : arr){
        i.resize(length);
    }
};
void Field::setwidth(int x){
    if (x < 10 || x > 25){
        throw invalid_argument("Incorrect size of field");
    }
    this->width = x;
    arr.resize(width, vector<Node>(length));
};

Field::Field(int x, int y) : width(0), length(0){
    setwidth(x);
    setlength(y);
}

void Field::checkObs(int obs){
    if (obs < 0 || obs > 100){
        throw invalid_argument("Impossible to fill with such a percentage of obstacles");
    }
}
//конструктор с препятствиями
Field::Field(int x, int y, bool fromUser, int obsPerc) : Field(x, y) {
    if (fromUser){
        vector<pair<int, int>> obstacles = obsFromUser();
        createObstacleField(obstacles);
    } else {
        checkObs(obsPerc);
        createObstacleField(obsPerc);
    }
};
//конструктор копирования
Field::Field (const Field& other) : width(other.width), length(other.length){
    arr.resize(width);
    for (int i = 0; i < width; i++){
        arr[i].resize(length);
        for(int j = 0; j < length; j++){
            arr[i][j] = other.arr[i][j];
        }
    }
};
//конструктор перемещения
Field::Field (Field&& other) noexcept : width(other.width), length(other.length), arr(move(other.arr)){
    other.width = 0;
    other.length = 0;
    other.arr.clear();
};
//оператор присваивания с копированием
Field& Field:: operator= (const Field& other){
    if (this != &other){
        arr.clear();
        width = other.width;
        length = other.length;
        arr.resize(width);
        for (int i = 0; i < width; i++){
            arr[i].resize(length);
            for(int j = 0; j < length; j++){
                arr[i][j] = other.arr[i][j];
            }
        } 
    }
    return *this;
};
//оператор присваивания с перемещением
Field& Field::operator= (Field&& other) noexcept{
    if (this != &other){
        arr.clear();
        width = other.width;
        length = other.length;
        arr = move(other.arr);
        other.arr.clear();
        other.width = 0;
        other.length = 0;
    }
    return *this;
};
// поле с препятствиями
void Field::createObstacleField(int obsPerc){
    int obsCount = (width * length) * obsPerc / 100;
    vector<pair<int, int>> freeNodes;
    for (int i = 0; i < width; i++){
        for (int j = 0; j < length; j++){
            if (arr[i][j].isEmpty() && !(i < 4 && j < 4)) {
                freeNodes.push_back({i, j});
            }
        }
    }
    obsCount = min(obsCount, (int)freeNodes.size());
    for (int i = 0; i < obsCount; i++){
        int randomIndex = rand() % freeNodes.size();
        int x = freeNodes[randomIndex].first;
        int y = freeNodes[randomIndex].second;
        arr[x][y].setObstacle();
        freeNodes[randomIndex] = freeNodes.back();
        freeNodes.pop_back();
    }
};
vector<pair<int, int>> Field::obsFromUser(){
    vector<pair<int, int>> obstacles;
    int x, y;
    cout << "Введите координаты препятствий (x y). Для завершения введите невалидные координаты:\n";
    while(true){
        cin >> x >> y;
        if (!validpos(x, y)){
            break;
        }
        obstacles.push_back({x, y});
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return obstacles;
};
void Field:: createObstacleField(const vector<pair<int, int>>& coords){
    for (const auto& coord : coords){
        if (arr[coord.first][coord.second].isEmpty()){
            arr[coord.first][coord.second].setObstacle();
        }
    }
};
// проверки
bool Field::validpos(int x, int y) const {
    return (x >= 0 && x < width && y >= 0 && y < length);
};
//геттеры
int Field::getwidth() const { return width;};
int Field::getlength() const { return length;};
Node& Field::getNode(int x, int y) {
    if (x < 0 || x >= width || y < 0 || y >= length){
        throw out_of_range("Field coordinates out of bounds");
    }
    return arr[x][y];
};
const Node& Field::getNode(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= length){
        throw out_of_range("Field coordinates out of bounds");
    }
    return arr[x][y];
}

bool Field::placePlayer(int x, int y){
    bool res = false;
    if (validpos(x, y) && getNode(x, y).isPassable()){
        getNode(x, y).setPlayer();
        res = true;
    }
    return res;
}
bool Field::placeEnemy(int x, int y){
    bool res = false;
    if (validpos(x, y) && getNode(x, y).isPassable()){
        getNode(x, y).setEnemy();
        res = true;
    }
    return res;
}
bool Field::placeTower(int x, int y){
    bool res = false;
    if (validpos(x, y) && getNode(x, y).isEmpty()){
        getNode(x, y).setTower();
        res = true;
    }
    return res;
}

void Field::resize(int newWidth, int newHeight) {
    *this = Field(newWidth, newHeight); 
}
void Field::resizeWithObstacles(int newWidth, int newHeight, int obstaclePercent) {
    *this = Field(newWidth, newHeight, false, obstaclePercent);
}