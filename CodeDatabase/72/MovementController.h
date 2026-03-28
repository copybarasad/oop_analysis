//
// Created by bob on 10/15/25.
//

#ifndef GAME_TERM_MOVEMENTCONTROLLER_H
#define GAME_TERM_MOVEMENTCONTROLLER_H
#include "../field_objects/Field.h"

class MovementController {

    std::shared_ptr<Field> field;

    Position directionToOffset(const Direction & direction);

public:

    explicit MovementController(std::shared_ptr<Field> field);

    bool movePlayer(const Direction & direction);

    void moveEnemies();

};


#endif //GAME_TERM_MOVEMENTCONTROLLER_H