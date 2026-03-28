//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_TARGETSELECTION_H
#define GAME_TERM_TARGETSELECTION_H
#include "Position.h"
#include "field_objects/Field.h"

class Field;

class TargetSelection {

    Field & field;

public:

    explicit TargetSelection(Field & field);

    Position selectArea(int areaWidth, int areaHeight, int radius) const;

};


#endif //GAME_TERM_TARGETSELECTION_H