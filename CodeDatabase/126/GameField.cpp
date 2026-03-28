#include "../../headers/field/GameField.h"
#include "../../headers/field/Cell.h"
#include "../../headers/field/Position.h"
#include "../../headers/exceptions/GameException.h"
#include <chrono>
#include <random>
#include <sstream>

using namespace std;

GameField::GameField(int w, int h) : width(w), height(h){
    if(w < 10) width = 10;
    if(h < 10) height = 10;
    if(w > 25) width = 25;
    if(h > 25) height = 25;
    
    grid.resize(height, vector<Cell>(width));
    initializeBlockedCells();
}

void GameField::initializeBlockedCells(){
    auto seed = chrono::high_resolution_clock::now().time_since_epoch().count();
    mt19937 gen(seed);
    uniform_int_distribution<> dis(0, 7);
    
    for (int y = 0; y < height; y++){
        for (int x = 0; x < width; x++){
            if(dis(gen) == 5){
                grid[y][x].setBlocked(true);
            }
        }
    }
    grid[height/2][width/2].setBlocked(false);
}

GameField::GameField(const GameField& other) : width(other.width), height(other.height){
    grid = other.grid;
}

GameField::GameField(GameField&& other) noexcept : width(other.width), height(other.height), grid(move(other.grid)){
    other.width = 0;
    other.height = 0;
}

GameField& GameField::operator=(const GameField& other){
    if(this != &other){
        width = other.width;
        height = other.height;
        grid = other.grid;
    }
    return *this;
}

GameField& GameField::operator=(GameField&& other) noexcept {
    if(this != &other){
        width = other.width;
        height = other.height;
        grid = move(other.grid);
        other.width = 0;
        other.height = 0;
    }
    return *this;
}

bool GameField::isValidPosition(const Position& pos) const {
    return pos.getX() >= 0 && pos.getX() < width && pos.getY() >= 0 && pos.getY() < height;
}

bool GameField::isCellFree(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    return !grid[pos.getY()][pos.getX()].getIsOccupied() && !grid[pos.getY()][pos.getX()].getIsBlocked();
}

bool GameField::isCellBlocked(const Position& pos) const {
    if (!isValidPosition(pos)) return false;
    return grid[pos.getY()][pos.getX()].getIsBlocked();
}

void GameField::setPlayerPosition(const Position& pos) {
    if (isValidPosition(pos) && !isCellBlocked(pos)) {
        grid[pos.getY()][pos.getX()].setPlayer();
    }
}

void GameField::setEnemyPosition(const Position& pos) {
    if (isValidPosition(pos) && !isCellBlocked(pos)) {
        grid[pos.getY()][pos.getX()].setEnemy();
    }
}

void GameField::setTowerPosition(const Position& pos) {
    if (isValidPosition(pos) && !isCellBlocked(pos)) {
        grid[pos.getY()][pos.getX()].setTower();
    }
}

void GameField::setTrapPosition(const Position& pos) {
    if (isValidPosition(pos)) {
        grid[pos.getY()][pos.getX()].setTrap();
    }
}

void GameField::removeTrapPosition(const Position& pos) {
    if (isValidPosition(pos)) {
        grid[pos.getY()][pos.getX()].removeTrap();
    }
}

void GameField::clearCell(const Position& pos){
    if (isValidPosition(pos)) {
        grid[pos.getY()][pos.getX()].clear();
    }
}

Cell GameField::getCell(const Position& pos) const {
    if (isValidPosition(pos)) {
        return grid[pos.getY()][pos.getX()];
    }
    return Cell();
}

int GameField::getWidth() const { return width; }

int GameField::getHeight() const { return height; }

string GameField::serialize() const {
    stringstream ss;
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                ss << grid[y][x].serialize();
            }
            if (y < height - 1) ss << "\n";
        }
        return ss.str();
}

void GameField::deserialize(istream& stream) {
    string line;
    for (int y = 0; y < height; y++) {
        if (!getline(stream, line)) {
            throw CorruptedSaveException("", "Unexpected end of field data");
        }
        if (line.length() != static_cast<size_t>(width)) {
            throw CorruptedSaveException("", "Invalid field data - size mismatch");
        }
        for (int x = 0; x < width; x++) {
            grid[y][x].deserialize(line[x]);
        }
    }
}