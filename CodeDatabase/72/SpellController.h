//
// Created by Mac on 25.10.2025.
//

#ifndef GAME_TERM_SPELLCONTROLLER_H
#define GAME_TERM_SPELLCONTROLLER_H

#include "../field_objects/Field.h"
#include "../Position.h"
#include "../spells/ISpell.h"

class SpellController {
    std::shared_ptr<Field> field;
public:

    explicit SpellController(std::shared_ptr<Field> field);

    void useSpell();
};


#endif //GAME_TERM_SPELLCONTROLLER_H