//
// Created by Mac on 11.11.2025.
//

#ifndef GAME_TERM_SPELLFACTORY_H
#define GAME_TERM_SPELLFACTORY_H

#include "ISpell.h"

class SpellFactory {
public:
    static std::shared_ptr<ISpell> create(const Type & type);
};


#endif //GAME_TERM_SPELLFACTORY_H