//
// Created by Mac on 28.10.2025.
//

#ifndef GAME_TERM_ITRAPSPELL_H
#define GAME_TERM_ITRAPSPELL_H
#include "ISpell.h"
#include "../Position.h"

class ITrapSpell: public ISpell {

    virtual bool canPost(Field & field, const Position & target) const = 0;

};

#endif //GAME_TERM_ITRAPSPELL_H