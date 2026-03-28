#include "../../headers/field/Position.h"
#include <cmath>
#include <sstream>

using namespace std;

Position::Position(int x, int y) : x(x), y(y) {}

int Position::getX() const { return x; }

int Position::getY() const { return y; }

void Position::setX(int xNew) { x = xNew; }

void Position::setY(int yNew) { y = yNew; }

bool Position::operator==(const Position& other) const {
    return x == other.x && y == other.y;
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

Position Position::getNeighbour(Direction dir) const {
    Position newPos = *this;
    switch(dir){
        case Direction::UP: 
            newPos.y--;
            break;
        case Direction::DOWN:
            newPos.y++;
            break;
        case Direction::LEFT: 
            newPos.x--; 
            break;
        case Direction::RIGHT: 
            newPos.x++; 
            break;
    }
    return newPos;
}

double Position::distanceTo(const Position& other) const {
    return sqrt(pow(x - other.x, 2) + pow(y - other.y, 2));
}

string Position::toString() const {
    return to_string(x) + "," + to_string(y);
}

Position Position::fromString(const string& str) {
    size_t pos = str.find(',');
    if (pos == string::npos) {
        throw invalid_argument("Invalid position format");
    }

    int x = stoi(str.substr(0, pos));
    int y = stoi(str.substr(pos + 1));
    return Position(x, y);
}