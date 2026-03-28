//
// Created by bob on 10/15/25.
//

#ifndef GAME_TERM_INPUTCONTROLLER_H
#define GAME_TERM_INPUTCONTROLLER_H
#include <iostream>

#include "../field_objects/Field.h"


class InputController {

public:
    char getCommand() const;

    Direction getShootDirection() const;

    bool saveMenu() const;
};


#endif //GAME_TERM_INPUTCONTROLLER_H