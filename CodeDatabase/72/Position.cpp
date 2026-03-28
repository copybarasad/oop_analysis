//
// Created by bob on 10/4/25.
//

#include "Position.h"

Position::Position(const int &x, const int &y): x(x), y(y) {};

void Position::setCoords(const int &x, const int &y) {
    this->x = x;
    this->y = y;
}

void Position::setX(const int &x) { this->x = x; }

void Position::setY(const int &y) { this->y = y; }

int Position::getX() const { return this->x; }

int Position::getY() const { return this->y; }

bool Position::operator==(const Position &obj) const {
    return this->x == obj.x && this->y == obj.y;
}

bool Position::operator>(const Position &obj) const {
    return this->x > obj.x && this->y > obj.y;
}

bool Position::operator<(const Position &obj) const {
    return this->x < obj.x && this->y < obj.y;
}

void Position::operator+=(const Position &obj) {
    this->x += obj.x;
    this->y += obj.y;
}

Position Position::operator+(const Position &obj) const {
    return {this->x + obj.x, this->y + obj.y};
}

void Position::load(Reader &reader) {
    reader.read(x);
    reader.read(y);
}

void Position::save(Saver &saver) const {
    saver.save(x);
    saver.save(y);
}





