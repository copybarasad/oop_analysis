//
// Created by Mac on 28.10.2025.
//

#ifndef GAME_TERM_IDIRECTDAMAGE_H
#define GAME_TERM_IDIRECTDAMAGE_H
#include "ISpell.h"
#include "../field_objects/Field.h"


class IDirectDamage: public ISpell {

    virtual void castOnPlayer(std::shared_ptr<Player> player) = 0;

};


#endif //GAME_TERM_IDIRECTDAMAGE_H