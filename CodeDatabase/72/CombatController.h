//
// Created by bob on 10/15/25.
//

#ifndef GAME_TERM_COMBATCONTROLLER_H
#define GAME_TERM_COMBATCONTROLLER_H
#include "../field_objects/Field.h"


class Field;

class CombatController {

    std::shared_ptr<Field> field;

public:

    explicit CombatController(std::shared_ptr<Field> field);

    void playerShoot(const Direction & direction);
};


#endif //GAME_TERM_COMBATCONTROLLER_H