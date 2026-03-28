#include "GameObjects/Field.hpp"


Field::Field(Coordinates coord, FieldType status) {
    this->pos = coord;
    this->status = status;
    this->initStatus = status;
}

void Field::toInitStatus() {
    this->status = this->initStatus;
}

void Field::setStatus(FieldType status) {
    this->status = status;
}


FieldType Field::getStatus() {
    return this->status;
}


Coordinates Field::getPosition() {
    return this->pos;
}
