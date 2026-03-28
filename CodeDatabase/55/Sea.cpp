#include "Sea.h"
#include "Player.h"
#include "Enemy.h"
#include "Vortex.h"
#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

void Sea::allocateField() {
    sea_field = new Sea_sign*[size_y];
    for (int i = 0; i < size_y; ++i) {
        sea_field[i] = new Sea_sign[size_x];
        for (int j = 0; j < size_x; ++j) {
            sea_field[i][j] = Sea_sign();
        }
    }
}

void Sea::deallocateField() {
    if (sea_field) {
        for (int i = 0; i < size_y; ++i) {
            delete[] sea_field[i];
        }
        delete[] sea_field;
        sea_field = nullptr;
    }
}

void Sea::copyField(const Sea& other) {
    allocateField();
    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            sea_field[i][j] = other.sea_field[i][j];
        }
    }
}

Sea::Sea(const GameConfig &cfg) : config(cfg), size_x(cfg.sea_width), size_y(cfg.sea_height), sea_field(nullptr) {
    allocateField();
}

Sea::Sea(const Sea& other) noexcept : size_x(other.size_x), size_y(other.size_y), sea_field(nullptr) {
    copyField(other);
}

Sea::Sea(Sea&& other) noexcept : size_x(other.size_x), size_y(other.size_y), sea_field(other.sea_field) {
    other.sea_field = nullptr;
    other.size_x = 0;
    other.size_y = 0;
}

Sea& Sea::operator=(const Sea& other) {
    if(this != &other) {
        deallocateField();
        size_x = other.size_x;
        size_y = other.size_y;
        copyField(other);
    }
    return *this;
}

Sea& Sea::operator=(Sea&& other) noexcept {
    if (this != &other) {
        deallocateField();
        size_x = other.size_x;
        size_y = other.size_y;
        sea_field = other.sea_field;
        other.sea_field = nullptr;
        other.size_x = 0;
        other.size_y = 0;
    }
    return *this;
}

Sea::~Sea() {
    deallocateField();
}

int Sea::getWidth() {
    return size_x;
}

int Sea::getHeight() {
    return size_y;
}

bool Sea::isValidPosition(Position& pos) {
    return pos.x >= 0 && pos.x < size_x && pos.y >= 0 && pos.y < size_y;
}

Sea_sign& Sea::getSeaZone(Position &pos) {
    if(isValidPosition(pos)) {
        return sea_field[pos.y][pos.x];
    }
    throw out_of_range("\033[41mНеверная позиция для клетки\033[0m");
}

bool Sea::isEmptyPosition(Position& pos) {
    return isValidPosition(pos) && sea_field[pos.y][pos.x].isEmpty();
}

void Sea::removeEntity(Position &pos) {
    if(isValidPosition(pos)){
        sea_field[pos.y][pos.x].setFlag(common_flag);
        sea_field[pos.y][pos.x].setSign(common_flag);
    }
}

bool Sea::setTrapOnSeaZone(Position &pos){
    if(isValidPosition(pos)){
        sea_field[pos.y][pos.x].setFlag(fish_net_flag);
        sea_field[pos.y][pos.x].setSign(fish_net_flag);
        return true;
    }else{
        return false;
    }
}

void Sea::display() {
    cout << setw(4) << left << ' ';
    for(int j = 0; j < size_x; j++) {
        cout << setw(2) << left << j;
    }
    cout << endl;
    for(int y = 0; y < size_y; y++) {
        cout << setw(3) << y << '|';
        for(int x = 0; x < size_x; x++) {
            if(sea_field[y][x].getFlag() == player_flag){
                cout << "\033[34m" << sea_field[y][x].getSign() << "\033[0m" << " ";
            }else if(sea_field[y][x].getFlag() == enemy_flag || sea_field[y][x].getFlag() == octopus_flag){
                cout << "\033[31m" << sea_field[y][x].getSign() << "\033[0m" << " ";
            }else if(sea_field[y][x].getFlag() == vortex_flag){
                cout << "\033[32m" << sea_field[y][x].getSign() << "\033[0m" << " ";
            }else if(sea_field[y][x].getFlag() == fish_net_flag){
                cout << "\033[33m" << sea_field[y][x].getSign() << "\033[0m" << " ";
            }else{
                cout << sea_field[y][x].getSign() << " ";
            }
        }
        cout << '|' << endl;
    }
}

vector<Position> Sea::emptyZones() {
    vector<Position> emptyPositions;
    for(int y = 0; y < size_y; y++) {
        for(int x = 0; x < size_x; x++) {
            if(sea_field[y][x].isEmpty()) {
                Position empty_pos(x, y);
                emptyPositions.push_back(empty_pos);
            }
        }
    }
    return emptyPositions;
}

void Sea::saveTo(SaveFileRAII &out) const{
    out.write(size_x).write(size_y);
    for(int i = 0; i < size_y; i++){
        for(int j = 0; j < size_x; j++){
            sea_field[i][j].saveTo(out);
        }
    }
}

void Sea::loadFrom(SaveFileRAII& in) {
    deallocateField();
    in.read(size_x).read(size_y);
    allocateField();

    for (int y = 0; y < size_y; ++y)
        for (int x = 0; x < size_x; ++x)
            sea_field[y][x].loadFrom(in);
}