#pragma once
#include "types.hpp"


class Field {
private:
    Coordinates pos;
    FieldType status;
    FieldType initStatus;

public:
    Field(Coordinates p, FieldType status);

    void setStatus(FieldType status);
    void toInitStatus();

    FieldType getStatus();
    Coordinates getPosition();
};
